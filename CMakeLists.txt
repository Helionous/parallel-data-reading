cmake_minimum_required(VERSION 3.29)
project(app)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core Widgets Concurrent REQUIRED)

include_directories(src/business src/forms src/helper src/object src/run)

add_executable(app
        src/run/Main.cpp
        src/forms/Frame.h
        src/forms/Frame.cpp
        src/business/SearchParallelism.h
        src/business/SearchParallelism.cpp
        src/business/SearchRamMemory.h
        src/business/SearchRamMemory.cpp
        src/business/SearchSimple.cpp
        src/business/SearchSimple.h
        src/helper/FileReaderRam.h
        src/helper/FileReaderRam.cpp
        src/helper/DetermineTime.h
        src/helper/DetermineTime.cpp
        src/helper/ParallelFileReader.h
        src/helper/ParallelFileReader.cpp
        src/object/Person.h
        src/object/Person.cpp
        src/object/GlobalVar.h
        src/object/GlobalVar.cpp
)

target_link_libraries(app
        Qt::Core
        Qt6::Widgets
        Qt6::Concurrent
)
