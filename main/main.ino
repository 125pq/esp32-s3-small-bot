#include <Arduino.h>

#include "Config.h"
#include "Display.h"
#include "SoundVisualizer.h"
#include "WiFi_Module.h"

Display display;
SoundVisualizer soundVisualizer;

void setup() {
	Serial.begin(SERIAL_BAUD);
	delay(300);

	// Call the encapsulated WiFi connection function from the external file
	connectToWiFi();

	#if ENABLE_DISPLAY_OUTPUT
	display.init();
	display.showBootTest();
	#endif

	soundVisualizer.init();
	#if ENABLE_DISPLAY_OUTPUT
	Serial.println("System ready: LCD + I2S mic -> Serial Plotter");
	#else
	Serial.println("System ready: I2S mic -> Serial Plotter");
	#endif
}

void loop() {
	const int level = soundVisualizer.readLevel();
	(void)level;

	#if ENABLE_DISPLAY_OUTPUT
	display.showBars(level);
	#endif

	delay(AUDIO_SAMPLE_DELAY_MS);
}
