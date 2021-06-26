#pragma once
#include <SPI.h>
#include <TFT_eSPI.h> // 别忘了配置
#include "lua.h"

#define TFT_GREY 0x5AEB

namespace graphic {
	TFT_eSPI tft = TFT_eSPI();

	void setup() {
		tft.init();
		tft.setRotation(0);
		tft.fillScreen(TFT_GREY);

		tft.setTextColor(TFT_GREEN, TFT_GREY);
        tft.drawCentreString("DearOS", 64, 64, 1);
	}

    static int fillScreenLua(lua_State *L) {
        int n = lua_gettop(L);  // 获取参数个数
        if (n != 0 && !lua_isnumber(L, 1)) {
            lua_pushstring(L, "Incorrect argument");
            lua_error(L);
        }
        tft.fillScreen(lua_tonumber(L, 1));
        return 0;
    }

    void registerLua(lua_State *L) {
        lua_register(L, "fillScreen", fillScreenLua);
    }

	void loop() {
		
	}
} // namespace graphic