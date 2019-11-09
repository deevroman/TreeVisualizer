cmake_minimum_required(VERSION 3.15.3)
project(myqtproject)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
#set(CMAKE_BUILD_TYPE "Relese")
find_package(Qt5 COMPONENTS Widgets REQUIRED)
add_executable(
        myqtproject
        mainwindow.cpp
        mainwindow.h
        main.cpp
        NodeButton.h)

target_link_libraries(myqtproject Qt5::Widgets)