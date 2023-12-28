# This file is configured by CMake automatically as DartConfiguration.tcl
# If you choose not to use CMake, this file may be hand configured, by
# filling in the required variables.


# Configuration directories and files
SourceDirectory: /home/ian/TStickProject/sygaldry/dependencies/eigen
BuildDirectory: /home/ian/TStickProject/sygaldry/sygaldry-instruments/t_stick_synth/build/esp-idf/main/sygbuild/dependencies/eigen

# Where to place the cost data store
CostDataFile: 

# Site is something like machine.domain, i.e. pragmatic.crd
Site: s-tier-abacus

# Build name is osname-revision-compiler, i.e. Linux-2.4.2-2smp-c++
BuildName: generic-g++-12.2.0-32bit

# Subprojects
LabelsForSubprojects: 

# Submission information
SubmitURL: http://my.cdash.org/submit.php?project=Eigen
SubmitInactivityTimeout: 

# Dashboard start time
NightlyStartTime: 00:00:00 UTC

# Commands for the build/test/submit cycle
ConfigureCommand: "/nix/store/fic9avx8yabwb3ib8m49ld3kq9fkwwwd-cmake-3.25.3/bin/cmake" "/home/ian/TStickProject/sygaldry/dependencies/eigen"
MakeCommand: /nix/store/fic9avx8yabwb3ib8m49ld3kq9fkwwwd-cmake-3.25.3/bin/cmake --build . --target buildtests --config "${CTEST_CONFIGURATION_TYPE}" --  
DefaultCTestConfigurationType: Release

# version control
UpdateVersionOnly: 

# CVS options
# Default is "-d -P -A"
CVSCommand: 
CVSUpdateOptions: 

# Subversion options
SVNCommand: 
SVNOptions: 
SVNUpdateOptions: 

# Git options
GITCommand: /nix/store/48bh1pp2qh6wfd0pnr5vlvdhkc6djdz3-git-2.40.1/bin/git
GITInitSubmodules: 
GITUpdateOptions: 
GITUpdateCustom: 

# Perforce options
P4Command: 
P4Client: 
P4Options: 
P4UpdateOptions: 
P4UpdateCustom: 

# Generic update command
UpdateCommand: /nix/store/48bh1pp2qh6wfd0pnr5vlvdhkc6djdz3-git-2.40.1/bin/git
UpdateOptions: 
UpdateType: git

# Compiler info
Compiler: /home/ian/TStickProject/sygaldry/nixenv/esp-idf-tools/tools/xtensa-esp32-elf/esp-12.2.0_20230208/xtensa-esp32-elf/bin/xtensa-esp32-elf-g++
CompilerVersion: 12.2.0

# Dynamic analysis (MemCheck)
PurifyCommand: 
ValgrindCommand: 
ValgrindCommandOptions: 
DrMemoryCommand: 
DrMemoryCommandOptions: 
CudaSanitizerCommand: 
CudaSanitizerCommandOptions: 
MemoryCheckType: 
MemoryCheckSanitizerOptions: 
MemoryCheckCommand: /bin/valgrind
MemoryCheckCommandOptions: 
MemoryCheckSuppressionFile: 

# Coverage
CoverageCommand: /nix/store/73z104zi47lnk6km2rl7sn51j6cnnp80-gcc-13.1.0/bin/gcov
CoverageExtraFlags: -l

# Testing options
# TimeOut is the amount of time in seconds to wait for processes
# to complete during testing.  After TimeOut seconds, the
# process will be summarily terminated.
# Currently set to 25 minutes
TimeOut: 1500

# During parallel testing CTest will not start a new test if doing
# so would cause the system load to exceed this value.
TestLoad: 

UseLaunchers: 
CurlOptions: 
# warning, if you add new options here that have to do with submit,
# you have to update cmCTestSubmitCommand.cxx

# For CTest submissions that timeout, these options
# specify behavior for retrying the submission
CTestSubmitRetryDelay: 5
CTestSubmitRetryCount: 3
