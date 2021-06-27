#pragma once
#include "lua.h"
#include "llimits.h"
#include "lobject.h"

typedef unsigned int uint;

enum class ProcessState {
	Ready,
	Running,
	Waiting,
    Terminated,
};

typedef unsigned long ulong;

/// 进程控制块
struct PCB {
	char *name;          // 进程名
	uint pid;            // 进程ID (unused)
	ProcessState state;  // 进程状态
    char *dstMem;        // 进程首地址
    uint memSize;        // 占用内存大小（仅程序段）
    int timeLeft;        // 剩余时间片
    lua_State *L;        // 当前程序的Lua栈
    ulong millis;        // 希望被唤醒的时钟刻，用于delay
	PCB *next;           // 下一个PCB块
};

#define nowPCB (endPCB ? endPCB->next : nullptr)

extern PCB *endPCB;                // 循环链表，指向队尾
extern uint pcbCount;              // 进程块的数量

void printS(const char *str, const char *msg);
void printI(const char *str, int msg);

namespace process {
    /// 将PCB块加入链表
	void addProcess(PCB *pcb);

    /// 将当前PCB移出队列并释放资源
    void inline removeNowPCB();
    /// 将当前PCB移到队尾
    void inline bringNowPCBToBack();

	void loop();
} // namespace process