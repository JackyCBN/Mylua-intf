project ( "lua53" )
cmake_minimum_required ( VERSION 3.0 )

include_directories ( src ${CMAKE_CURRENT_BINARY_DIR} )
set ( SRC_CORE src/lapi.c src/lcode.c src/lctype.c src/ldebug.c src/ldo.c src/ldump.c src/lfunc.c src/lgc.c src/llex.c
  src/lmem.c src/lobject.c src/lopcodes.c src/lparser.c src/lstate.c src/lstring.c src/ltable.c
  src/ltm.c src/lundump.c src/lvm.c src/lzio.c )
set ( SRC_LIB src/lauxlib.c src/lbaselib.c src/lbitlib.c src/lcorolib.c src/ldblib.c src/liolib.c
  src/lmathlib.c src/loslib.c src/lstrlib.c src/ltablib.c src/lutf8lib.c src/loadlib.c src/linit.c )

set ( SRC_LUA src/lua.c )
set ( SRC_LUAC src/luac.c )

# compile with C++ compiler
set_source_files_properties(${SRC_CORE} ${SRC_LIB} ${SRC_LUA} ${SRC_LUAC} PROPERTIES LANGUAGE CXX)

add_library ( liblua ${SRC_CORE} ${SRC_LIB} )
set_target_properties ( liblua PROPERTIES OUTPUT_NAME lua53-static )
set_target_properties ( liblua PROPERTIES FOLDER "lua53" )


add_library ( libluadll SHARED ${SRC_CORE} ${SRC_LIB} )
target_compile_definitions ( libluadll PRIVATE _CRT_SECURE_NO_WARNINGS LUA_BUILD_AS_DLL )
set_target_properties ( libluadll PROPERTIES OUTPUT_NAME lua53 )
set_target_properties ( libluadll PROPERTIES FOLDER "lua53" )

add_executable ( lua ${SRC_LUA} )
target_link_libraries ( lua libluadll )
set_target_properties ( lua PROPERTIES FOLDER "lua53" )

add_executable ( luac ${SRC_LUAC} )
target_link_libraries ( luac liblua )
set_target_properties ( luac PROPERTIES FOLDER "lua53" )