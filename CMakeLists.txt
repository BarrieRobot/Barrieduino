cmake_minimum_required(VERSION 2.8.3)
project(barrieduino)

find_package(catkin REQUIRED COMPONENTS
  rosserial_arduino
  rosserial_client
)

catkin_package()

rosserial_generate_ros_lib(
  PACKAGE rosserial_arduino
  SCRIPT make_libraries.py
)

rosserial_configure_client(
  DIRECTORY firmware
  TOOLCHAIN_FILE ${ROSSERIAL_ARDUINO_TOOLCHAIN}
)

rosserial_add_client_target(firmware code ALL)
rosserial_add_client_target(firmware code-upload)

#add_custom_target(target_name ALL catkin_make WORKING_DIRECTORY /home/casper/catkin_ws/ SOURCES ${SOURCE_FILES})
#include(libs.cmake)
#add_executable(my_exec ${SOURCE_FILES})
