#pragma once
#include "graphic.hpp"

namespace system0 {
    /// 致命错误（蓝屏）
    void critical(const char *errMsg) {
        graphic::tft.fillScreen(TFT_SKYBLUE);
        graphic::tft.drawCentreString(errMsg, 64, 64, 2);
        while (true);
    }

    const uint maxStrLen = 1024;
    uint strlen(const char *str) {
        uint i = 0;
        for (; i < maxStrLen; i++) {
            if (str[i] == '\0') {
                return i;
            }
        }
        return -1;
    }
}