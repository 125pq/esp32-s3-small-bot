#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// LCD (ST7789) pins
#define LCD_SDA_PIN 11
#define LCD_SCK_PIN 12
#define LCD_CS_PIN 10
#define LCD_DC_PIN 21
#define LCD_RST_PIN 14

// LCD panel settings
#define LCD_WIDTH 240
#define LCD_HEIGHT 240
#define LCD_OFFSET_X 0
#define LCD_OFFSET_Y 0
#define LCD_SPI_FREQ 40000000

// I2S microphone pins
#define I2S_SCK_PIN 19
#define I2S_WS_PIN 20
// Avoid conflict with LCD_DC_PIN (21). Change this to 21 only if your wiring requires it.
#define I2S_SD_PIN 18

#define I2S_SAMPLE_RATE 44100
#define I2S_PORT I2S_NUM_0
#define I2S_BUFFER_LEN 64

// Serial and app timing
#define SERIAL_BAUD 115200
#define DISPLAY_UPDATE_MS 100
#define AUDIO_SAMPLE_DELAY_MS 10

// Runtime feature switch: enable screen refresh for visual verification.
#define ENABLE_DISPLAY_OUTPUT 1

// Serial plotter settings
#define SERIAL_PLOT_MIN 0
#define SERIAL_PLOT_MAX 4095
#define SERIAL_PLOT_INTERVAL_MS 25

#endif // CONFIG_H 
