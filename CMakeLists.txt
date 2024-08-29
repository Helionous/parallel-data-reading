cmake_minimum_required(VERSION 3.29)
project(app)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(TBB REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

include_directories(src/business src/forms src/helper src/object src/run)

add_executable(app
        src/run/Main.cpp
        src/forms/Frame.cpp
        src/forms/Frame.h
        src/business/SearchParallelism.cpp
        src/business/SearchParallelism.h
        src/business/SearchRamMemory.cpp
        src/business/SearchSimple.cpp
        src/object/Person.cpp
        src/helper/FileReader.h
        src/helper/FileReader.cpp
        src/object/Person.h
)

target_link_libraries(app
        Qt::Core
        Qt6::Widgets
        TBB::tbb
)
