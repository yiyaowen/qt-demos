cmake_minimum_required(VERSION 3.10)

project(Qt_viewer LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)

add_executable(Viewer
    Main.cpp
    Viewer.cpp
    Viewer.qrc
)

target_link_libraries(Viewer
    Qt::Core
    Qt::Widgets
    Qt::Gui
)
