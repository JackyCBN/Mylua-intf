
PROJECT(MyLuaIntf)
cmake_minimum_required ( VERSION 3.0 )

# cmake helper functions
include(${CMAKE_SOURCE_DIR}/utility.cmake)

include_directories(${CMAKE_SOURCE_DIR}/src/3rd/lua53/src)
include_directories(${CMAKE_SOURCE_DIR}/src/3rd)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/3rd/lua53/)
# files
FILE(GLOB_RECURSE ide_source_list
    "${CMAKE_SOURCE_DIR}/src/3rd/LuaIntf/*.c"
	"${CMAKE_SOURCE_DIR}/src/3rd/LuaIntf/*.cpp"
	"${CMAKE_SOURCE_DIR}/src/3rd/LuaIntf/*.h"
	"${CMAKE_SOURCE_DIR}/src/3rd/LuaIntf/*.cxx"
	"${CMAKE_SOURCE_DIR}/src/3rd/LuaIntf/*.hpp")

# library
createSrcGroups(ide_source_list)

ADD_EXECUTABLE(MyLuaIntf ${ide_source_list})