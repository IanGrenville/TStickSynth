#pragma once
/*
Copyright 2023 Travis J. West, Input Devices and Music Interaction Laboratory
(IDMIL), Centre for Interdisciplinary Research in Music Media and Technology
(CIRMMT), McGill University, Montréal, Canada, and Univ. Lille, Inria, CNRS,
Centrale Lille, UMR 9189 CRIStAL, F-59000 Lille, France

SPDX-License-Identifier: MIT
*/

// include statements

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/i2s_std.h>
#include <driver/gpio.h>

namespace sygaldry { namespace sensors { namespace esp32 {

/*! \addtogroup sygse
*/
/// \{

/*! \defgroup sygse-newcomponent New Component
*/
/// \{

/*! \brief brief doc

detailed doc
*/
struct WaveSynth
: name_<"Wavetable Synthesizer">
, description_<"Description goes here">
, author_<"Ian Bruce Grenville">
, copyright_<"Copyright 2023 Sygaldry Contributors">
, license_<"SPDX-License-Identifier: MIT">
, version_<"0.0.0">
{
    struct inputs_t {
        array<"frequencies", NUM_WAVETABLE, "Frequencies for each wavetable", float, 20.0f, 20000.0f> freqs;
        array<"mix", NUM_WAVETABLE, "Relative mix of each wavetable", float, 0.0f, 1.0f> mix;
    } inputs;

    struct outputs_t {
    } outputs;

    /// brief doc
    void init();

    /// brief doc
    void main();
}

/// \}
/// \}

} } }
