cmake_minimum_required(VERSION 3.17)
project(proj3a)

find_package(Boost 1.50 REQUIRED COMPONENTS filesystem)
message(STATUS "Boost version: ${Boost_VERSION}")

add_subdirectory(test)

#include_directories(test)

set(CMAKE_CXX_STANDARD 20)


include_directories(src)
include_directories(src/PGA)

add_executable(proj3a
        src/PGA/image_lib.h
        src/PGA/multivector.h
        src/PGA/PGA_3D.h
        src/PGA/stb_image.h
        src/PGA/stb_image_write.h
        src/FileReader.cpp
        src/FileReader.h
        src/commands.h
        src/InputData.h src/main.cpp src/Scene.cpp src/Scene.h)


target_link_libraries(proj3a PUBLIC Boost::filesystem)

