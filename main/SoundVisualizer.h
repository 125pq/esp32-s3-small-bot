#ifndef SOUND_VISUALIZER_H
#define SOUND_VISUALIZER_H

#include <Arduino.h>
#include <driver/i2s.h>
#include "Config.h"

class SoundVisualizer {
public:
    SoundVisualizer();

    void init();
    int readLevel();

private:
    void installI2S();
    void setPins();

    unsigned long lastPlotMs;
};

#endif // SOUND_VISUALIZER_H
