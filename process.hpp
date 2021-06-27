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

/// 进程控制块
struct PCB {
	char *name;          // 进程名
	uint pid;            // 进程ID (unused)
	ProcessState state;  // 进程状态
    char *dstMem;        // 进程首地址
    uint memSize;        // 占用内存大小（仅程序段）
    int timeLeft;        // 剩余时间片
    lua_State *L;        // 当前程序的Lua栈
	PCB *next;           // 下一个PCB块
};

extern PCB *lastPCB, *endPCB;      // 循环链表
extern uint pcbCount;              // 进程块的数量

void printS(const char *str, const char *msg);
void printI(const char *str, int msg);

namespace process {
    /// 将PCB块加入链表
	void addProcess(PCB *pcb);

    /// 将当前PCB移出队列并释放资源
    void inline removeNowPCB();

	void loop();
} // namespace process