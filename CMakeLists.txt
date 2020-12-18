cmake_minimum_required(VERSION 3.10)

project(multithreading-basic-demo LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS 
    Core
    Widgets
    Gui
)

add_executable(Main
    Main.cpp
    ThreadDialog.cpp
    ThreadDialog.h
    WorkThread.cpp
    WorkThread.h
)

target_link_libraries(Main
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
)
