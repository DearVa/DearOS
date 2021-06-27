#include "process.hpp"
#include "HardwareSerial.h"
#include <Esp.h>

PCB *endPCB = nullptr;
uint pcbCount = 0;
const int timeSpan = 128;  // 时间片，即运行lua指令条数

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
		if (pcbCount == 0) {
			endPCB = pcb;  // 构造循环链表
			pcb->next = pcb;
		} else {
			pcb->next = endPCB->next;
			endPCB->next = pcb;
			endPCB = pcb;
		}
		pcbCount++;
	}

    void removeNowPCB() {
        if (pcbCount == 0) {
			return;
		}
        auto temp = endPCB->next;
        if (pcbCount == 1) {
            endPCB = nullptr;
        } else {
            endPCB->next = temp->next;
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

    void bringNowPCBToBack() {
		if (pcbCount <= 1) {
			return;
		}
        endPCB = endPCB->next;
	}

	void loop() {
        if (pcbCount == 0) {
            return;
        }
        auto temp = nowPCB;
        printS("当前PCB：", temp->name);
        if (temp->millis > millis()) {  // 尚未达到需要轮转的时间
            bringNowPCBToBack();
            return;
        }
        temp->timeLeft = timeSpan;
        int yCount;  // 存放返回值个数
        printS("开始运行：", temp->name);
        int result = lua_resume(temp->L, 0, 0, &yCount);
        if (result != 0) {  // 发生运行时错误
            printS("运行时错误：", lua_tostring(temp->L, -1));
            removeNowPCB();
        } else if (temp->state == ProcessState::Terminated) {  // 进程运行完毕
            printS("运行完毕：", temp->name);
            removeNowPCB();
		} else {  // 如果该程还没有运行完，那就把它放到队尾
			bringNowPCBToBack();
		}
	}
}