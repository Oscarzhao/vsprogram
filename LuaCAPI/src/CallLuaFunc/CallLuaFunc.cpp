/**********************************************************
**  功能：调用 Lua 函数
**  使用API调用函数的方法是很简单的：
    **  首先，将被调用的函数入栈；
    **  第二，依次将所有参数入栈；
    **  第三，使用lua_pcall调用函数；最后，从栈中获取函数执行返回的结果。
**********************************************************/

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};

// 注意： windowSize.lua 的编码是 utf-8 会报错
#include <iostream>
#include <string>
#define MAX_COLOR   255

// 打印 堆栈
static void stackDump(lua_State* L)
{
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++)
    {
        int t = lua_type(L, i);
        switch(t){
            case LUA_TSTRING:
                std::cout << "'" << lua_tostring(L, i) << "'";
                break;
            case LUA_TBOOLEAN:
                std::cout << lua_toboolean(L, i)? "true": "false";
                break;
            case LUA_TNUMBER:
                std::cout << lua_tonumber(L, i);
                break;
            default:
                std::cout << lua_typename(L, t);
                break;
        }

        std::cout << "    "; // put a separator
    }
    std::cout << std::endl;
}

void error(lua_State* L, const char* fmt, ...)
{
    va_list argp;
    va_start(argp, fmt);
    vfprintf(stderr, fmt, argp);
    va_end(argp);
    lua_close(L);
    std::cout << std::endl;
    system("pause");
    exit(EXIT_FAILURE);
}

double func(lua_State * L, double x, double y)
{
    double z;
    stackDump(L);

    // push functions and arguments
    lua_getglobal(L, "var1");
    stackDump(L);
    lua_getglobal(L, "funcTest");   // function to be called
    stackDump(L);
    lua_pushnumber(L, x);    // push 1st argument
    lua_pushnumber(L, y);    // push 2nd argument

    stackDump(L);
    // do the call ( 2 arguments, 1 result)
    if (lua_pcall(L, 2, 1, 0))
    {
        error(L, "error running function 'funcTest': %s", lua_tostring(L, -1));
    }

    // retrieve result
    if (!lua_isnumber(L, -1))
        error(L, "function 'funcTest' must return a number");
    z = lua_tonumber(L, -1);
    lua_pop(L, 1);   // pop returned value
    return z;
}

// 字符串截取函数
// split("hi,,there", ",")
static int l_split(lua_State *L)
{
    const char * s = luaL_checkstring(L, 1);
    const char * sep = luaL_checkstring(L, 2);
    const char * e;
    int i = 1;

    lua_newtable(L);   // result
    
    // repeat for each separator
    while((e = strchr(s, *sep)) != NULL)
    {
        lua_pushlstring(L, s, e-s);  // push substring
        lua_rawseti(L, -2, i++);   
        s = e + 1;
    }
}
int main()
{
    lua_State* L = lua_open();
    luaL_openlibs(L);
    const char * filename = "../../src/CallLuaFunc/func.lua";
    if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
        error(L, "cannot run configuration file: %s", lua_tostring(L, -1));

    double result = func(L, 2, 1);
    
    std::cout << "func(1, 0) = " << result << std::endl;

    lua_close(L);
    system("pause");
    return 0;
}