#pragma once
#include "lua.h"

namespace process {
	enum class ProcessState {
		Ready,
		Running,
		Waiting,
	};

	struct PCB {
		char *name;          // 进程名
		uint pid;            // 进程ID (unused)
		ProcessState state;  // 进程状态 (unused)
        char *dstMem;        // 进程首地址
        uint memSize;        // 占用内存大小（仅程序段）
        char *memPtr;        // 当前运行处指针
        lua_State *L;        // 当前程序的Lua栈
		PCB *next;           // 下一个PCB块
	};

    void deletePCB(PCB *pcb) {
        if (pcb != nullptr) {
            if (pcb->L != nullptr) {
                lua_close(pcb->L);
            }
            if (pcb->dstMem != nullptr) {
                delete pcb->dstMem;
            }
            if (pcb->name != nullptr) {
                delete pcb->name;
            }
            delete pcb;
        }
    }

	PCB *lastPCB = nullptr, *endPCB = nullptr;      // 循环链表
	uint pcbCount = 0;              // 进程块的数量

    const uint timeSpan = 100;  // 时间片，即运行lua指令条数

    /// 获取当前PCB
	inline PCB *nowPCB() {
		return lastPCB ? lastPCB->next : nullptr;
	}

    /// 将PCB块加入链表
	void addProcess(PCB *pcb) {
		if (nowPCB() == nullptr) {
			lastPCB = endPCB = pcb;
			pcb->next = pcb;
		} else {
			pcb->next = endPCB->next;
			endPCB->next = pcb;
			lastPCB = endPCB = pcb;
		}
		pcbCount++;
	}

    /// 将NowPCB移动到队列尾
	void inline bringToBack() {
		if (pcbCount <= 1) {
			return;
		}
		auto temp = nowPCB();
		lastPCB->next = lastPCB->next->next;
		temp->next = endPCB->next;
		endPCB->next = temp;
		endPCB = temp;
	}

    void inline removeNowPCB() {
        if (pcbCount == 0) {
			return;
		}
        auto temp = nowPCB();
        if (pcbCount == 1) {
            lastPCB = endPCB = nullptr;
        } else {
            lastPCB->next = lastPCB->next->next;
            if (endPCB == nowPCB()) {
                endPCB = lastPCB;
            }
        }
        deletePCB(temp);
        pcbCount--;
    }

	void inline loop() {
        if (pcbCount == 0) {
            return;
        }
        Serial.println("Running process " + String(nowPCB()->name));
        Serial.println(lua_pcall(nowPCB()->L, 0, 0, 0));
        removeNowPCB();
		// PrintWaitQueue();
		// bool flag = true;
		// for (ulong i = 0; i < timeSpan; i++) {
		// 	if (nowPCB() == null) {
		// 		break;
		// 	}
		// 	flag = Run();
		// 	if (flag) {
		// 		break;
		// 	}
		// }
		// if (!flag) {
		// 	bringToBack(); // 如果该任务还没有运行完，那就把它放到队尾
		// }
	}
} // namespace process