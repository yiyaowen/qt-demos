cmake_minimum_required(VERSION 3.10)

project(gobang-analyzer LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions("-D QT_NO_DEBUG_OUTPUT")

add_subdirectory(Gobang.Core.C)
include_directories("Gobang.Core.C/Interface")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
    Core
    Widgets
    Gui
)

add_executable(Main
    Main.cpp
    MainWindow.cpp
    MainWindow.h
    BoardArray.cpp
    BoardArray.h
    RenjuGroup.c
    RenjuGroup.h
    Point/Point.c
    Point/Point.h
    Pair/IntPair.c
    Pair/IntPair.h
    Image.qrc
)

target_link_libraries(Main
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    core
)

