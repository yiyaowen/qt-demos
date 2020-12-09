cmake_minimum_required(VERSION 3.10)

project(shape-widget VERSION 1.0 LANGUAGES CXX)

add_definitions(-D DEBUG)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

add_executable(Main
    Main.cpp
    ShapeWidget.cpp
)

target_link_libraries(Main
    Qt::Core
    Qt::Widgets
)
