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
        for (; i >= 0; i--) {
            if (filePath[i] == '/') {
                uint newLen = len - i;
                char *result = new char[newLen];
                for (uint j = 0; j < newLen; j++) {
                    result[j] = filePath[i + 1 + j];
                }
                return result;
            }
        }
        return "";
    }
}