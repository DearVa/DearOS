#include "file.hpp"
#include "graphic.hpp"
#include "memory.hpp"
#include "process.hpp"

#include "lua.h"
#include "ltests.h"
#include "lstate.h"
#include "lauxlib.h"
#include "lualib.h"

// static int loadline(lua_State *L) {
//   int status;
//   lua_settop(L, 0);
//   if (!pushline(L, 1))
//     return -1;  /* no input */
//   if ((status = addreturn(L)) != LUA_OK)  /* 'return ...' did not work? */
//     status = multiline(L);  /* try as command, maybe with continuation lines */
//   lua_remove(L, 1);  /* remove line from the stack */
//   lua_assert(lua_gettop(L) == 1);
//   return status;
// }

// static int doREPL(lua_State *L) {
// 	int status;
// 	lua_initreadline(L);
// 	while ((status = loadline(L)) != -1) {
// 		if (status == LUA_OK)
// 			status = docall(L, 0, LUA_MULTRET);
// 		if (status == LUA_OK)
// 			l_print(L);
// 		else
// 			report(L, status);
// 	}
// 	lua_settop(L, 0); /* clear stack */
// 	lua_writeline();
// }

void setup() {
	Serial.begin(115200);
	memory::setup();
	graphic::setup();
	file::setup();
	Serial.println(ESP.getFreeHeap());
	lua_State *L = luaL_newstate(); /* create state */
	if (L == NULL) {
		Serial.println("Cannot create state: not enough memory");
		return;
	}
	lua_close(L);
	Serial.println(ESP.getFreeHeap());
}

void loop() {
	process::loop();
	graphic::loop();
}