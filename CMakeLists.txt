cmake_minimum_required(VERSION 3.15.3)
project(myqtproject)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_BUILD_TYPE "Release")
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(
        myqtproject
        mainwindow.cpp
        mainwindow.h
        main.cpp
        )

target_link_libraries(myqtproject Qt5::Widgets)