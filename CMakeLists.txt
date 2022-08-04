cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 20)

project(lvgl_demo C CXX)

cmake_policy(SET CMP0077 NEW)

#include_directories(${CMAKE_SOURCE_DIR})
set(LVGL_CONFIG_DIR ${CMAKE_SOURCE_DIR}/config)
set(LV_CONF_PATH ${CMAKE_SOURCE_DIR}/config/lv_conf.h)


add_subdirectory(lvgl)
add_subdirectory(lv_drivers)


add_executable(lvgl_demo main.cpp init.c)
target_link_libraries(lvgl_demo lvgl lv_driver_sdl pthread)
#target_link_libraries(lvgl_demo lv_driver_gtk pthread)

#target_include_directories(lvgl_demo PRIVATE ${CMAKE_SOURCE_DIR}/lvgl/src)
