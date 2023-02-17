#include <iostream>
#include <fstream>
#include <string>

#include "lua.hpp"

int main() {

    lua_State* L = luaL_newstate();

    std::string filename = "myscript.lua";
    int result = luaL_loadfile(L, filename.c_str());

    if (result != LUA_OK) {
        std::cerr << "[LOTUS] Failed to load script: " << lua_tostring(L, -1) << std::endl;
        return 1;
    }

    std::string bytecodeFilename = "myscript.luac";
    std::ofstream bytecodeFile(bytecodeFilename, std::ios::binary);
    result = lua_dump(L, [](lua_State*, const void* p, size_t size, void* ud) -> int {
        std::ofstream* file = static_cast<std::ofstream*>(ud);
        file->write(static_cast<const char*>(p), size);
        return 0;
    }, &bytecodeFile);

    if (result != LUA_OK) {
        std::cerr << "[LOTUS] Failed to dump bytecode: " << lua_tostring(L, -1) << std::endl;
        return 1;
    }

    bytecodeFile.close();
    lua_close(L);

    std::cout << "[LOTUS] Bytecode saved to file: " << bytecodeFilename << std::endl;

    return 0;
}
