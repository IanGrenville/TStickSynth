# Install script for directory: /home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen

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
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE FILE FILES
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/AdolcForward"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/AlignedVector3"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/ArpackSupport"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/AutoDiff"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/BVH"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/EulerAngles"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/FFT"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/IterativeSolvers"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/KroneckerProduct"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/LevenbergMarquardt"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/MatrixFunctions"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/MoreVectorization"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/MPRealSupport"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/NonLinearOptimization"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/NumericalDiff"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/OpenGLSupport"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/Polynomials"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/Skyline"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/SparseExtra"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/SpecialFunctions"
    "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/Splines"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE DIRECTORY FILES "/home/ian/TStickProject/sygaldry/dependencies/eigen/unsupported/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/dependencies/eigen/unsupported/Eigen/CXX11/cmake_install.cmake")

endif()

