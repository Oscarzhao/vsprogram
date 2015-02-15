extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
};

#include <cstring>
#include <iostream>

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

int main()
{
    lua_State* L = lua_open();
    lua_pushboolean(L, 1);
    lua_pushnumber(L, 10);
    lua_pushnil(L);
    lua_pushstring(L, "hello");
    stackDump(L);

    // Pushes a copy of the element at the given index onto the stack.
    // void lua_pushvalue(lua_State * L, int index);
    lua_pushvalue(L, -4);
    stackDump(L);

    // Moves the top element into the given valid index without shifting any element 
    // (therefore replacing the value at the given index), and then pops the top element.
    // void lua_replace (lua_State *L, int index);
    lua_replace(L, 3); 
    stackDump(L);
    
    // Accepts any index, or 0, and sets the stack top to this index. If the new top is larger than the old one, then the new elements are filled with nil. 
    // If index is 0, then all stack elements are removed.
    // void lua_settop (lua_State *L, int index);
    lua_settop(L, 6);
    stackDump(L);

    //Removes the element at the given valid index,
    // shifting down the elements above this index to fill the gap
    // This function cannot be called with a pseudo-index, because a pseudo-index is not an actual stack position.
    // void lua_remove (lua_State *L, int index);
    lua_remove(L, -3);
    stackDump(L);

    lua_settop(L, -5); 
    stackDump(L);

    lua_close(L);
    system("pause");
    return 0;
}