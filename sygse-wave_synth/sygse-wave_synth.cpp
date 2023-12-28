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
//#include <driver/i2s_std.h>
//#include <driver/i2s_pdm.h>
#include <driver/dac_continuous.h>
#include <cmath>
#include <cstring>

namespace sygaldry { namespace sygse {

//Two different audio buffers, so the audio task can output audio while the instrument task is modifying the paramters of the sound


struct synth_state {
    uint8_t wavetables[NUMBER_WAVETABLES][WAVETABLE_SIZE]; 
      array<"frequencies", NUMBER_WAVETABLES,
         "Frequencies for each wavetable", 
         float, 20.0f, 9000.0f, 440.0f> frequencies;

       array<"mix", NUMBER_WAVETABLES, "Relative mix of each wavetable", float, 0.0f, 1.0f, 0.5f> mix;

       array<"indices", NUMBER_WAVETABLES, "Current index into each wavetable", float, 0.0f, WAVETABLE_SIZE, 0.0f> indices;
} state_1, state_2, *active_state;


uint8_t w_buf[BUFF_SIZE];

float freq_scale = (WAVETABLE_SIZE*1.0f)/SAMPLE_RATE;

//i2s_chan_handle_t tx_chan;        // I2S tx channel handler

static bool IRAM_ATTR dac_on_convert_done_callback(dac_continuous_handle_t handle, const dac_event_data_t *event, void *user_data)
{
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



void i2s_write_task_(void *params)
{
    printf("Synth Loop\n");
    //i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO,   I2S_ROLE_MASTER);   
    //ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &tx_chan, NULL));

   /* i2s_pdm_tx_config_t pdm_cfg = {
        .clk_cfg = I2S_PDM_TX_CLK_DAC_DEFAULT_CONFIG(44100),
        .slot_cfg = I2S_PDM_TX_SLOT_DAC_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .clk = GPIO_NUM_4,
            .dout = GPIO_NUM_5,
            .invert_flags = {
                .clk_inv = false,
            },
        },
    } */

    /*i2s_std_config_t std_cfg = {
        .clk_cfg  =  I2S_STD_CLK_DEFAULT_CONFIG(48000),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,    
            .bclk = GPIO_NUM_26,
            .ws   = GPIO_NUM_27,
            .dout = GPIO_NUM_25,
            .din  = I2S_GPIO_UNUSED,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv   = false,
            },
        },
    }; 
    Initialize the channels
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_chan, &std_cfg));
    */

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
    //ESP_ERROR_CHECK(i2s_channel_enable(tx_chan));


    QueueHandle_t que = xQueueCreate(10, sizeof(dac_event_data_t));
    assert(que);
    dac_event_callbacks_t cbs = {
        .on_convert_done = dac_on_convert_done_callback,
        .on_stop = NULL,
    };
    /* Must register the callback if using asynchronous writing */

    ESP_ERROR_CHECK(dac_continuous_register_event_callback(dac_handle, &cbs, que));
    ESP_ERROR_CHECK(dac_continuous_start_async_writing(dac_handle));


    
    int samples_missed = 0; //If we missed writing samples before, we don't wanna overwrite those
    

    while (1) {
        dac_event_data_t evt_data;
        size_t byte_written = 0;
        struct synth_state play_state;
        play_state = *active_state;
        /* Receive the event from callback and load the data into the DMA buffer until the whole audio loaded */
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
                //printf("%d, %d, %d, %d, %d \n",w_buf[i],lower_index,upper_index,wavetable[lower_index],wavetable[upper_index]);
                phase_acc += phase_incr;
                if (phase_acc >= WAVETABLE_SIZE) {
                    phase_acc -= WAVETABLE_SIZE;
                }
            }  
            //We can get away with doing this because of the fact that the main loop doesn't touch the indices
            active_state->indices[wt_num] = phase_acc; 
        }
        while (byte_written < BUFF_SIZE) {
            xQueueReceive(que, &evt_data, 100);
            size_t loaded_bytes = 0;
            ESP_ERROR_CHECK(dac_continuous_write_asynchronously(dac_handle, (uint8_t*) evt_data.buf, evt_data.buf_size,
                                                                w_buf + byte_written, BUFF_SIZE - byte_written, &loaded_bytes));
            byte_written += loaded_bytes;
        }
        //Clear the legacy data in DMA, clear times equal to the 'dac_continuous_config_t::desc_num' 
        /*for (int i = 0; i < 4; i++) {
            xQueueReceive(que, &evt_data, portMAX_DELAY);
            memset(evt_data.buf, 0, evt_data.buf_size);
        }*/
        //vTaskDelay(pdMS_TO_TICKS(1000));
    }

