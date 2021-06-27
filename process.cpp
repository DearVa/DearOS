#include "process.hpp"
#include "HardwareSerial.h"

PCB *lastPCB = nullptr, *endPCB = nullptr;
uint pcbCount = 0;
const int timeSpan = 128;  // 时间片，即运行lua指令条数

inline PCB *nowPCB() {
	return lastPCB ? lastPCB->next : nullptr;
}

void printS(const char *str, const char *msg) {
    Serial.print(str);
    Serial.println(msg);
}

void printI(const char *str, int msg) {
    Serial.print(str);
    Serial.println(msg);
}

namespace process {
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

    void removeNowPCB() {
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
        if (temp->dstMem != nullptr) {
            delete temp->dstMem;
        }
        if (temp->name != nullptr) {
            delete temp->name;
        }
        delete temp;
        pcbCount--;
    }

    void loop() {
        if (pcbCount == 0) {
            return;
        }
        auto temp = nowPCB();
        temp->timeLeft = timeSpan;
        int yCount;  // 存放返回值个数
        int result = lua_resume(temp->L, 0, 0, &yCount);
        if (result != 0) {  // 发生运行时错误
            removeNowPCB();
        } else if (temp->state == ProcessState::Terminated) {  // 进程运行完毕
            removeNowPCB();
		} else {  // 如果该程还没有运行完，那就把它放到队尾
			if (pcbCount <= 1) {
				return;
			}
			lastPCB->next = lastPCB->next->next;
			temp->next = endPCB->next;
			endPCB->next = temp;
			endPCB = temp;
		}
	}
}