cmake_minimum_required(VERSION 3.29)
project(lab4_sem3)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Charts)

add_executable(lab4_sem3 main.cpp
        DirectedWindow.cpp
        DirectedWindow.h
        UndirectedWindow.cpp
        UndirectedWindow.h
        MainWindow.cpp
        MainWindow.h
        GraphTypes.h
        HasseWindow.cpp
        HasseWindow.h
)
target_link_libraries(lab4_sem3
        PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Charts
)
add_subdirectory(Google_tests)
include_directories(
        include
)
