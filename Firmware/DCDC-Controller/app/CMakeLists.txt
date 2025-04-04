# SPDX-License-Identifier: Apache-2.0
cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DTC_OVERLAY_FILE ${CMAKE_CURRENT_LIST_DIR}/usb_pdmon.overlay)
set(WCH_SDK_DIR ${CMAKE_CURRENT_LIST_DIR}/../packages/ch32x035/EVT/EXAM/SRC CACHE PATH "" FORCE)
set(ZEPHYR_EXTRA_MODULES "${CMAKE_CURRENT_LIST_DIR}/../packages/zephyrboards")
list(APPEND ZEPHYR_EXTRA_MODULES "${CMAKE_CURRENT_LIST_DIR}/../packages/BQ2575x")

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(usb_c_sink)

# target_sources(app PRIVATE src/main.c src/power_ctrl.c)
target_sources(app PRIVATE src/main.cpp)
