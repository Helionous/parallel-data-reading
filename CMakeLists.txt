cmake_minimum_required(VERSION 3.29)
project(app)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

include_directories(forms)

add_executable(app
        forms/form.cpp
        run/main.cpp
)
target_link_libraries(app
        Qt::Core
        Qt6::Widgets
)

