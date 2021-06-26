#pragma once
#include <SPI.h>
#include <TFT_eSPI.h> // 别忘了配置

#define TFT_GREY 0x5AEB

namespace graphic {
	TFT_eSPI tft = TFT_eSPI();

	void setup() {
		tft.init();
		tft.setRotation(0);
		tft.fillScreen(TFT_GREY);

		tft.setTextColor(TFT_GREEN, TFT_GREY);
        tft.drawCentreString("DearOS", 64, 64, 3);
	}

	void loop() {
		
	}
} // namespace graphic