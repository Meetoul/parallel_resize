cmake_minimum_required(VERSION 3.0)
project( Resizer )
set (CMAKE_CXX_STANDARD 11)

set(SOURCE_EXE main.cpp)

include_directories(MultithreadResizer)

add_executable( Resizer ${SOURCE_EXE} )

add_subdirectory(MultithreadResizer)

target_link_libraries( Resizer MultithreadResizer)