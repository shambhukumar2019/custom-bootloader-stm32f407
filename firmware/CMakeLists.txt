add_subdirectory(config)
add_subdirectory(vendor)
add_subdirectory(hal)
add_subdirectory(drivers)
add_subdirectory(bsp)
add_subdirectory(app)

add_executable(${CMAKE_PROJECT_NAME}.elf 
    ${CMAKE_CURRENT_SOURCE_DIR}/app/main.c
)

target_link_libraries(${CMAKE_PROJECT_NAME}.elf PUBLIC
    stm32f407_mcu
    config
    vendor
    hal
    drivers
    bsp
    app
)

add_fw_artifacts(${CMAKE_PROJECT_NAME})

