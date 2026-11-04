set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_SYSTEM_PROCESSOR "ARM")

set(MCU "STM32F407VGTX")

# Set the root path for finding libraries, includes, and packages to target root
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Set the compiler executables for cross-compiling to ARM architecture
set(TOOLCHAIN_PREFIX "arm-none-eabi-")
find_program(CMAKE_C_COMPILER           ${TOOLCHAIN_PREFIX}gcc      REQUIRED)
find_program(CMAKE_ASM_COMPILER         ${TOOLCHAIN_PREFIX}gcc      REQUIRED)
find_program(CMAKE_CXX_COMPILER         ${TOOLCHAIN_PREFIX}g++      REQUIRED)
find_program(CMAKE_OBJCOPY              ${TOOLCHAIN_PREFIX}objcopy  REQUIRED)
find_program(CMAKE_OBJDUMP              ${TOOLCHAIN_PREFIX}objdump  REQUIRED)
find_program(CMAKE_SIZE                 ${TOOLCHAIN_PREFIX}size     REQUIRED)

# Do not build an executable for try_compile() tests, just a static library.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)







