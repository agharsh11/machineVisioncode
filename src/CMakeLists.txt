cmake_minimum_required(VERSION 2.8)
project( mainr )

include_directories( ../include )

file(GLOB SOURCES "./*.cpp")

find_package( OpenCV REQUIRED )

add_executable( mainr ${SOURCES} )

target_link_libraries( mainr ${OpenCV_LIBS} )
