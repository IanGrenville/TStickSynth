\page page-sygse-wave_synth Wavetable Synthesizer

Copyright 2023 Ian Bruce Grenville, Input Devices and Music Interaction Laboratory
(IDMIL), McGill University, Montréal, Canada

SPDX-License-Identifier: MIT

[TOC]

```cpp
// @#'sygse-wave_synth.hpp'
#pragma once

#include "sygah-metadata.hpp"
#include "sygah-endpoints.hpp"

namespace sygaldry { namespace sygse {

/*! \addtogroup sygse
*/
/// \{

/*! \defgroup sygse-newcomponent New Component
*/
/// \{

/*! \brief brief doc

detailed doc
*/

constexpr const int NUMBER_WAVETABLES = 4;
constexpr const int WAVETABLE_SIZE = 1024;
constexpr const int BUFF_SIZE = 1000;
constexpr const int SAMPLE_RATE = 40000;

struct WaveSynth : name_<"Wavetable Synthesizer">
, author_<"Ian Bruce Grenville">
, copyright_<"Copyright 2023 Sygaldry Contributors">
, license_<"SPDX-License-Identifier: MIT">
{
    struct inputs_t {
        array<"frequencies", NUMBER_WAVETABLES,
         "Frequencies for each wavetable", 
         float, 20.0f, 10000.0f, 440.0f> frequencies;

       array<"mix", NUMBER_WAVETABLES, "Relative mix of each wavetable", float, 0.0f, 1.0f, 0.5f> mix;

    } inputs;

    struct outputs_t {
    } outputs;

    /// brief doc
    void init();

    /// brief doc
    void main();
};

/// \}
/// \}

} } 
// @/
```

