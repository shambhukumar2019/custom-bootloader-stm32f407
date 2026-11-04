add_library(vendor STATIC
    platform/stm32f4xx/src/system_stm32f4xx.c
    platform/stm32f4xx/src/syscalls.c
    platform/stm32f4xx/src/sysmem.c
    platform/stm32f4xx/startup_stm32f407vgtx.s
)

target_link_libraries(vendor PUBLIC stm32f407_mcu)


target_include_directories(vendor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/platform
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/cmsis
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/stm32f4xx/inc
)
