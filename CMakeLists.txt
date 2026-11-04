cmake_minimum_required(VERSION 3.20)

project(blink LANGUAGES C ASM)

# generate compile_commands.json to help ide's with code completion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${CMAKE_SOURCE_DIR}/config/stm32f407_target.cmake)
include(${CMAKE_SOURCE_DIR}/config/stm32f4_post_build.cmake)

add_subdirectory(firmware)





