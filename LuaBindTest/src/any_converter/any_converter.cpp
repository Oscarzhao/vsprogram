// Copyright (c) 2005 Daniel Wallin and Arvid Norberg

/*
* 这个程序生成的 dll 不能使用 doc 里的代码进行测试
* 需要额外 改变 luaconf.h 文件，并且重新编译luabind
*/
#include <iostream>
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

bool dostring(lua_State* L, const char* str)
{
    if (luaL_loadbuffer(L, str, std::strlen(str), str) || lua_pcall(L, 0, 0, 0))
    {
        const char* a = lua_tostring(L, -1);
        std::cout << a << "\n";
        lua_pop(L, 1);
        return true;
    }
}

#include <luabind/luabind.hpp>
#include <luabind/detail/convert_to_lua.hpp>
#include <boost/any.hpp>

template<class T>
struct convert_any
{
    static void convert(lua_State* L, const boost::any& a)
    {
        luabind::detail::convert_to_lua(L, *boost::any_cast<T>(&a));
    }
};

std::map<const std::type_info*, void(*)(lua_State*, const boost::any&)> any_converters;

template<class T>
void register_any_converter()
{
    any_converters[&typeid(T)] = convert_any<T>::convert;
}

namespace luabind
{
    namespace converters
    {
        yes_t is_user_defined(by_value<boost::any>);
        yes_t is_user_defined(by_const_reference<boost::any>);

        void convert_cpp_to_lua(lua_State* L, const boost::any&);
        conv_t conv = any_converters[&a.type()];
        conv(L, a);
    }
}