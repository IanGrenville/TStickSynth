/*
Copyright 2023 Travis J. West, https://traviswest.ca, Input Devices and Music
Interaction Laboratory (IDMIL), Centre for Interdisciplinary Research in Music
Media and Technology (CIRMMT), McGill University, Montréal, Canada, and Univ.
Lille, Inria, CNRS, Centrale Lille, UMR 9189 CRIStAL, F-59000 Lille, France

SPDX-License-Identifier: MIT
*/

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <cmath>
//#include <math>
#include "sygac-runtime.hpp"
#include "sygah-metadata.hpp"
#include "sygse-button.hpp"
#include "sygse-adc.hpp"
#include "sygsa-two_wire.hpp"
#include "sygse-trill.hpp"
#include "sygsp-icm20948.hpp"
#include "sygsa-icm20948-two_wire_serif.hpp"
#include "sygsp-complementary_mimu_fusion.hpp"
#include "sygbe-spiffs.hpp"
#include "sygbe-wifi.hpp"
#include "sygbp-liblo.hpp"
#include "sygbp-cli.hpp"
#include "sygbp-output_logger.hpp"
#include "sygse-wave_synth.hpp"
#include "sygah-metadata.hpp"
#include "sygah-endpoints.hpp"
#include "sygac-components.hpp"

using namespace sygaldry;

struct TStickSynth
{
    struct Instrument {
        sygsa::TwoWire<21,22/*,1000000*/> i2c;
        struct Sensors {
            sygse::Button<GPIO_NUM_36> button;
            //sygse::OneshotAdc<33> adc;
            //sygsa::TrillCraft touch;
            sygsp::ICM20948< sygsa::ICM20948TwoWireSerif<sygsp::ICM20948_I2C_ADDRESS_1>
                           , sygsa::ICM20948TwoWireSerif<sygsp::AK09916_I2C_ADDRESS>
                           > mimu;
            sygsp::ComplementaryMimuFusion<decltype(mimu)> mimu_fusion;
            
        } sensors;
        //sygbe::WiFi wifi;
        //sygbp::LibloOsc<Sensors> osc;
    };
    sygse::WaveSynth synth;
    //sygbe::SpiffsSessionStorage<Instrument> session_storage;
    Instrument instrument;
    //sygbp::CstdioCli<Instrument> cli;
} tstick_synth{};

struct EulerAngles {
    double roll;
    double pitch;
    double yaw;
};

constexpr auto runtime = Runtime{tstick_synth};

EulerAngles quaternion_to_euler(float w, float x, float y, float z){
    return {
        .roll = atan2(2*(w*x + y*z),1-2*(x*x + y*y)),
        .pitch = 2*atan2(sqrt(1+2*(w*y - x*z)),sqrt(1-2*(w*y - x*z))) - (M_PI/2),
        .yaw = atan2(2*(w*z+x*y),1-2*(y*y + z*z))
    };
}

const std::vector<float> pitches{
    27.5,
    29.14,
    30.87,
    32.7,
    34.65,
    36.71,
    38.89,
    41.2,
    43.65,
    46.25,
    49.0,
    51.91,
    55.0,
    58.27,
    61.74,
    65.41,
    69.3,
    73.42,
    77.78,
    82.41,
    87.31,
    92.5,
    98.0,
    103.83,
    110.0,
    116.54,
    123.47,
    130.81,
    138.59,
    146.83,
    155.56,
    164.81,
    174.61,
    185.0,
    196.0,
    207.65,
    220.0,
    233.08,
    246.94,
    261.63,
    277.18,
    293.66,
    311.13,
    329.63,
    349.23,
    369.99,
    392.0,
    415.3,
    440.0,
    466.16,
    493.88,
    523.25,
    554.37,
    587.33,
    622.25,
    659.25,
    698.46,
    739.99,
    783.99,
    830.61,
    880.0,
    932.33,
    987.77,
    1046.5,
    1108.73,
    1174.66,
    1244.51,
    1318.51,
    1396.91,
    1479.98,
    1567.98,
    1661.22,
    1760.0,
    1864.66,
    1975.53,
    2093.0,
    2217.46,
    2349.32,
    2489.02,
    2637.02,
    2793.83,
    2959.96,
    3135.96,
    3322.44,
    3520.0,
    3729.31,
    3951.07,
    4186.01
};

extern "C" void app_main(void)
{
    printf("initializing\n");
    runtime.init();
    // give IDF processes time to finish up init business
    vTaskDelay(pdMS_TO_TICKS(100));
    printf("looping\n");
    float energy[3] = {0.0,0.0,0.0};
    float fix_pitch = 200;
    while (true)
    {
        //Just some temporary nonsense to test how much computation it takes to effect the sound
        float j = 0;
        for(int i = 0; i< 1000; i++){
            j+=sqrt(i);
        }
        //
        //printf(runtime.container.instrument.sensors.mimu.outputs.error_message.value().c_str());
        EulerAngles angles = quaternion_to_euler(runtime.container.instrument.sensors.mimu_fusion.outputs.quaternion[0],runtime.container.instrument.sensors.mimu_fusion.outputs.quaternion[1],runtime.container.instrument.sensors.mimu_fusion.outputs.quaternion[2],runtime.container.instrument.sensors.mimu_fusion.outputs.quaternion[3]);
        std::array<float, 3> accl = runtime.container.instrument.sensors.mimu_fusion.outputs.sensor_accl.value();
        std::array<float, 3> angle_rate = runtime.container.instrument.sensors.mimu_fusion.outputs.angular_rate.value();
        energy[0] += abs(angle_rate[0]);
        energy[1] += abs(angle_rate[1]);
        energy[2] += abs(angle_rate[2]);
        if(energy[0] > 10){
            fix_pitch =  *std::lower_bound(pitches.begin(),pitches.end(),200 - angles.pitch*(180));
            
            runtime.container.synth.inputs.frequencies[0] = fix_pitch;
            runtime.container.synth.inputs.frequencies[1] = fix_pitch;
            runtime.container.synth.inputs.frequencies[2] = fix_pitch;
            runtime.container.synth.inputs.frequencies[3] = fix_pitch;
        }
        /*
        printf("######################\n");
        printf("######################\n");
        printf("Angles: %f, %f, %f \n",angles.pitch*(180/M_PI),angles.yaw*(180/M_PI),angles.roll*(180/M_PI));
        printf("Accl:   %f, %f, %f \n",accl[0],accl[1],accl[2]);
        printf("Rate:   %f, %f, %f \n",angle_rate[0],angle_rate[1],angle_rate[2]);
        printf("######################\n");
        printf("######################\n");
        */
       
        //runtime.container.synth.inputs.mix[3] = std::min(std::max(log(energy[0])/3,0.0),1.0);
        runtime.container.synth.inputs.mix[1] = std::min(std::max(log(energy[1])/5,0.0),1.0);
        runtime.container.synth.inputs.mix[0] = std::min(std::max(log(energy[2])/3,0.0),1.0);
        //printf("%4.2f %4.2f %4.2f %4.2f \n",energy[0],energy[1],energy[2],energy[0] + energy[1]);
        //printf("%4.2f %4.2f %4.2f %4.2f %5.2f %4.2f\n",runtime.container.synth.inputs.mix[0],runtime.container.synth.inputs.mix[1],runtime.container.synth.inputs.mix[2],energy[2],fix_pitch,energy[0] + energy[1] + energy[2]);
        energy[0] = energy[0]/1.5;
        energy[1] = energy[1]/1.5;
        energy[2] = energy[2]/1.5;
        runtime.tick();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
