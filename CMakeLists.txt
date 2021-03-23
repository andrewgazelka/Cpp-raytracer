cmake_minimum_required(VERSION 3.17)
project(project3)

find_package(Boost 1.50 REQUIRED COMPONENTS filesystem)
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
message(STATUS "Boost version: ${Boost_VERSION}")

add_subdirectory(test)

#include_directories(test)

set(CMAKE_CXX_STANDARD 20)


include_directories(src)
include_directories(src/PGA)

add_executable(project3
        src/PGA/image_lib.h
        src/PGA/multivector.h
        src/PGA/PGA_3D.h
        src/PGA/stb_image.h
        src/PGA/stb_image_write.h
        src/FileReader.cpp
        src/FileReader.h
        src/commands.h
        src/InputData.h src/main.cpp src/Scene.cpp src/Scene.h src/utils.h src/collision.h src/primitives/Triangle.cpp src/primitives/Triangle.h src/primitives/PrimitiveProcessor.cpp src/primitives/PrimitiveProcessor.h)


target_link_libraries(project3 PUBLIC Boost::filesystem)

