\page page-sygin-t_stick T-Stick

Copyright 2023 Ian Bruce Grenville, Input Devices and Music
Interaction Laboratory (IDMIL), McGill University, Montréal, Canada

SPDX-License-Identifier: MIT

[TOC]

## Implementation

```cpp
// @#'main/t_stick_synth.cpp'

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <cmath>
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

//Defines the structure of the instrument for Sygaldry in terms of its components
struct TStickSynth
{
    struct Instrument {
        //The "instrument" component, i.e. the input devices
        sygsa::TwoWire<21,22> i2c;
        struct Sensors {
            sygse::Button<GPIO_NUM_36> button;
            sygsp::ICM20948< sygsa::ICM20948TwoWireSerif<sygsp::ICM20948_I2C_ADDRESS_1>
                           , sygsa::ICM20948TwoWireSerif<sygsp::AK09916_I2C_ADDRESS>
                           > mimu;
            sygsp::ComplementaryMimuFusion<decltype(mimu)> mimu_fusion;
            
        } sensors;
    };
    //The system as a whole consists of the synthesizer and the instrument
    sygse::WaveSynth synth;
    Instrument instrument;
} tstick_synth{};

//Struct for holding a set of angles to describe a rotation
struct EulerAngles {
    double roll, pitch, yaw;
};

constexpr auto runtime = Runtime{tstick_synth};

//Convert the quaternions we get from the mimu_fusion component to angles
//Formulae from https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
EulerAngles quaternion_to_euler(float w, float x, float y, float z){
    return {
        .roll = atan2(2*(w*x + y*z),1-2*(x*x + y*y)),
        .pitch = 2*atan2(sqrt(1+2*(w*y - x*z)),sqrt(1-2*(w*y - x*z))) - (M_PI/2),
        .yaw = atan2(2*(w*z+x*y),1-2*(y*y + z*z))
    };
}
//Array of pitches corresponding to the notes of a standard piano
//The pitch of the instrument is rounded to the nearest element of this array
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

//The main instrument loop that handles user inputs and controls the synth parameters
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
        
        std::array<float, 4> quaternion = runtime.container.instrument.sensors.mimu_fusion.outputs.quaternion;
        EulerAngles angles = quaternion_to_euler(quaternion[0],quaternion[1],quaternion[2],quaternion[3]);
        std::array<float, 3> accl = runtime.container.instrument.sensors.mimu_fusion.outputs.sensor_accl.value();
        std::array<float, 3> angle_rate = runtime.container.instrument.sensors.mimu_fusion.outputs.angular_rate.value();
        energy[0] += abs(angle_rate[0]);
        energy[1] += abs(angle_rate[1]);
        energy[2] += abs(angle_rate[2]);
        if(energy[0] > 10){
            fix_pitch =  *std::lower_bound(pitches.begin(),pitches.end(),200 - angles.pitch*(180));       
            runtime.container.synth.inputs.frequencies.fill(fix_pitch);
        }
        runtime.container.synth.inputs.mix[1] = std::min(std::max(log(energy[1])/5,0.0),1.0);
        runtime.container.synth.inputs.mix[0] = std::min(std::max(log(energy[2])/3,0.0),1.0);
        energy[0] = energy[0]/1.5;
        energy[1] = energy[1]/1.5;
        energy[2] = energy[2]/1.5;
        runtime.tick();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
// @/
```

## Build Boilerplate

The `idf.py init` generated boilerplate `CMakeLists.txt` for an `esp-idf` project:

```cmake
# @#'CMakeLists.txt'
cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(t-stick_synth)
# @/
```

```cmake
# @#'main/CMakeLists.txt'
idf_component_register(SRCS "t_stick_synth.cpp"
        )
add_subdirectory(../../../ sygbuild) # add sygaldry as a subdirectory
target_compile_options(${COMPONENT_LIB} PRIVATE "-Wfatal-errors" "-ftemplate-backtrace-limit=0")
target_link_libraries(${COMPONENT_LIB} PRIVATE sygaldry)
# @/
```

## Partition Table

In order to use SPIFFS for session data storage, we are required
to provide a custom partition table that declares a data partition with
`spiffs` subtype where the SPIFFS will be located.

```csv
# @#'partitions.csv'
# name,   type, subtype,   offset,     size,   flags
nvs,      data, nvs,       0x9000,   0x4000,
phy_init, data, phy,       0xf000,   0x1000,
main,     app,  factory,  0x10000, 0x290000,
storage,  data, spiffs,  0x300000,       1M,
# @/
```

The `sdkconfig` is then directed to use this partition table by setting
the following two lines:

```
CONFIG_PARTITION_TABLE_CUSTOM=y
CONFIG_PARTITION_TABLE_CUSTOM_FILENAME="partitions.csv"
```

This is done manually to avoid having to reproduce the entire `sdkconfig` here.
