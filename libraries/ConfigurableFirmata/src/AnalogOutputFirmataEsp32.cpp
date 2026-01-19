#include <Arduino.h>
#include "AnalogOutputFirmata.h"
#include <ConfigurableFirmata.h>

#define LEDC_BASE_FREQ 5000
#define DEFAULT_PWM_RESOLUTION 13
#define MAX_PWM_CHANNELS 16

AnalogOutputFirmata::AnalogOutputFirmata() {}

void AnalogOutputFirmata::reset() {
    internalReset();
}

bool AnalogOutputFirmata::handleSysex(byte command, byte argc, byte* argv) {
    // If no special handling is required for Sysex commands, just return false.
    return false;
}

void AnalogOutputFirmata::analogWriteInternal(uint8_t pin, uint32_t value) {
    uint32_t duty = min(value, static_cast<uint32_t>((1 << DEFAULT_PWM_RESOLUTION) - 1));
    ledcWrite(pin, duty);
}

void AnalogOutputFirmata::setupPwmPin(byte pin) {
    static int channel = 0;
    if (channel >= MAX_PWM_CHANNELS) {
        Firmata.sendStringf(F("Warning: Too many PWM channels used, unable to configure pin %d"), pin);
        return;
    }
    ledcSetup(channel, LEDC_BASE_FREQ, DEFAULT_PWM_RESOLUTION);
    ledcAttachPin(pin, channel);
    ledcWrite(channel, 0);
    channel++;
}

void AnalogOutputFirmata::internalReset() {
    for (int i = 0; i < MAX_PWM_CHANNELS; i++) {
        ledcDetachPin(i);
    }
}

bool AnalogOutputFirmata::handlePinMode(byte pin, int mode) {
    if (mode == PIN_MODE_PWM && FIRMATA_IS_PIN_PWM(pin)) {
        setupPwmPin(pin);
        return true;
    }
    if (mode != PIN_MODE_PWM && Firmata.getPinMode(pin) == PIN_MODE_PWM) {
        ledcDetachPin(pin);
    }
    return false;
}

void AnalogOutputFirmata::handleCapability(byte pin) {
    if (FIRMATA_IS_PIN_PWM(pin)) {
        Firmata.write(PIN_MODE_PWM);
        Firmata.write(DEFAULT_PWM_RESOLUTION);
    }
}
