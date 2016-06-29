cmake_minimum_required(VERSION 2.8.9)
project(kalsim)


set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(PkgConfig REQUIRED)
pkg_search_module(CAIRO REQUIRED cairo)

include_directories(include)

#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.cxx")

add_executable(kalsim ${SOURCES})

target_link_libraries(kalsim ${CAIRO_LIBRARIES})
target_include_directories(kalsim PUBLIC ${CAIRO_INCLUDE_DIRS})
target_compile_options(kalsim PUBLIC ${CAIRO_CFLAGS_OTHER})

