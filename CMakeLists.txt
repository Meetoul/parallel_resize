cmake_minimum_required(VERSION 3.0)
project( Resizer )
set (CMAKE_CXX_STANDARD 11)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable( Resizer main.cpp )
target_link_libraries( Resizer ${OpenCV_LIBS} )
target_link_libraries( Resizer Threads::Threads)