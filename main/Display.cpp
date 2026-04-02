#include "Display.h"

namespace {
static const uint8_t FONT5X7[][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // space
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // )
    {0x14, 0x08, 0x3E, 0x08, 0x14}, // *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // ?
    {0x32, 0x49, 0x79, 0x41, 0x3E}, // @
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}  // Z
};

static inline uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return static_cast<uint16_t>(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
}
}

#if defined(FSPI)
static SPIClass g_lcdSpi(FSPI);
#elif defined(HSPI)
static SPIClass g_lcdSpi(HSPI);
#else
static SPIClass g_lcdSpi;
#endif

Display::Display() : spi(&g_lcdSpi) {
}

void Display::spiWriteCompat(const uint8_t *data, size_t len) {
    if (data == nullptr || len == 0) {
        return;
    }

#if defined(ARDUINO_ARCH_ESP32)
    spi->writeBytes(data, len);
#else
    for (size_t i = 0; i < len; i++) {
        spi->transfer(data[i]);
    }
#endif
}

void Display::beginWrite() {
    digitalWrite(LCD_CS_PIN, LOW);
}

void Display::endWrite() {
    digitalWrite(LCD_CS_PIN, HIGH);
}

void Display::writeCmd(uint8_t cmd) {
    beginWrite();
    digitalWrite(LCD_DC_PIN, LOW);
    spi->transfer(cmd);
    endWrite();
}

void Display::writeData(const uint8_t *data, size_t len) {
    if (data == nullptr || len == 0) {
        return;
    }

    beginWrite();
    digitalWrite(LCD_DC_PIN, HIGH);
    spiWriteCompat(data, len);
    endWrite();
}

void Display::setAddrWindow(int x0, int y0, int x1, int y1) {
    x0 += LCD_OFFSET_X;
    x1 += LCD_OFFSET_X;
    y0 += LCD_OFFSET_Y;
    y1 += LCD_OFFSET_Y;

    const uint8_t col[] = {
        static_cast<uint8_t>(x0 >> 8), static_cast<uint8_t>(x0),
        static_cast<uint8_t>(x1 >> 8), static_cast<uint8_t>(x1)
    };
    const uint8_t row[] = {
        static_cast<uint8_t>(y0 >> 8), static_cast<uint8_t>(y0),
        static_cast<uint8_t>(y1 >> 8), static_cast<uint8_t>(y1)
    };

    writeCmd(ST7789_CASET);
    writeData(col, sizeof(col));
    writeCmd(ST7789_RASET);
    writeData(row, sizeof(row));
    writeCmd(ST7789_RAMWR);
}

void Display::fillRect(int x, int y, int w, int h, uint16_t color565) {
    if (w <= 0 || h <= 0) {
        return;
    }

    const int x1 = x + w - 1;
    const int y1 = y + h - 1;
    setAddrWindow(x, y, x1, y1);

    const uint8_t hi = static_cast<uint8_t>(color565 >> 8);
    const uint8_t lo = static_cast<uint8_t>(color565 & 0xFF);
    uint8_t line[2 * LCD_WIDTH];

    for (int i = 0; i < w; i++) {
        line[2 * i] = hi;
        line[2 * i + 1] = lo;
    }

    beginWrite();
    digitalWrite(LCD_DC_PIN, HIGH);
    for (int j = 0; j < h; j++) {
        spiWriteCompat(line, static_cast<size_t>(w) * 2);
    }
    endWrite();
}

void Display::fillScreen(uint16_t color565) {
    fillRect(0, 0, LCD_WIDTH, LCD_HEIGHT, color565);
}

