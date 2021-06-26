#pragma once
#include <Esp.h>

namespace memory {
    const uint criticalSize = 512;  // 为系统预留的空间

    void setup() {
        Serial.println(ESP.getFreeHeap());
        int *a = new int(1);
        Serial.println(ESP.getFreeHeap());
    }

    /// 检查系统剩余内存是否够size
    inline bool check(uint size) {
        return ESP.getFreeHeap() >= size + criticalSize;
    }
}