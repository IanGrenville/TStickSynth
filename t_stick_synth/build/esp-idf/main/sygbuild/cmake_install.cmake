# Install script for directory: /home/ian/TStickProject/sygaldry

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/var/empty/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/ian/TStickProject/sygaldry/nixenv/esp-idf-tools/tools/xtensa-esp32-elf/esp-12.2.0_20230208/xtensa-esp32-elf/bin/xtensa-esp32-elf-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/dependencies/pfr/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/dependencies/mp11/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygac-components/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygac-endpoints/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygac-functions/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygac-metadata/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygac-mimu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygac-runtime/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygah-consteval/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygah-endpoints/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygah-string_literal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygah-metadata/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygah-mimu/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygsp-button/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygsp-delay/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygsp-micros/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygsp-mimu_units/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygsp-complementary_mimu_fusion/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygsp-arduino_hack/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygse-arduino_hack/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygsa-two_wire/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygsa-trill_craft/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygse-delay/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygse-micros/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygse-adc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygse-button/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygse-gpio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygsp-icm20948/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygse-trill/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygse-wave_synth/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygbp-cstdio_reader/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygbp-cli/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygbp-liblo/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygbp-osc_match_pattern/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygbp-osc_string_constants/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygbp-output_logger/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygbp-rapid_json/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygbp-session_data/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygbp-spelling/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygbp-test_component/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygbe-runtime/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygbe-libmapper_arduino/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygbe-spiffs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygbe-wifi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygup-basic_logger/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry/sygup-cstdio_logger/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/sygaldry-instruments/test/cmake_install.cmake")
endif()

