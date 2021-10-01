cmake_minimum_required(VERSION 3.0)

project(lvgl_demo C)

#include_directories(${CMAKE_SOURCE_DIR})
set(LVGL_CONFIG_DIR ${CMAKE_SOURCE_DIR}/config)


add_subdirectory(lvgl)
add_subdirectory(lv_drivers)


add_executable(lvgl_demo main.c)
target_link_libraries(lvgl_demo lvgl lv_driver_sdl)
target_include_directories(lvgl_demo PRIVATE ${CMAKE_SOURCE_DIR}/lvgl/src)
