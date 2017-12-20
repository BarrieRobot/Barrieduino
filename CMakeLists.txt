cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME coffee_loader)
project(${PROJECT_NAME} C CXX)

set(${CMAKE_PROJECT_NAME}_SKETCH Barrieduino.ino)
include(libs.cmake)

add_library(mylib wire_scheme.h function_list.h ROS_code.h ROS_include.h LED_ring.h LED_ring.cpp servo_code.cpp Sensors.cpp Sensors.h CoffeeMachine.cpp CoffeeMachine.h)

#### Uncomment below additional settings as needed.
set(${CMAKE_PROJECT_NAME}_BOARD mega)
# set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyACM0)
set(mega.build.mcu atmega2560)
# set(mega.upload.protocol wiring)
# set(mega.upload.speed 115200)

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
