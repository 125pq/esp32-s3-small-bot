#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include "Config.h"

class Display {
public:
    Display();

    void init();
    void fillScreen(uint16_t color565);
    void showBootTest();
    void showBars(int level);

private:
    static const uint8_t ST7789_SWRESET = 0x01;
    static const uint8_t ST7789_SLPOUT = 0x11;
    static const uint8_t ST7789_NORON = 0x13;
    static const uint8_t ST7789_INVON = 0x21;
    static const uint8_t ST7789_DISPON = 0x29;
    static const uint8_t ST7789_CASET = 0x2A;
    static const uint8_t ST7789_RASET = 0x2B;
    static const uint8_t ST7789_RAMWR = 0x2C;
    static const uint8_t ST7789_MADCTL = 0x36;
    static const uint8_t ST7789_COLMOD = 0x3A;

    static const uint8_t ST7789_MADCTL_MX = 0x40;
    static const uint8_t ST7789_MADCTL_MV = 0x20;
    static const uint8_t ST7789_MADCTL_RGB = 0x00;

    SPIClass *spi;

    void beginWrite();
    void endWrite();
    void writeCmd(uint8_t cmd);
    void writeData(const uint8_t *data, size_t len);
    void setAddrWindow(int x0, int y0, int x1, int y1);
    void fillRect(int x, int y, int w, int h, uint16_t color565);
    void spiWriteCompat(const uint8_t *data, size_t len);

    void drawChar5x7(int x, int y, char c, uint16_t color565, uint16_t bg565, uint8_t scale);
    void drawText(int x, int y, const char *text, uint16_t color565, uint16_t bg565, uint8_t scale);
};

#endif // DISPLAY_H
