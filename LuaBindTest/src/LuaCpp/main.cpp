#include <iostream>
#include <fstream>
#include <string>
#include <lua.hpp>
#include <lua.h>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <boost/timer.hpp>
#include <boost/shared_ptr.hpp>
#include "Apple.h"

using std::string;
using std::cout;
using std::endl;
using std::ios;
using std::fstream;

void testFunc()
{
    std::cout << "Hi, there.  I am a cpp function" << std::endl;
}

// 将 C++ 函数暴露给 lua
bool initApple(lua_State * state)
{
    using namespace luabind;
    module(state)
        [
            def("testFunc", &testFunc),

            class_<Apple>("Apple")
                .def(constructor<>())
                .def("cleanApple", &Apple::cleanApple)
                .def("eatApple", &Apple::eatApple)
                .def("anyCFunc", &Apple::anyCFunc)
                .def("getWeight",&Apple::getWeight)
                .def("cal", &Apple::cal)
        ];
    return true;
}

// luabind 调用 C++ 函数
void luaCallCpp(lua_State* state, int times)
{
    double start = 10;
    double d = 0;
    for (int i = 0; i < times; i++)
    {
        d = luabind::call_function<double>(state, "testCalFunc", start);
    }
}

// luabind 调用智能指针
// 读取和调用 test.lua 中的变量和函数
void readLuaFile(lua_State* L)
{
    try
    {
        int nLuaGlobal = luabind::object_cast<int>(luabind::globals(L)["nGlobal"]);
        string strLuaGlobal = luabind::object_cast<string>(luabind::globals(L)["strGlobal"]);

        // 获取table，通过 luabind::object
        luabind::object luaTable = luabind::globals(L)["t"];
        string name = luabind::object_cast<string>(luaTable["name"]);
        int age = luabind::object_cast<int>(luaTable["age"]);

        // 获取table， 通过 gettable
        string desc = luabind::object_cast<string>(luabind::gettable(luaTable, "desc"));


        // 下面是 调用 lua 函数
        int nAddRes = luabind::call_function<int>(L, "add", 3, 5);
        string strEchoRes = luabind::call_function<string>(L, "strEcho", "C++参数");
    }
    catch (...)
    {
        std::cout << "Exception happens" << std::endl;
    }
}

int main()
{
    // 首先声明 luaState 环境
    lua_State * L = lua_open();   // 也可以使用 luaL_newState()
    luaL_openlibs(L);   // 注意将 lua 默认库打开
    luabind::open(L);

    
    boost::timer t1;  // 开始计时
    std::ofstream logfile;
    logfile.open("../config/release_luabind.csv", ios::out);
    
    initApple(L);

    // 加载lua 脚本，
    int ret_code = luaL_dofile(L, "../config/cal.lua");
    if (0 != ret_code)
    {
        std::cout << "Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        lua_close(L);
        exit(1);
    }
    logfile << "注册luabind，加载lua脚本," << t1.elapsed() << "s\n";
    for(int i = 1; i < 101; i++)
    {
        int times = 10000 * i;
        t1.restart();
        luaCallCpp(L, times);
        double time = t1.elapsed();
        logfile << times << "," << time << "s\n";
    }
    
    logfile.close();
    lua_close(L);
    system("pause");
    return 0;
}