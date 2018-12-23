cmake_minimum_required ( VERSION 3.0 )
PROJECT("MyLuaIntf")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# cmake helper functions
include(${CMAKE_SOURCE_DIR}/utility.cmake)

include_directories(${CMAKE_SOURCE_DIR}/src/3rd/lua53/src)
include_directories(${CMAKE_SOURCE_DIR}/src/MyBinding)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/3rd/lua53/ Bin)
# files
FILE(GLOB_RECURSE ide_source_list
    "${CMAKE_SOURCE_DIR}/src/MyBinding/LuaIntf/*.c"
	"${CMAKE_SOURCE_DIR}/src/MyBinding/LuaIntf/*.cpp"
	"${CMAKE_SOURCE_DIR}/src/MyBinding/LuaIntf/*.h"
	"${CMAKE_SOURCE_DIR}/src/MyBinding/LuaIntf/*.cxx"
	"${CMAKE_SOURCE_DIR}/src/MyBinding/LuaIntf/*.hpp"
	"${CMAKE_SOURCE_DIR}/src/main.cpp")

# library
createSrcGroups(ide_source_list )

ADD_EXECUTABLE(MyLuaIntf ${ide_source_list})
target_link_libraries(MyLuaIntf libluadll)
set_target_properties(MyLuaIntf PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Bin/Debug")
set_target_properties(MyLuaIntf PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/Bin/Release")

if(MSVC)
	target_compile_options(MyLuaIntf PRIVATE "/Zc:__cplusplus")
endif(MSVC)