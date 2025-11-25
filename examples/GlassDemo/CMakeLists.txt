cmake_minimum_required(VERSION 3.16)

project(GlassDemo LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(GlassDemo
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

target_link_libraries(GlassDemo PRIVATE Qt6::Core Qt6::Widgets QtLiquidGlass)

