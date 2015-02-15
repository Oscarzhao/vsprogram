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
#include <cmath>
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

static int l_sin(lua_State* L)
{
    //if (lua_isnumber(L, -1))
    //{
    //    double d = lua_tonumber(L, 1);  // get argument
    //    lua_pushnumber(L, sin(d));  // push result
    //    return 1;    // number of results
    //}
    //else
    //{
    //    return 0;
    //}

    // 或者 
    double d = luaL_checknumber(L, 1);
    lua_pushnumber(L, sin(d));
    return 1;  // number of results
}

// 实现 map 函数:  以数组的每一个元素为参数调用一个指定的函数
//                  并将数组的该元素替换为调用函数返回的结果
// p204
int l_map(lua_State * L)
{
    int i, n;
    // 1st argument must be a table
    luaL_checktype(L, 1, LUA_TTABLE);

    // 2nd argument must be a function (f)
    luaL_checktype(L, 2, LUA_TFUNCTION);

    n = luaL_getn(L, 1); // get size of the table

    for (int i = 1; i < n; i++)
    {
        lua_pushvalue(L, 2);   // push f
        lua_rawgeti(L, 1, i);  // push t[i]
        lua_call(L, 1, 1); // t[i] = result
        lua_rawseti(L, 1, i);
    }

    return 0;
}


/* assume that table is on the stack top*/
int getfield(lua_State * L, const char * key)
{
    int result;
    lua_pushstring(L, key);
    lua_gettable(L, -2);    // get background[key]
    if (!lua_isnumber(L, -1))
        error(L, "invalid component in background color");
    result = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);   // remove number
    return result;
}
// 打印出 table 的内容
// 默认 table 在栈顶
void printTable(lua_State * L)
{
    std::cout << "  red: " << getfield(L, "red")
        << "\ngreen: " << getfield(L, "green")
        << "\nblue : " << getfield(L, "blue");
}

// 测试用例：l_map 测试
void mapTest()
{
    lua_State * L = lua_open();
    luaL_openlibs(L);
    lua_newtable(L);

    lua_pushstring(L, "red");
    lua_pushnumber(L, 100);
    lua_settable(L, -3);

    lua_pushstring(L, "green");
    lua_pushnumber(L, 99);
    lua_settable(L, -3);

    lua_pushstring(L, "blue");
    lua_pushnumber(L, 98);
    lua_settable(L, -3);

    stackDump(L);
    printTable(L);
}

int main()
{
    //lua_State* L = lua_open();
    //luaL_openlibs(L);

    //// 入栈 类型 为function 的值
    //lua_pushcfunction(L, l_sin);
    //// 将 function 赋值给全局变量 mysin
    //// 重新编译Lua，可以在 Lua程序中使用 新的 mysin 函数
    //lua_setglobal(L, "mysin");

    //lua_close(L);

    mapTest();
    system("pause");
    return 0;
}