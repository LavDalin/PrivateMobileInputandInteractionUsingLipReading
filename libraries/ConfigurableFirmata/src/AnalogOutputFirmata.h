#ifndef AnalogOutputFirmata_h
#define AnalogOutputFirmata_h

#include <FirmataFeature.h>
#include <ConfigurableFirmata.h>
#include <Arduino.h> // For Arduino types

class AnalogOutputFirmata : public FirmataFeature {
public:
    AnalogOutputFirmata();
    void reset();
    void analogWriteInternal(uint8_t pin, uint32_t value);
    void setupPwmPin(byte pin);
    void internalReset();
    bool handlePinMode(byte pin, int mode);
    void handleCapability(byte pin);
    bool handleSysex(byte command, byte argc, byte* argv) override;
};

#endif
