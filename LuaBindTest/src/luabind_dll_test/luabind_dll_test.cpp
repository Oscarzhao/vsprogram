// Copyright (c) 2005 Daniel Wallin and Arvid Norberg
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.


/*
* 这个程序生成的 dll 不能使用 doc 里的代码进行测试
* 需要额外 改变 luaconf.h 文件，并且重新编译luabind
*/

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

#include <iostream>
#include <luabind/luabind.hpp>

void greet()
{
    std::cout << "Nice to meet you\n";
}

extern "C" int init(lua_State* L)
{
    luabind::open(L);
    luabind::module(L)
    [
        luabind::def("greet", &greet)
    ];

    return 0;
}