    while (false) {
        
        //dac_continuous_write(dac_handle,(uint8_t *)w_buf,BUFF_SIZE,&w_bytes,100);

        /*for (int i = 0; i< BUFF_SIZE; i++){
            printf("%d\n", (uint8_t) w_buf[i]);
        }*/
        /* for (int i = 0; i < BUFF_SIZE/2; i++){
            int temp = w_buf[(2*i)+1];
            w_buf[(2*i)+1] = w_buf[2*i];
            w_buf[2*i] = temp;
        } 
        
        
        //Write i2s data 
        if (i2s_channel_write(tx_chan, w_buf, BUFF_SIZE, &w_bytes, 10000) == ESP_OK) {
            //printf("Write Task: i2s write %d bytes\n",w_bytes);
        } else {
           printf("Write Task: i2s write failed\n");
        }
        
        //Correct for any missed samples by putting them at the start of the next buffer we'll send
        if(w_bytes < 2*BUFF_SIZE){
            samples_missed = BUFF_SIZE - (w_bytes/2);
            //printf("Correcting misses: %d \n", samples_missed);
            for(int i = 0; i < samples_missed; i++){
                w_buf[i] = w_buf[(BUFF_SIZE - samples_missed)+i];
            }
        }
        */
    }
}    

void WaveSynth::init()
{
    printf("Initializating Synth\n");
    //Configure other parameters
    state_1.frequencies[0] = 200.0f;
    state_1.frequencies[1] = 200.0f;
    state_1.frequencies[2] = 540.0f;
    state_1.frequencies[3] = 200.0f;
    state_1.mix[0] = 1.0f;
    state_1.mix[1] = 0.0f;
    state_1.mix[2] = 0.0f;
    state_1.mix[3] = 0.0f;
    //Initialize the wavetable with a single period of a sine wave
    for (int i = 0; i < WAVETABLE_SIZE; i++){
        //Sin wavetable
        float sample = 0.0f;
        sample = (sinf(i*(2*3.14159/1024.0f)) + 1)/2;
        state_1.wavetables[0][i] = (uint8_t) (sample*255.0f);
        //printf("WT0: %d,%f, %d \n",i,sample,state_1.wavetables[0][i]);
        //Square wavetable
        sample = (i < WAVETABLE_SIZE/2) ? 1 : 0;
        state_1.wavetables[1][i] = (uint8_t) (sample*255.0f);
        //printf("WT1: %d,%f, %d \n",i,sample,state_1.wavetables[1][i]);
        //Saw wavetable
        sample = ((float) i)/WAVETABLE_SIZE;
        state_1.wavetables[2][i] = (uint8_t) (sample*255.0f);
        //Noise wavetable
        //We want the noise wavetable to still have some pitch, so it's derived from the saw table
        state_1.wavetables[3][i] = ((rand() % 256));
        if(i > 1){
            state_1.wavetables[3][i-2] = state_1.wavetables[3][i];
        }
    }
    inputs.frequencies = state_1.frequencies;
    inputs.mix = state_1.mix;
    state_2 = state_1;
    active_state = &state_1;
    //Create and start synth thread
    printf("Starting loop\n");


    xTaskCreate(i2s_write_task_, "i2s_write_task", 2*65536, NULL, 5, NULL);
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
    for (int i = 0; i < NUMBER_WAVETABLES; i++){
        update_state->frequencies[i] = inputs.frequencies[i];
        update_state->mix[i] = inputs.mix[i];
    }
    //Once all the params are updated, we switch the synth to the new state
    active_state = update_state;
}

} }
