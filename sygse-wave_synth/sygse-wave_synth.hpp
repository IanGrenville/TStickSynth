#pragma once
/*
Copyright 2023 Travis J. West, Input Devices and Music Interaction Laboratory
(IDMIL), Centre for Interdisciplinary Research in Music Media and Technology
(CIRMMT), McGill University, Montréal, Canada, and Univ. Lille, Inria, CNRS,
Centrale Lille, UMR 9189 CRIStAL, F-59000 Lille, France

SPDX-License-Identifier: MIT
*/

// include statements


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
