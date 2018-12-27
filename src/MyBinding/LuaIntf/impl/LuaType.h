
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
{ };

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

template<typename T>
struct LuaIntegerTypeMapping
{
	static void push(lua_State* L, T value)
	{
		lua_pushinteger(L, static_cast<lua_Integer>(value));
	}

	static T get(lua_State* L, int index)
	{
		return  static_cast<T>(luaL_checkinteger(L, index));
	}

	static T opt(lua_State* L, int index, T def)
	{
		return static_cast<T>(luaL_optinteger(L, index, static_cast<lua_Integer>(def)));
	}
};

template<>
struct LuaTypeMapping<signed char>
	:LuaIntegerTypeMapping<signed char>
{
};

template <>
struct LuaTypeMapping <short>
	: LuaIntegerTypeMapping <short> {};

template <>
struct LuaTypeMapping <int>
	: LuaIntegerTypeMapping <int> {};

template <>
struct LuaTypeMapping <long>
	: LuaIntegerTypeMapping <long> {};

#if LUA_VERSION_NUM <= 502

template <typename T>
struct LuaUnsignedTypeMapping
{
	static void push(lua_State* L, T value)
	{
		lua_pushunsigned(L, static_cast<lua_Unsigned>(value));
	}

	static T get(lua_State* L, int index)
	{
		return static_cast<T>(luaL_checkunsigned(L, index));
	}

	static T opt(lua_State* L, int index, T def)
	{
		return static_cast<T>(luaL_optunsigned(L, index, static_cast<lua_Unsigned>(def)));
	}
};

#else

template <typename T>
using LuaUnsignedTypeMapping = LuaIntegerTypeMapping<T>;

#endif

template <>
struct LuaTypeMapping <unsigned char>
	: LuaUnsignedTypeMapping <unsigned char> {};

template <>
struct LuaTypeMapping <unsigned short>
	: LuaUnsignedTypeMapping <unsigned short> {};

template <>
struct LuaTypeMapping <unsigned int>
	: LuaUnsignedTypeMapping <unsigned int> {};

template <>
struct LuaTypeMapping <unsigned long>
	: LuaUnsignedTypeMapping <unsigned long> {};

//---------------------------------------------------------------------------

template <typename T>
struct LuaNumberTypeMapping
{
	static void push(lua_State* L, T value)
	{
		lua_pushnumber(L, static_cast<lua_Number>(value));
	}

	static T get(lua_State* L, int index)
	{
		return static_cast<T>(luaL_checknumber(L, index));
	}

	static T opt(lua_State* L, int index, T def)
	{
		return static_cast<T>(luaL_optnumber(L, index, static_cast<lua_Number>(def)));
	}
};

template <>
struct LuaTypeMapping <float>
	: LuaNumberTypeMapping <float> {};

template <>
struct LuaTypeMapping <double>
	: LuaNumberTypeMapping <double> {};

template <>
struct LuaTypeMapping <long double>
	: LuaNumberTypeMapping <long double> {};

//---------------------------------------------------------------------------

#if LUAINTF_UNSAFE_INT64 && (LUA_VERSION_NUM <= 502 || defined(LUA_32BITS))

template <typename T>
struct LuaUnsafeInt64TypeMapping
{
	static void push(lua_State* L, T value)
	{
		lua_Number f = static_cast<lua_Number>(value);
#if LUAINTF_UNSAFE_INT64_CHECK
		T verify = static_cast<T>(f);
		if (value != verify) {
			luaL_error(L, "unsafe cast from 64-bit int");
		}
#endif
		lua_pushnumber(L, f);
	}

	static T get(lua_State* L, int index)
	{
		return static_cast<T>(luaL_checknumber(L, index));
	}

	static T opt(lua_State* L, int index, T def)
	{
		return lua_isnoneornil(L, index) ? def : static_cast<T>(luaL_checknumber(L, index));
	}
};

template <>
struct LuaTypeMapping <long long>
	: LuaUnsafeInt64TypeMapping <long long> {};

template <>
struct LuaTypeMapping <unsigned long long>
	: LuaUnsafeInt64TypeMapping <unsigned long long> {};

#elif LUA_VERSION_NUM >= 503

template <>
struct LuaTypeMapping <long long>
	: LuaIntegerTypeMapping <long long> {};

template <>
struct LuaTypeMapping <unsigned long long>
	: LuaIntegerTypeMapping <unsigned long long> {};

#endif

template<>
struct LuaTypeMapping<char>
{
	static void push(lua_State* L, char value)
	{
		char str[] = { value, 0 };
		lua_pushstring(L, str);
	}

	static char get(lua_State* L, int index)
	{
		return luaL_checkstring(L, index)[0];
	}

	static char opt(lua_State* L, int index, char def)
	{
		return lua_isnoneornil(L,index)?def: luaL_checkstring(L, index)[0];
	}
};

template<>
struct LuaTypeMapping<const char*>
{
	static void push(lua_State* L, const char* str)
	{
		lua_pushstring(L, str);
	}

	static const char* get(lua_State* L, int index)
	{
		return luaL_checkstring(L, index);
	}

	static const char* opt(lua_State* L, int index, const char* def)
	{
		return luaL_optstring(L, index, def);
	}
};

template <>
struct LuaTypeMapping <char*>
{
	static void push(lua_State* L, const char* str)
	{
		lua_pushstring(L, str);
	}
};


