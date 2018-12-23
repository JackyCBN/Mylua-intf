#include <stdio.h>
#include <iostream>
#include "lauxlib.h"
#include "LuaIntf/LuaState.h"

using namespace LuaIntf;
using namespace std;
int main()
{

	
   lua_State* L = luaL_newstate();


   LuaState state(L);
   LuaState state2 = state;
   LuaState state3(state);

	std::cout << (state2 == state) << (state == L) <<(L==state) << endl;
	std::cout << (state3 != state) << endl;
	std::cout << (state3 != L) << (L != state) << endl;
   lua_close(L);

	printf("Hello World from t1 Main!\n");
	return 0;
}
