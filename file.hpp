#pragma once
#include <SPIFFS.h>
#include "system.hpp"

namespace file {
    void setup() {
        if (!SPIFFS.begin()) {
            system0::critical("SPIFFS Fail");
        }
    }

    char *getFileName(char *filePath) {
        uint len = system0::strlen(filePath);
        if (len <= 1) {
            return "";
        }
        uint i = len - 1;
        for (; i > 0; i--) {
            if (filePath[i] == '/') {
                return filePath + i + 1;
            }
        }
        return "";
    }
}