void Display::drawChar5x7(int x, int y, char c, uint16_t color565, uint16_t bg565, uint8_t scale) {
    if (c < ' ' || c > 'Z') {
        c = '?';
    }

    const uint8_t *glyph = FONT5X7[c - ' '];
    for (int col = 0; col < 5; col++) {
        uint8_t bits = glyph[col];
        for (int row = 0; row < 7; row++) {
            const uint16_t pixel = (bits & 0x01) ? color565 : bg565;
            fillRect(x + col * scale, y + row * scale, scale, scale, pixel);
            bits >>= 1;
        }
    }

    fillRect(x + 5 * scale, y, scale, 7 * scale, bg565);
}

void Display::drawText(int x, int y, const char *text, uint16_t color565, uint16_t bg565, uint8_t scale) {
    if (text == nullptr) {
        return;
    }

    int cursorX = x;
    while (*text) {
        drawChar5x7(cursorX, y, *text, color565, bg565, scale);
        cursorX += 6 * scale;
        text++;
    }
}

void Display::showBootTest() {
    fillScreen(rgb565(255, 0, 0));
    delay(250);
    fillScreen(rgb565(0, 255, 0));
    delay(250);
    fillScreen(rgb565(0, 0, 255));
    delay(250);

    fillScreen(rgb565(5, 5, 5));
    drawText(16, 24, "ESP32-S3 BOT", rgb565(255, 220, 80), rgb565(5, 5, 5), 3);
    delay(600);
}

void Display::showBars(int level) {
    static unsigned long lastUpdateMs = 0;
    const unsigned long now = millis();
    if (now - lastUpdateMs < DISPLAY_UPDATE_MS) {
        return;
    }
    lastUpdateMs = now;

    const int clamped = constrain(level, 0, 3000);
    const int barHeight = map(clamped, 0, 3000, 0, LCD_HEIGHT - 70);

    fillScreen(rgb565(8, 10, 16));
    drawText(12, 10, "MIC LEVEL", rgb565(255, 255, 255), rgb565(8, 10, 16), 2);

    const int barX = 40;
    const int barY = LCD_HEIGHT - 20 - (LCD_HEIGHT - 70);
    const int barW = LCD_WIDTH - 80;
    const int barAreaH = LCD_HEIGHT - 70;

    fillRect(barX, barY, barW, barAreaH, rgb565(28, 32, 42));
    if (barHeight > 0) {
        const int y = barY + barAreaH - barHeight;
        fillRect(barX, y, barW, barHeight, rgb565(100, 230, 120));
    }

    char line[24];
    snprintf(line, sizeof(line), "AMP:%4d", clamped);
    drawText(55, LCD_HEIGHT - 18, line, rgb565(255, 220, 90), rgb565(8, 10, 16), 2);
}

void Display::init() {
    pinMode(LCD_CS_PIN, OUTPUT);
    pinMode(LCD_DC_PIN, OUTPUT);
    pinMode(LCD_RST_PIN, OUTPUT);

    digitalWrite(LCD_CS_PIN, HIGH);
    digitalWrite(LCD_DC_PIN, HIGH);
    digitalWrite(LCD_RST_PIN, HIGH);

#if defined(ARDUINO_ARCH_ESP32)
    spi->begin(LCD_SCK_PIN, -1, LCD_SDA_PIN, LCD_CS_PIN);
#else
    spi->begin();
#endif

    spi->beginTransaction(SPISettings(LCD_SPI_FREQ, MSBFIRST, SPI_MODE3));

    digitalWrite(LCD_RST_PIN, LOW);
    delay(20);
    digitalWrite(LCD_RST_PIN, HIGH);
    delay(120);

    writeCmd(ST7789_SWRESET);
    delay(150);
    writeCmd(ST7789_SLPOUT);
    delay(120);
    writeCmd(ST7789_NORON);

    uint8_t madctl = ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB;
    writeCmd(ST7789_MADCTL);
    writeData(&madctl, 1);

    uint8_t colmod = 0x55;
    writeCmd(ST7789_COLMOD);
    writeData(&colmod, 1);
    delay(10);

    writeCmd(ST7789_INVON);
    writeCmd(ST7789_DISPON);
    delay(20);
}
