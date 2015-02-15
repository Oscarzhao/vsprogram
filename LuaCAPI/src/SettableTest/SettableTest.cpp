/**********************************************************
**  功能：使用 lua_settable 将 信息写入 table 中
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


// 在 C 中定义一个颜色表
struct ColorTable {
    char * name;
    unsigned char red, green, blue;
} colortable[]  = {
    { "WHITE", MAX_COLOR, MAX_COLOR, MAX_COLOR },
    { "RED",   MAX_COLOR, 0,         0         },
    { "GREEN", 0,         MAX_COLOR, 0         },
    { "BLUE",  0,         0,         MAX_COLOR },
    { "BLACK", 0,         0,         0         },
    { NULL,    0,         0,         0         }

};

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
            case LUA_TTABLE:
                std::cout << "table";
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

/* assume that table is on the stack top*/
int getfield(lua_State * L, const char * key)
{
    int result;
    lua_pushstring(L, key);
    lua_gettable(L, -2);    // get background[key]
    if (!lua_isnumber(L, -1))
        error(L, "invalid component in background color");
    result = (int)(lua_tonumber(L, -1) * MAX_COLOR);
    lua_pop(L, 1);   // remove number
    return result;
}

// lua_settable以table在栈中的索引作为参数，并将栈中的key和value出栈，用这两个值修改table
// Setfield函数假定调用之前table是在栈顶位置(索引为-1)
// 将index和value入栈之后，table索引变为-3
void setfield(lua_State* L, const char * index, int value)
{
    lua_pushstring(L, index);
    lua_pushnumber(L, (double)value/MAX_COLOR);
    lua_settable(L, -3);
}

void setcolor (lua_State* L, struct ColorTable * ct)
{
    lua_newtable(L);
    setfield(L, "r", ct->red);  // table.r = colortable[i].red
    setfield(L, "g", ct->green); // table.g = colortable[i].green
    setfield(L, "b", ct->blue);  // table.b = colortable[i].blue
    stackDump(L);
    lua_setglobal(L, ct->name);  // 'name' = table
}
int main()
{
    lua_State* L = lua_open();

    int i = 0;
    while(colortable[i].name != NULL)
    {
        setcolor(L, &colortable[i]);
        i++;
    }
    // print nothing here, because
    // lua_setglobal pop out the table
    stackDump(L);

    system("pause");
    return 0;
}