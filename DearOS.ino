#include "file.hpp"
#include "graphic.hpp"
#include "memory.hpp"
#include "process.hpp"
#include "loader.hpp"
#include "system.hpp"

/// 万物之根源
void setup() {
	Serial.begin(115200);
	memory::setup();
	graphic::setup();
	file::setup();
    system0::setup();
	Serial.println(ESP.getFreeHeap());
    Serial.println("Load Program...");
	Serial.println(loader::loadProgram("/test1"));
	Serial.println(loader::loadProgram("/test2"));
	Serial.println(ESP.getFreeHeap());
}

void loop() {
	process::loop();
}