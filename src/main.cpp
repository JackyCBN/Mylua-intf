#include <stdio.h>
#include <iostream>
#include "lauxlib.h"
#include "LuaIntf/LuaState.h"

using namespace LuaIntf;
using namespace std;

enum ETest
{
	a,
	b
};

struct test
{
	int a;
	int b;
};
static void* dummy = 0;

int release_object(lua_State* L)
{
	void* p = lua_touserdata(L, -1);
	return 0;
}

void testGc(lua_State* L)
{
	luaL_openlibs(L);
	int count = 0;
	int refIndex = -1;
	int stackCount = 0;
	int type = -1;
	int vv = -1;


	stackCount = lua_gettop(L);
	lua_pushlightuserdata(L, &dummy);
	stackCount = lua_gettop(L);
	lua_newtable(L);
	lua_createtable(L, 0, 1);
	lua_pushliteral(L, "v");
	lua_setfield(L, -2, "__mode");
	lua_setmetatable(L, -2);
	stackCount = lua_gettop(L);
	lua_settable(L, LUA_REGISTRYINDEX);
	stackCount = lua_gettop(L);

	//void* newP = new test();
	//lua_pushlightuserdata(L, newP);
	//lua_pushlightuserdata(L, newP);

	 // get reftable
	stackCount = lua_gettop(L);
	lua_pushlightuserdata(L, &dummy);
	lua_gettable(L, LUA_REGISTRYINDEX);
	stackCount = lua_gettop(L);

	void* p = lua_newuserdata(L, sizeof(test));
	test* pt = static_cast<test*>(p);
	pt->a = 2;
	pt->b = 3;
	stackCount = lua_gettop(L);
	lua_createtable(L, 0, 1);
	lua_pushcfunction(L, release_object);
	lua_setfield(L, -2, "__gc");
	lua_setmetatable(L, -2);
	stackCount = lua_gettop(L);

	//lua_rawsetp(L, -2, p);
	lua_rawsetp(L, -2, nullptr);
	stackCount = lua_gettop(L);

	//lua_rawgetp(L, -1, p);
	lua_rawgetp(L, -1, nullptr);
	stackCount = lua_gettop(L);
	bool b = lua_isnil(L, -1);
	b = lua_isuserdata(L, -1);

	lua_setglobal(L, "MyTest");
	stackCount = lua_gettop(L);
	lua_pop(L, 1);
	stackCount = lua_gettop(L);
	//lua_rawgetp(L, -2, p);
	//stackCount = lua_gettop(L);

	//int ret = lua_compare(L, -1, -2, LUA_OPEQ);

	//void* p2 = lua_newuserdata(L, sizeof(test));
	//test* pt2 = static_cast<test*>(p);
	//pt2->a = 2;
	//pt2->b = 3;
}


int main(int argc, char* argv[])
{
   lua_State* L = luaL_newstate();

   luaL_openlibs(L);
   //testGc(L);
   int count = 0;
   int test = 0;
   count = lua_gettop(L);
   test = lua_rawlen(L, LUA_REGISTRYINDEX);
   count = lua_gettop(L);
   lua_rawgeti(L, LUA_REGISTRYINDEX, 2);
   count = lua_gettop(L);
   lua_pushglobaltable(L);
   //int a = lua_compare(L, -2, -1, LUA_OPEQ);
   //lua_pushvalue(L, LUA_REGISTRYINDEX);
   int ref = luaL_ref(L, LUA_REGISTRYINDEX);

   lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
   //int a = lua_compare(L, -2, -1, LUA_OPEQ);
   //lua_pushvalue(L, LUA_REGISTRYINDEX);
   ref = luaL_ref(L, LUA_REGISTRYINDEX);
 //  lua_pushvalue(L, LUA_REGISTRYINDEX);
 //  int a = lua_compare(L, -3, -1, LUA_OPEQ);
	//a = lua_compare(L, -3, -2, LUA_OPEQ);
	
	// load file
	if(argc>1)
	{
		//R"(G:\Code\lua\Mylua-intf\res\main.lua)"
		std::cout << "load lua file:" << argv[1] << std::endl;
		luaL_dofile(L, argv[1]);
		Lua::push(L, ETest::a);

		std::cout << typeid(std::enable_if_t<std::is_enum_v<ETest>, int>).name() << std::endl;
	}
	else
	{
		std::cout << "no lua file to load" << std::endl;
	}


   lua_close(L);

	//printf("Hello World from t1 Main!\n");
	return 0;
}
