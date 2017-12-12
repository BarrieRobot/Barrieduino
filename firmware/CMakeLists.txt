cmake_minimum_required(VERSION 2.8.3)
set (CMAKE_CXX_STANDARD 11)

include_directories(${ROS_LIB_DIR})
#include_directories(/home/casper/Arduino/libraries/)
include_directories(/home/casper/Arduino/libraries/FastLED)
include_directories(/home/casper/Arduino/libraries/Streaming)
include_directories(/home/casper/Arduino/libraries/OneWire)
include_directories(/home/casper/Arduino/libraries/DallasTemperature)
include_directories(/home/casper/Arduino/libraries/HX711_ADC/src)


# Include libs used in this project
#include(libs.cmake)

# Remove this if using an Arduino without native USB (eg, other than Leonardo)
add_definitions(-DUSB_CON)

generate_arduino_firmware(code
  #SRCS chatter.cpp ${ROS_LIB_DIR}/time.cpp
  SRCS coffee_loader.ino CoffeeMachine.cpp LED_ring.cpp Sensors.cpp servo_code.cpp ${ROS_LIB_DIR}/time.cpp
  BOARD mega2560
  PORT /dev/ttyACM0
)
