#pragma once

namespace process {
	enum class ProcessState {
		Ready,
		Running,
		Waiting,
	};

	struct PCB {
		char name[16];       // 进程名
		uint pid;            // 进程ID (unused)
		ProcessState state;  // 进程状态 (unused)
        void *dstMem;        // 进程首地址
        uint memSize;        // 占用内存大小（仅程序段）
        void *memPtr;        // 当前运行处指针
		PCB *next;           // 下一个PCB块
	};

	PCB *lastPCB, *endPCB;      // 循环链表
	uint pcbCount;              // 进程块的数量

    const uint timeSpan = 100;  // 时间片

    /// 获取当前PCB
	inline PCB *nowPCB() {
		return lastPCB ? lastPCB->next : nullptr;
	}

    /// 将PCB块加入链表
	void addProcess(PCB *pcb) {
		if (nowPCB == nullptr) {
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
		if (lastPCB == endPCB) {
			return; // 队列仅剩一个PCB
		}
		auto temp = nowPCB();
		lastPCB->next = lastPCB->next->next;
		temp->next = endPCB->next;
		endPCB->next = temp;
		endPCB = temp;
	}

	void inline loop() {
        if (pcbCount == 0) {
            return;
        }
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