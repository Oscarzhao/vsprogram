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

void lua_rawgetiTest(const char * filename)
{
    lua_State * L = lua_open();
    luaL_openlibs(L);

    std::cout << "Print: ";
    stackDump(L);
    if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
        error(L, "cannot run configuration file: %s", lua_tostring(L, -1));

    lua_getglobal(L, "list");
    std::cout << "push a table:  ";
    stackDump(L);

    //lua_pushnumber(L, 2);

    lua_rawgeti(L, -1, 1);

    std::cout << "result: ";
    stackDump(L);
    
    lua_pop(L, 1);   // 弹出 上次执行的结果

    lua_close(L);
}
// table在栈中的index
// 栈顶 必须是 key
// 然后才能用 lua_gettable 方法
// 每执行一次 lua_gettable, key 会被弹出，value 被压入
void gettableTest(const char* filename)
{
    lua_State * L = lua_open();
    luaL_openlibs(L);

    if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
        error(L, "cannot run configuration file: %s", lua_tostring(L, -1));

    std::cout << "1. ";
    stackDump(L);
    lua_getglobal(L, "background");
    std::cout << "2. ";
    stackDump(L);
    lua_pushstring(L, "r");
    lua_pushstring(L, "g");
    std::cout << "3. ";
    stackDump(L);
    lua_gettable(L, -3);
    if (lua_isnumber(L, -1))
    {
        int result = (int)(lua_tonumber(L, -1) * MAX_COLOR);
        std::cout << "int: " << result << std::endl;
    }
    else if (lua_isstring(L, -1))
    {
        std::string result = lua_tostring(L, -1);
        std::cout << "string: " << result << std::endl;
    }
    else
    {
        error(L, "invalid component in background color");
        lua_close(L);
        exit(EXIT_FAILURE);
    }
    // 弹出 上次执行的结果
    lua_pop(L, 1);  // 不执行的话，下一句无法获取值
    lua_gettable(L, -2);
    if (lua_isnumber(L, -1))
    {
        int result = (int)(lua_tonumber(L, -1) * MAX_COLOR);
        std::cout << "int: " << result << std::endl;
    }
    else if (lua_isstring(L, -1))
    {
        std::string result = lua_tostring(L, -1);
        std::cout << "string: " << result << std::endl;
    }
    else
    {
        error(L, "invalid component in background color");
        lua_close(L);
        exit(EXIT_FAILURE);
    }

    lua_close(L);
}
void load (const char * filename, int &width, int &height, int & red, int & green, int & blue)
{
    lua_State * L = lua_open();
    luaL_openlibs(L);
    //luaopen_base(L);
    //luaopen_string(L);
    //luaopen_io(L);
    //luaopen_math(L);

    if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
        error(L, "cannot run configuration file: %s", lua_tostring(L, -1));

    lua_getglobal(L, "width");
    lua_getglobal(L, "height");
    lua_getglobal(L, "background");
    if (!lua_isnumber(L, -3))
        error(L, "\"width\" should be a number \n");
    if (!lua_isnumber(L, -2))
        error(L, "\"height\" should be a number \n");
    if (!lua_istable(L, -1))
        error(L, "\"background\" should be a table \n");

    width = (int) lua_tonumber(L, -3);
    height = (int) lua_tonumber(L, -2);
    red = getfield(L, "r");
    green = getfield(L, "g");
    blue = getfield(L, "b");


    lua_close(L);
}

int main()
{
    const char * filename = "../../src/GettableTest/windowSize.lua";
    int width, height, red, green, blue;
    //gettableTest(filename);
    lua_rawgetiTest(filename);
    //load(filename, width, height, red, green, blue);
    //std::cout << "width: " << width << ", height: " << height << "\nbackground: " << red << "," << green << "," << blue << std::endl;
    system("pause");
    return 0;
}