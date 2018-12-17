cmake_minimum_required(VERSION 3.13)

PROJECT(MyLuaIntf)
include_directories(${CMAKE_SOURCE_DIR}/3rd/lua)
include_directories(${CMAKE_SOURCE_DIR}/3rd/LuaIntf)

file(GLOB_RECURSE CRCS *.c)
file(GLOB_RECURSE SRCS *.cpp)
file(GLOB_RECURSE HDRS *.h)

ADD_EXECUTABLE(MyLuaIntf src/main.c ${CRCS} ${SRCS} ${HDRS})