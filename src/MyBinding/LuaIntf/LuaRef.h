//
// https://github.com/SteveKChiu/lua-intf
//
// Copyright 2013, Steve Chiu <steve.k.chiu@gmail.com>
//
// The MIT License (http://www.opensource.org/licenses/mit-license.php)
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#ifndef LUAREF_H
#define LUAREF_H

//---------------------------------------------------------------------------

#include "LuaState.h"
#include <functional>
#include <tuple>
#include <memory>

namespace LuaIntf
{

	class LuaRef;

	//---------------------------------------------------------------------------

	/**
	 * Assignable and convertible result of bracket-style lookup.
	 */
	class LuaTableRef
	{
	
	};

	//---------------------------------------------------------------------------

	/**
	 * Lightweight reference to a Lua object.
	 *
	 * The reference is maintained for the lifetime of the C++ object.
	 */
	class LuaRef
	{
	public:
		static LuaRef popFromStack(lua_State* L)
		{
			return LuaRef(L);
		}

		static LuaRef globals(lua_State* L)
		{
			lua_pushglobaltable(L);
			return popFromStack(L);
		}

		static LuaRef registry(lua_State* L)
		{
			return LuaRef(L, LUA_REGISTRYINDEX);
		}

		constexpr LuaRef()
			:L(nullptr),
			m_ref(LUA_NOREF)
		{
			
		}

		LuaRef(lua_State* state, nullptr_t)
			:L(state),
			m_ref(LUA_REFNIL)
		{
			assert(L);
		}

		LuaRef(lua_State* state, int index)
			:L(state)
		{
			assert(L);
			lua_pushvalue(L, index);
			m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
		}

		LuaRef(lua_State* state, const char* name)
			:L(state)
		{
			assert(L);
			Lua::pushGlobal(L, name);
			m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
		}

		LuaRef(const LuaRef& that);

		LuaRef(lua_State* state)
			:L(state)
		{
			m_ref = luaL_ref(L, LUA_REGISTRYINDEX);
		}

		LuaRef(LuaRef&&  that)
			:L(that.L)
		{
			m_ref = that.m_ref;
			that.m_ref = LUA_NOREF;
		}

		~LuaRef()
		{
			if(L)
			{
				luaL_unref(L, LUA_REGISTRYINDEX, m_ref);
			}
		}

		LuaRef& operator = (const LuaRef& that);

		LuaRef& operator = (LuaRef&& that)
		{
			std::swap(L, that.L);
			std::swap(m_ref, that.m_ref);
			return *this;
		}

		LuaRef& operator = (std::nullptr_t)
		{
			
		}

	private:
		lua_State* L;
		int m_ref;

	};


#if LUAINTF_HEADERS_ONLY
#include "src/LuaRef.cpp"
#endif

	//---------------------------------------------------------------------------

}

#endif
