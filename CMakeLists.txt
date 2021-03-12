cmake_minimum_required(VERSION 3.17)
project(proj3a)

find_package(Boost 1.50 REQUIRED COMPONENTS filesystem)
message(STATUS "Boost version: ${Boost_VERSION}")

find_package(Catch2 REQUIRED)

set(CMAKE_CXX_STANDARD 14)

#include_directories(src)
#include_directories(src/PGA)

add_executable(proj3a
        src/PGA/image_lib.h
        src/PGA/multivector.h
        src/PGA/parse_pga.h
        src/PGA/PGA_3D.h
        src/PGA/rayTrace_pga.cpp
        src/PGA/stb_image.h
        src/PGA/stb_image_write.h
        src/FileReader.cpp
        src/FileReader.h
        src/commands.h
        src/InputData.h)


target_link_libraries(proj3a PUBLIC Boost::filesystem)
target_link_libraries(proj3a PRIVATE Catch2::Catch2)