template <>
struct LuaTypeMapping <std::string>
{
	static void push(lua_State* L, const std::string& str)
	{
		lua_pushlstring(L, str.data(), str.length());
	}

	static std::string get(lua_State* L, int index)
	{
		size_t len = 0;
		const char* p = luaL_checklstring(L, index, &len);
		return std::string(p, len);
	}

	static std::string opt(lua_State* L, int index, const std::string& def)
	{
		return lua_isnoneornil(L, index) ? def : get(L, index);
	}
};
//---------------------------------------------------------------------------

#if LUAINTF_STD_WIDE_STRING

template <typename CH>
struct LuaTypeMapping <std::basic_string<CH>>
{
	using WString = std::basic_string<CH>;
	using WStringConvert = std::wstring_convert<std::codecvt_utf8<CH>, CH>;

	static void push(lua_State* L, const WString& str)
	{
		if (str.empty()) {
			lua_pushliteral(L, "");
		}
		else {
			WStringConvert conv;
			std::string buf = conv.to_bytes(str);
			lua_pushlstring(L, buf.data(), buf.length());
		}
	}

	static WString get(lua_State* L, int index)
	{
		size_t len;
		const char* p = luaL_checklstring(L, index, &len);
		WStringConvert conv;
		return conv.from_bytes(p, p + len);
	}

	static WString opt(lua_State* L, int index, const WString& def)
	{
		return lua_isnoneornil(L, index) ? def : get(L, index);
	}
};

#endif

//---------------------------------------------------------------------------

/**
 * Transitient string type without copying underlying char values, use with caution.
 * It works like const char* with length field.
 */
struct LuaString
{
	constexpr LuaString()
		: data(nullptr)
		, size(0)
	{}

	LuaString(const std::string& str)
		: data(str.data())
		, size(str.size())
	{}

	LuaString(const char* str)
		: data(str)
		, size(std::strlen(str))
	{}

	LuaString(const char* str, size_t len)
		: data(str)
		, size(len)
	{}

	LuaString(lua_State* L, int index)
	{
		data = luaL_checklstring(L, index, &size);
	}

	explicit operator bool() const
	{
		return data != nullptr;
	}

	const char* data;
	size_t size;
};

template <>
struct LuaTypeMapping <LuaString>
{
	static void push(lua_State* L, const LuaString& str)
	{
		if (str.data) {
			lua_pushlstring(L, str.data, str.size);
		}
		else {
			lua_pushnil(L);
		}
	}

	static LuaString get(lua_State* L, int index)
	{
		return LuaString(L, index);
	}

	static LuaString opt(lua_State* L, int index, const LuaString& def)
	{
		return lua_isnoneornil(L, index) ? def : LuaString(L, index);
	}
};

//---------------------------------------------------------------------------

/**
 * Default type mapping to catch all enum conversion
 */
#if LUA_VERSION_NUM <= 502

template <typename T>
struct LuaTypeMapping <T, typename std::enable_if<std::is_enum<T>::value>::type>
	: std::conditional<
	std::is_unsigned<typename std::underlying_type<T>::type>::value,
	LuaUnsignedTypeMapping<T>,
	LuaIntegerTypeMapping<T>
	>::type {};

#else

template <typename T>
struct LuaTypeMapping <T, typename std::enable_if<std::is_enum<T>::value>::type>
	: LuaIntegerTypeMapping<T> {};
#endif

//---------------------------------------------------------------------------

/**
 * Template for list container type
 */
#define LUA_USING_LIST_TYPE_X(LIST, ...) \
    template <__VA_ARGS__> \
    struct LuaTypeMapping <LIST> \
    { \
        static void push(lua_State* L, const LIST& v) \
        { \
            if (v.empty()) { \
                lua_newtable(L); \
            } else { \
                Lua::pushList(L, v); \
            } \
        } \
    \
        static LIST get(lua_State* L, int index) \
        { \
            return lua_isnoneornil(L, index) ? LIST() : Lua::getList<LIST>(L, index); \
        } \
    \
        static LIST opt(lua_State* L, int index, const LIST& def) \
        { \
            return lua_isnoneornil(L, index) ? def : Lua::getList<LIST>(L, index); \
        } \
    };

#define LUA_USING_LIST_TYPE(LIST) \
    LUA_USING_LIST_TYPE_X(LIST<T>, typename T)

 //---------------------------------------------------------------------------

 /**
  * Template for map container type
  */
#define LUA_USING_MAP_TYPE_X(MAP, ...) \
    template <__VA_ARGS__> \
    struct LuaTypeMapping <MAP> \
    { \
        static void push(lua_State* L, const MAP& v) \
        { \
            if (v.empty()) { \
                lua_newtable(L); \
            } else { \
                Lua::pushMap(L, v); \
            } \
        } \
    \
        static MAP get(lua_State* L, int index) \
        { \
            return lua_isnoneornil(L, index) ? MAP() : Lua::getMap<MAP>(L, index); \
        } \
    \
        static MAP opt(lua_State* L, int index, const MAP& def) \
        { \
            return lua_isnoneornil(L, index) ? def : Lua::getMap<MAP>(L, index); \
        } \
    };

#define LUA_COMMA ,

#define LUA_USING_MAP_TYPE(MAP) \
    LUA_USING_MAP_TYPE_X(MAP<K LUA_COMMA V>, typename K, typename V)