cmake_minimum_required(VERSION 3.13)

PROJECT(MyLuaIntf)
include_directories(${CMAKE_SOURCE_DIR}/3rd/lua)
include_directories(${CMAKE_SOURCE_DIR}/3rd/LuaIntf)
ADD_SUBDIRECTORY(src)