/*
Copyright 2023 Travis J. West, Input Devices and Music Interaction Laboratory
(IDMIL), Centre for Interdisciplinary Research in Music Media and Technology
(CIRMMT), McGill University, Montréal, Canada, and Univ. Lille, Inria, CNRS,
Centrale Lille, UMR 9189 CRIStAL, F-59000 Lille, France

SPDX-License-Identifier: MIT
*/
#include "sygse-wave_synth.hpp"

namespace sygaldry { namespace sygse {

//Two different audio buffers, so the audio task can output audio while the instrument task is modifying the paramters of the sound
#define NUMBER_WAVETABLES 1

struct synth_state {
    int16_t wavetables[NUMBER_WAVETABLES][1024]; 
    float frequencies[NUMBER_WAVETABLES]; //Playback frequency for each wavetable
    float mix[NUMBER_WAVETABLES]; //Vector of relative intensities for each wavetable
} state_1, state_2, *active_state;


int16_t *w_buf;

float freq_scale = BUFF_SIZE/44100.0f;

static i2s_chan_handle_t tx_chan;        // I2S tx channel handler

void i2s_write_task_()
{
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO,   I2S_ROLE_MASTER);   
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &tx_chan, NULL));
    i2s_std_config_t std_cfg = {
        .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(44100),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,    
            .bclk = GPIO_NUM_33,
            .ws   = GPIO_NUM_32,
            .dout = GPIO_NUM_25,
            .din  = I2S_GPIO_UNUSED,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
            },
        },
    };
    /* Initialize the channels */
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_chan, &std_cfg));


    size_t w_bytes = EXAMPLE_BUFF_SIZE;
    ESP_ERROR_CHECK(i2s_channel_enable(tx_chan));
    float phase_acc = 0.0f;
    while (1) {
        for (int wt_num = 0; wt_num < NUMBER_WAVETABLES; wt_num++) {
            int_16_t *wavetable = active_state.wavetables[wt_num];
            float phase_incr = frequency[wave_table]*freq_scale;
            for (int i = 0; i < EXAMPLE_BUFF_SIZE; i++) {
                //Linearly interpolate the samples from the wavetable
                int lower_index = math.floor(phase_acc);
                int upper_index = math.ceil(phase_acc);
                float lower_contribution = upper_index - phase_acc;
                float upper_contribution = phase_acc - lower_index;
                w_buf[i] = math.round(lower_contribution*wavetable[lower_index] + 
                           upper_contribution*wavetable[upper_index]);
                phase_acc += phase_incr;
                if (phase_acc >= BUFF_SIZE) {
                    phase_acc -= BUFF_SIZE;
                }
            }   
        }
        /* Write i2s data */
        if (i2s_channel_write(tx_chan, w_buf, BUFF_SIZE, &w_bytes, 1000) == ESP_OK) {
            printf("Write Task(%d): i2s write %d bytes\n",i,w_bytes);
        } else {
            printf("Write Task: i2s write failed\n");
        }
    }
}    

void WaveSynth::init()
{
    w_buf = (int16_t *)calloc(1, EXAMPLE_BUFF_SIZE);
    //Configure other parameters
    state_1 = {
        .frequencies = {440.0f};
        .mix = {1.0f};
    };
    //Initialize the wavetable with a single period of a sine wave
    for (int i = 0; i < WAVETABLE_SIZE; i++){
        float sample = 0.0f;
        sample = sinf(i*(2*pi/1024.0f));
        state_1.wavetables[0][i] = (int16_t) (sample*1000.0f);
    }
    input.frequencies = state_1.frequencies;
    input.mix = state_1.mix;
    state_2 = state_1;
    active_state = &state_1;
    //Create and start synth thread
    xTaskCreate(i2s_write_task_, "i2s_example_write_task", 4096, NULL, 5, NULL);
}

void WaveSynth::main()
{
    //Update the params for the synth thread based on user input
    //We write to a seperate object to not interfere with the synth while it's playing
    struct synth_state *update_state;
    if(active_state == &state_1){
        update_state = &state_2;
    }else{
        update_state = &state_1;
    }
    for (int i = 0; i < NUM_WAVETABLE; i++){
        update_state.frequencies[i] = input.frequencies[i];
        update_state.mix[i] = input.mix[i];
    }
    //Once all the params are updated, we switch the synth to the new state
    active_state = update_state;
}

}
