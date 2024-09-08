cmake_minimum_required(VERSION 3.10.2)

project( Image )

find_package( ITK REQUIRED )
include( ${ITK_USE_FILE} )

add_executable( Image Image.cpp )
target_link_libraries( Image ${ITK_LIBRARIES} )