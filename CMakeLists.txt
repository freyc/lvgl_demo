cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)

project(lvgl_demo C CXX)

#include_directories(${CMAKE_SOURCE_DIR})
set(LVGL_CONFIG_DIR ${CMAKE_SOURCE_DIR}/config)


add_subdirectory(lvgl)
add_subdirectory(lv_drivers)


add_executable(lvgl_demo main.cpp init.c)
target_link_libraries(lvgl_demo lvgl lv_driver_sdl)
#target_link_libraries(lvgl_demo lv_driver_gtk pthread)

#target_include_directories(lvgl_demo PRIVATE ${CMAKE_SOURCE_DIR}/lvgl/src)
