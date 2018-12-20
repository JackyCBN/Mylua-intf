cmake_minimum_required ( VERSION 3.0 )
PROJECT("MyLuaIntf")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# cmake helper functions
include(${CMAKE_SOURCE_DIR}/utility.cmake)

include_directories(${CMAKE_SOURCE_DIR}/src/3rd/lua53/src)
include_directories(${CMAKE_SOURCE_DIR}/src/3rd)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/3rd/lua53/ Bin)
# files
FILE(GLOB_RECURSE ide_source_list
    "${CMAKE_SOURCE_DIR}/src/3rd/LuaIntf/*.c"
	"${CMAKE_SOURCE_DIR}/src/3rd/LuaIntf/*.cpp"
	"${CMAKE_SOURCE_DIR}/src/3rd/LuaIntf/*.h"
	"${CMAKE_SOURCE_DIR}/src/3rd/LuaIntf/*.cxx"
	"${CMAKE_SOURCE_DIR}/src/3rd/LuaIntf/*.hpp"
	"${CMAKE_SOURCE_DIR}/src/main.cpp")

# library
createSrcGroups(ide_source_list )

ADD_EXECUTABLE(MyLuaIntf ${ide_source_list})
target_link_libraries(MyLuaIntf libluadll)
set_target_properties(MyLuaIntf PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Bin")
set_target_properties(MyLuaIntf PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/Bin")

#target_link_libraries(MyLuaIntf PUBLIC ${SANDBOX_3RD_DIR}/curl/prebuilt/win/x64/debug/libcurl_debug.lib)
#target_link_libraries(MyLuaIntf optimized ${SANDBOX_3RD_DIR}/curl/prebuilt/win/x64/release/libcurl_release.lib)
#target_link_libraries(MyLuaIntf ${SANDBOX_DIR}/../platform/Assets/Plugins/Win64/lua53.lib)
#target_link_libraries(MyLuaIntf ${SANDBOX_DIR}/../platform/Assets/Plugins/Win64/lua53.lib)

if(MSVC)
	target_compile_options(MyLuaIntf PRIVATE "/Zc:__cplusplus")
endif(MSVC)