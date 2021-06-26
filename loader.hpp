#pragma once
#include "esp_spiffs.h"
#include "memory.hpp"
#include "system.hpp"
#include "process.hpp"
#include "file.hpp"

namespace loader {
    PCB *loadProgram(char *filePath) {
        if (!memory::check(sizeof(PCB))) {
            return nullptr;  // 内存不足以创建内存块
        }
        PCB *pcb = new PCB();
        pcb->name = file::getFileName(filePath);
		pcb->name[15] = '\0';
		File f = SPIFFS.open(filePath, "r");
        pcb->memSize = f.size();
        if (!memory::check(pcb->memSize)) {
            return nullptr;  // 内存不足以创建内存块
        }
        pcb->dstMem = malloc(pcb->memSize);
        if (pcb->dstMem == nullptr) {
            system::critical("Unable to allocate memory");
        }
        f.read(pcb->dstMem, pcb->memSize);
        pcb->memPtr = pcb->dstMem;
        process::addProcess(pcb);
	}
} // namespace loader