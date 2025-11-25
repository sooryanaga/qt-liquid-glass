cmake_minimum_required(VERSION 3.16)

project(QtLiquidGlass LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

if(APPLE)
    enable_language(OBJCXX)
endif()

add_library(QtLiquidGlass
    src/QtLiquidGlass.cpp
)

if(APPLE)
    target_sources(QtLiquidGlass PRIVATE src/QtLiquidGlass.mm)
    
    find_library(APPKIT_LIBRARY AppKit)
    find_library(FOUNDATION_LIBRARY Foundation)
    
    target_link_libraries(QtLiquidGlass PRIVATE ${APPKIT_LIBRARY} ${FOUNDATION_LIBRARY})
endif()

target_include_directories(QtLiquidGlass PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(QtLiquidGlass PUBLIC Qt6::Core Qt6::Widgets)

add_subdirectory(examples/GlassDemo)

