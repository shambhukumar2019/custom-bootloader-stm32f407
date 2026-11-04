# compiler and linker options and flags for the STM32F407 target

# add_compile_options(-O0 -g -Wall -Wextra -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections)

# add_link_options(-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard --specs=nano.specs -Wl,--print-memory-usage -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -T${CMAKE_SOURCE_DIR}/firmware/vendor/platform/stm32f4xx/STM32F407VGTX.ld)

add_library(stm32f407_mcu INTERFACE)

target_compile_options(stm32f407_mcu INTERFACE -Wall -Wextra -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -ffunction-sections -fdata-sections)

target_link_options(stm32f407_mcu INTERFACE -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard --specs=nano.specs -Wl,--print-memory-usage -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -T${CMAKE_SOURCE_DIR}/firmware/vendor/platform/stm32f4xx/STM32F407VGTX.ld)


