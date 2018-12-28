#ifndef LUAINTF_H
#include "../LuaIntf.h"
using namespace LuaIntf;
#endif

LUA_INLINE LuaRef::LuaRef(const LuaRef& that)
	: L(that.L)
{
	if(L)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, that.m_ref);
		m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
	}
	else
	{
		m_ref = LUA_NOREF;
	}
}

LUA_INLINE LuaRef& LuaRef::operator = (const LuaRef& that)
{
	if(L)
	{
		luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
	}

	L = that.L;
	if (L)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, that.m_ref);
		m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
	}
	else
	{
		m_ref = LUA_NOREF;
	}
}