cmake_minimum_required(VERSION 3.10)

project(command-line-sql LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
    Core
    Sql
    REQUIRED
)

add_executable(Main
    Main.cpp
)

target_link_libraries(Main
    Qt5::Core
    Qt5::Sql
)
