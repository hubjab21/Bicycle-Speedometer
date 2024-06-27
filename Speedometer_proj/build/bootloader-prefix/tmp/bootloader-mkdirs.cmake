# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/mmich/esp-idf/components/bootloader/subproject"
  "C:/Users/mmich/Desktop/Git_repo_for_speedometer/Bicycle-Speedometer_MM/Speedometer_proj/build/bootloader"
  "C:/Users/mmich/Desktop/Git_repo_for_speedometer/Bicycle-Speedometer_MM/Speedometer_proj/build/bootloader-prefix"
  "C:/Users/mmich/Desktop/Git_repo_for_speedometer/Bicycle-Speedometer_MM/Speedometer_proj/build/bootloader-prefix/tmp"
  "C:/Users/mmich/Desktop/Git_repo_for_speedometer/Bicycle-Speedometer_MM/Speedometer_proj/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/mmich/Desktop/Git_repo_for_speedometer/Bicycle-Speedometer_MM/Speedometer_proj/build/bootloader-prefix/src"
  "C:/Users/mmich/Desktop/Git_repo_for_speedometer/Bicycle-Speedometer_MM/Speedometer_proj/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/mmich/Desktop/Git_repo_for_speedometer/Bicycle-Speedometer_MM/Speedometer_proj/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/mmich/Desktop/Git_repo_for_speedometer/Bicycle-Speedometer_MM/Speedometer_proj/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
