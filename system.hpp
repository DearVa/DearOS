#pragma once
#include "graphic.hpp"
#include "process.hpp"
#include "lua.h"
#include "lauxlib.h"

namespace system0 {
    /// 致命错误（蓝屏）
    void critical(const char *errMsg) {
        graphic::tft.fillScreen(TFT_SKYBLUE);
        graphic::tft.drawCentreString(errMsg, 64, 64, 2);
        while (true);
    }

    /// 自定义strlen函数，规定了字符串的最大长度
    const uint maxStrLen = 1024;
    uint strlen(const char *str) {
        uint i = 0;
        for (; i < maxStrLen; i++) {
            if (str[i] == '\0') {
                return i;
            }
        }
        return -1;
    }

    static int millisLua(lua_State *L) {
        int n = lua_gettop(L);  // 获取参数个数
        if (n != 0) {
            lua_pushstring(L, "Incorrect argument");
            lua_error(L);
        }
        lua_pushinteger(L, millis());
        return 1;
    }

    static int delayLua(lua_State *L) {
        int n = lua_gettop(L);  // 获取参数个数
        if (n != 1) {
            lua_pushstring(L, "Incorrect argument");
            lua_error(L);
        }
        ulong t = (ulong)luaL_checkinteger(L, 1);
        endPCB->next->millis = t + millis();
        endPCB->next->timeLeft = 0;
        return 0;
    }

    /// 全局lua_State，新进程必须从此处创建thread
    lua_State *L;
    /// 创建一个lua_State，注册函数和类
    void setup() {
        L = luaL_newstate();
        if (L == nullptr) {
            critical("lua Error.");
        }
        graphic::registerLua(L);
        lua_register(L, "millis", millisLua);
        lua_register(L, "delay", delayLua);
    }
}