#pragma once
#include "esp_spiffs.h"
#include "memory.hpp"
#include "system.hpp"
#include "process.hpp"
#include "file.hpp"
#include "lua.h"
#include "lauxlib.h"

namespace loader {
    /// 从文件系统加载一个二进制文件并放入PCB链表，0成功，1内存不足，2文件不存在，3文件错误
    int loadProgram(char *filePath) {
        if (!memory::check(sizeof(PCB))) {
            return 1;  // 内存不足以创建内存块
        }
        PCB *pcb = new PCB();
        if (pcb == nullptr) {
            return 1;
        }
        if (!SPIFFS.exists(filePath)) {
            return 2;
        }
        Serial.print("FilePath: ");
        Serial.println(filePath);
        pcb->name = file::getFileName(filePath);
        Serial.print("pcb->name: ");
        Serial.println(pcb->name);
		File f = SPIFFS.open(filePath, "r");
        pcb->memSize = f.size();
        Serial.print("memsize: ");
        Serial.println(pcb->memSize);
        if (!memory::check(pcb->memSize)) {
            delete pcb;
            f.close();
            return 1;  // 内存不足以创建内存块
        }
        pcb->dstMem = new char[pcb->memSize];
        if (pcb->dstMem == nullptr) {
            delete pcb;
            return 1;
        }
        f.read((uint8_t *)pcb->dstMem, pcb->memSize);
        f.close();
        Serial.println("Read Done. Starting State...");
        pcb->L = lua_newthread(system0::L);  // 创建state
        if (pcb->L == nullptr) {
            delete pcb->dstMem;
            delete pcb;
        }
        Serial.println("Started.");
        int result = luaL_loadbufferx(pcb->L, pcb->dstMem, pcb->memSize, pcb->name, nullptr);
        Serial.print("Buffer loaded: ");
        Serial.println(result);
        if (result == LUA_ERRSYNTAX) {
            lua_close(pcb->L);
            delete pcb->dstMem;
            delete pcb;
            return 3;
        } else if (result == LUA_ERRMEM) {
            lua_close(pcb->L);
            delete pcb->dstMem;
            delete pcb;
            return 1;
        }
        process::addProcess(pcb);
        return 0;
	}
} // namespace loader