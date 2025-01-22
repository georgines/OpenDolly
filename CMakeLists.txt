# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(controlador_camera C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(controlador_camera controlador_camera.cpp )

pico_set_program_name(controlador_camera "controlador_camera")
pico_set_program_version(controlador_camera "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(controlador_camera 0)
pico_enable_stdio_usb(controlador_camera 1)

# add_library(Acionador STATIC Acionador.cpp)
# target_link_libraries(Acionador pico_stdlib)

# add_library(ControleAnalogico STATIC ControleAnalogico.cpp)
# target_link_libraries(ControleAnalogico pico_stdlib hardware_adc)

# add_library(ControleMotorVelocidadeFixa STATIC ControleMotorVelocidadeFixa.cpp)
# target_link_libraries(ControleMotorVelocidadeFixa pico_stdlib)

# add_library(Oled STATIC Oled.cpp)
# target_link_libraries(Oled pico_stdlib pico_binary_info hardware_i2c)

target_sources(controlador_camera PRIVATE  
  Oled.cpp
  Acionador.cpp
  ControleMotorVelocidadeFixa.cpp
  ControleAnalogico.cpp
)
# Add the standard library to the build
target_link_libraries(controlador_camera
        pico_stdlib
        hardware_i2c
        pico_binary_info
        hardware_adc)

# Add the standard include files to the build
target_include_directories(controlador_camera PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(controlador_camera 
        
        )

pico_add_extra_outputs(controlador_camera)

