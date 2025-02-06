const int motorPin = 3; // GPIO pin connected to the motor driver
const int maxFrequency = 2000; // Maximum PWM frequency in Hz
const int minFrequency = 500;  // Minimum PWM frequency in Hz
const int resolution = 8;      // 8-bit resolution (0-255)

void setup() {
    // Initial setup for PWM
    ledcSetup(0, minFrequency, resolution); // PWM channel 0 with min frequency
    ledcAttachPin(motorPin, 0);             // Attach motorPin to channel 0
}

void loop() {
    // Ramp up frequency and intensity
    for (int freq = minFrequency; freq <= maxFrequency; freq += 100) {
        ledcSetup(0, freq, resolution);     // Update PWM frequency
        for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle += 25) {
            ledcWrite(0, dutyCycle);        // Increase intensity
            delay(50);                      // Short delay for smooth transition
        }
    }

    // Ramp down frequency and intensity
    for (int freq = maxFrequency; freq >= minFrequency; freq -= 100) {
        ledcSetup(0, freq, resolution);     // Update PWM frequency
        for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle -= 25) {
            ledcWrite(0, dutyCycle);        // Decrease intensity
            delay(50);                      // Short delay for smooth transition
        }
    }
}
