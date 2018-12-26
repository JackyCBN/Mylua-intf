
enum class LuaTypeID
{
	NONE = LUA_TNONE,

	NIL = LUA_TNIL,
	BOOLEAN = LUA_TBOOLEAN,
	LIGHTUSERDATA = LUA_TLIGHTUSERDATA,
	NUMBER = LUA_TNUMBER,
	STRING = LUA_TSTRING,
	TABLE = LUA_TTABLE,
	FUNCTION = LUA_TFUNCTION,
	USERDATA = LUA_TUSERDATA,
	THREAD = LUA_TTHREAD
};

template<typename T, typename  ENABLE = void>
struct LuaTypeMapping;

template <typename T, bool IS_CONST, bool IS_REF>
struct LuaClassMapping;


template <typename T>
using is_specialized = std::is_default_constructible<T>;

template <typename T>
constexpr bool is_specialized_v = is_specialized<T>::value;


template <typename T>
constexpr bool   LuaTypeMappingExists = is_specialized_v<LuaTypeMapping<T>>;

template <typename T>
struct LuaType
	: std::conditional_t<
		std::is_class_v<std::decay_t<T>>
		&& !LuaTypeMappingExists<std::decay_t<T>>,
			LuaClassMapping<std::decay_t<T>, std::is_const_v<std::remove_reference_t<T>>, std::is_reference_v<T>>, 
			LuaTypeMapping<std::decay_t<T>> >
{
	
};
//template<>
//struct LuaTypeMapping<bool>
//{
//	static void push(lua_State* L, bool value)
//	{
//		
//	}
//
//	static bool get(lua_State* L, int index)
//	{
//
//	}
//
//	static bool opt(lua_State* L, int index, bool def)
//	{
//		
//	}
//};

template<>
struct LuaTypeMapping<bool>
{
	static void push(lua_State* L, bool value)
	{
		lua_pushboolean(L, value);
	}

	static bool get(lua_State* L, int index)
	{
		return lua_toboolean(L, index);
	}

	static bool opt(lua_State* L, int index, bool def)
	{
		return lua_isnone(L, index) ? def : lua_toboolean(L, index);
	}
};