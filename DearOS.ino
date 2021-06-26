#include "file.hpp"
#include "graphic.hpp"
#include "memory.hpp"
#include "process.hpp"
#include "loader.hpp"

void setup() {
	Serial.begin(115200);
	memory::setup();
	graphic::setup();
	file::setup();
	Serial.println(ESP.getFreeHeap());
    Serial.println("Load Program...");
	Serial.println(loader::loadProgram("/explorer"));
	Serial.println(ESP.getFreeHeap());
}

void loop() {
	process::loop();
	// graphic::loop();
}