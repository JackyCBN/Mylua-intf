cmake_minimum_required(VERSION 3.10)

PROJECT(MyLuaIntf)
# cmake helper functions
include(${CMAKE_SOURCE_DIR}/utility.cmake)

include_directories(${CMAKE_SOURCE_DIR}/src/3rd/lua/src)
include_directories(${CMAKE_SOURCE_DIR}/src/3rd)

# files
FILE(GLOB_RECURSE ide_source_list
    "${CMAKE_SOURCE_DIR}/src/*.c"
	"${CMAKE_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_SOURCE_DIR}/src/*.h"
	"${CMAKE_SOURCE_DIR}/src/*.cxx"
	"${CMAKE_SOURCE_DIR}/src/*.hpp")

# library
createSrcGroups(ide_source_list)

ADD_EXECUTABLE(MyLuaIntf ${ide_source_list})