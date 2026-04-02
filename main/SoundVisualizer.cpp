#include "SoundVisualizer.h"

SoundVisualizer::SoundVisualizer() : lastPlotMs(0) {
}

void SoundVisualizer::installI2S() {
    const i2s_config_t cfg = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = I2S_SAMPLE_RATE,
        .bits_per_sample = i2s_bits_per_sample_t(16),
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = I2S_BUFFER_LEN,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };

    i2s_driver_install(I2S_PORT, &cfg, 0, nullptr);
}

void SoundVisualizer::setPins() {
    const i2s_pin_config_t pinCfg = {
        .mck_io_num = I2S_PIN_NO_CHANGE,
        .bck_io_num = I2S_SCK_PIN,
        .ws_io_num = I2S_WS_PIN,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_SD_PIN
    };

    i2s_set_pin(I2S_PORT, &pinCfg);
}

void SoundVisualizer::init() {
    installI2S();
    setPins();
    i2s_start(I2S_PORT);
}

int SoundVisualizer::readLevel() {
    int16_t buffer[I2S_BUFFER_LEN];
    size_t bytesIn = 0;

    const esp_err_t err = i2s_read(I2S_PORT, buffer, sizeof(buffer), &bytesIn, portMAX_DELAY);
    if (err != ESP_OK || bytesIn == 0) {
        return 0;
    }

    const int sampleCount = static_cast<int>(bytesIn / sizeof(int16_t));
    if (sampleCount <= 0) {
        return 0;
    }

    int64_t sumAbs = 0;
    for (int i = 0; i < sampleCount; i++) {
        sumAbs += abs(buffer[i]);
    }

    const int avgAbs = static_cast<int>(sumAbs / sampleCount);

    const unsigned long now = millis();
    if (now - lastPlotMs >= SERIAL_PLOT_INTERVAL_MS) {
        const int clamped = constrain(avgAbs, SERIAL_PLOT_MIN, SERIAL_PLOT_MAX);
        // One value per line for a clean real-time volume curve in Serial Plotter.
        Serial.println(clamped);
        lastPlotMs = now;
    }

    return avgAbs;
}
