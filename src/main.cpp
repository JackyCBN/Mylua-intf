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
int main(int argc, char* argv[])
{
   lua_State* L = luaL_newstate();

   // test LuaState
   //LuaState state(L);
   //LuaState state2 = state;
   //LuaState state3(state);

	//std::cout << (state2 == state) << (state == L) <<(L==state) << endl;
	//std::cout << (state3 != state) << endl;
	//std::cout << (state3 != L) << (L != state) << endl;

   luaL_openlibs(L);
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