```cpp
// @#'sygse-wave_synth.cpp'
/*
Copyright 2023 Travis J. West, Input Devices and Music Interaction Laboratory
(IDMIL), Centre for Interdisciplinary Research in Music Media and Technology
(CIRMMT), McGill University, Montréal, Canada, and Univ. Lille, Inria, CNRS,
Centrale Lille, UMR 9189 CRIStAL, F-59000 Lille, France

SPDX-License-Identifier: MIT
*/
#include "sygse-wave_synth.hpp"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <driver/gpio.h>
#include <driver/dac_continuous.h>
#include <cmath>
#include <cstring>

namespace sygaldry { namespace sygse {

//Two different audio buffers, so the audio task can output audio while the instrument task is modifying the paramters of the sound


struct synth_state {
    //A representation of the internal state of the synthesizer
    //This consists of the wavetables, frequencies of each oscillator, mix volume of each, and each's phase index
    uint8_t wavetables[NUMBER_WAVETABLES][WAVETABLE_SIZE]; 
    array<"frequencies", NUMBER_WAVETABLES, "Frequencies for each wavetable", float, 20.0f, 9000.0f, 440.0f> frequencies;
    array<"mix", NUMBER_WAVETABLES, "Relative mix of each wavetable", float, 0.0f, 1.0f, 0.5f> mix;
    array<"indices", NUMBER_WAVETABLES, "Current index into each wavetable", float, 0.0f, WAVETABLE_SIZE, 0.0f> indices;
} state_1, state_2, *active_state;


uint8_t w_buf[BUFF_SIZE];

float freq_scale = (WAVETABLE_SIZE*1.0f)/SAMPLE_RATE;

static bool IRAM_ATTR dac_on_convert_done_callback(dac_continuous_handle_t handle, const dac_event_data_t *event, void *user_data) {
    QueueHandle_t que = (QueueHandle_t)user_data;
    BaseType_t need_awoke;
    /* When the queue is full, drop the oldest item */
    if (xQueueIsQueueFullFromISR(que)) {
        dac_event_data_t dummy;
        xQueueReceiveFromISR(que, &dummy, &need_awoke);
    }
    /* Send the event from callback */
    xQueueSendFromISR(que, event, &need_awoke);
    return need_awoke;
}

void i2s_write_task_(void *params) {
    printf("Synth Loop\n");
    dac_continuous_handle_t dac_handle;
    dac_continuous_config_t cont_cfg = {
        .chan_mask = DAC_CHANNEL_MASK_ALL,
        .desc_num = 4,
        .buf_size = 2048,
        .freq_hz = SAMPLE_RATE,
        .offset = 0,
        .clk_src = DAC_DIGI_CLK_SRC_APLL,   // Using APLL as clock source to get a wider frequency range
        .chan_mode = DAC_CHANNEL_MODE_SIMUL,
    };
    /* Allocate continuous channels */
    ESP_ERROR_CHECK(dac_continuous_new_channels(&cont_cfg, &dac_handle));
    ESP_ERROR_CHECK(dac_continuous_enable(dac_handle));
   
    //Set up the queue for DAC callbacks
    QueueHandle_t que = xQueueCreate(10, sizeof(dac_event_data_t));
    assert(que);
    dac_event_callbacks_t cbs = {
        .on_convert_done = dac_on_convert_done_callback,
        .on_stop = NULL,
    };
    /* Must register the callback if using asynchronous writing */
    ESP_ERROR_CHECK(dac_continuous_register_event_callback(dac_handle, &cbs, que));
    ESP_ERROR_CHECK(dac_continuous_start_async_writing(dac_handle));
    
    while (1) {
        dac_event_data_t evt_data;
        size_t byte_written = 0;
        struct synth_state play_state;
        play_state = *active_state;
        for(int i = 0 ; i < BUFF_SIZE; i++){
            w_buf[i] = 0;
        }
        for (int wt_num = 0; wt_num < NUMBER_WAVETABLES; wt_num++) {
            //printf("Outer sample loop: %d \n", wt_num);
            uint8_t *wavetable = play_state.wavetables[wt_num];
            float mix_amt = play_state.mix[wt_num];
            float phase_acc = play_state.indices[wt_num];
            float phase_incr = freq_scale*play_state.frequencies[wt_num];
            for (int i = 0; i < BUFF_SIZE; i++) {
                //Linearly interpolate the samples from the wavetable
                int lower_index = floor(phase_acc);
                int upper_index = ceil(phase_acc);
                float lower_contribution = upper_index - phase_acc;
                float upper_contribution = phase_acc - lower_index;
                w_buf[i] += round(mix_amt*(lower_contribution*wavetable[lower_index] + upper_contribution*wavetable[upper_index]));
                phase_acc += phase_incr;
                if (phase_acc >= WAVETABLE_SIZE) {
                    phase_acc -= WAVETABLE_SIZE;
                }
            }  
            //We can get away with editing the state here because of the fact that the main loop doesn't touch the indices
            active_state->indices[wt_num] = phase_acc; 
        }
        //Write the data out the DAC
        while (byte_written < BUFF_SIZE) {
            xQueueReceive(que, &evt_data, 100);
            size_t loaded_bytes = 0;
            ESP_ERROR_CHECK(dac_continuous_write_asynchronously(dac_handle, (uint8_t*) evt_data.buf, evt_data.buf_size,
                                                                w_buf + byte_written, BUFF_SIZE - byte_written, &loaded_bytes));
            byte_written += loaded_bytes;
        }
    }
}    

void WaveSynth::init() {
    printf("Initializating Synth\n");
    //Configure other parameters
    state_1.frequencies.fill(200.0f);
    //Initialize the wavetable with a single period of a sine wave
    for (int i = 0; i < WAVETABLE_SIZE; i++){
        //Sin wavetable
        float sample = 0.0f;
        sample = (sinf(i*(2*M_PI/1024.0f)) + 1)/2;
        state_1.wavetables[0][i] = (uint8_t) (sample*255.0f);
        //Square wavetable
        sample = (i < WAVETABLE_SIZE/2) ? 1 : 0;
        state_1.wavetables[1][i] = (uint8_t) (sample*255.0f);
        //Saw wavetable
        sample = ((float) i)/WAVETABLE_SIZE;
        state_1.wavetables[2][i] = (uint8_t) (sample*255.0f);
        //Noise wavetable
        //We want the noise to have some frequency so we partially derive it from the saw
        state_1.wavetables[3][i] = (state_1.wavetables[2][i] + (rand() % 256)) % 256;
    }
    inputs.frequencies = state_1.frequencies;
    inputs.mix = state_1.mix;
    state_2 = state_1;
    active_state = &state_1;
    //Create and start synth thread
    printf("Starting loop\n");


    xTaskCreate(i2s_write_task_, "i2s_write_task", 2*65536, NULL, 5, NULL);
}

void WaveSynth::main() {
    //Update the params for the synth thread based on user input
    //We write to a seperate object to not interfere with the synth while it's playing
    struct synth_state *update_state;
    if(active_state == &state_1){
        update_state = &state_2;
    }else{
        update_state = &state_1;
    }
    for (int i = 0; i < NUMBER_WAVETABLES; i++){
        update_state->frequencies[i] = inputs.frequencies[i];
        update_state->mix[i] = inputs.mix[i];
    }
    //Once all the params are updated, we switch the synth to the new state
    active_state = update_state;
}

} }
// @/
```

```cpp
// @#'sygse-wave_synth.test.cpp'
#include <catch2/catch_test_macros.hpp>
#include "sygse-newcomponent.hpp"

using namespace sygaldry;
using namespace sygaldry::sygse;

@{tests}
// @/
```

```cmake
# @#'CMakeLists.txt'
set(lib sygse-wave_synth)
add_library(${lib} STATIC)
target_sources(${lib} PRIVATE ${lib}.cpp)
target_include_directories(${lib} PUBLIC .)
target_link_libraries(${lib}
        PUBLIC sygah-endpoints
        PUBLIC sygah-metadata
        PRIVATE idf::driver
        )
# @/
```
