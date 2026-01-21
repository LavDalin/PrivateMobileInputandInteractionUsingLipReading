# DRV2605 Haptic Directional Pulling System

Professional-grade haptic feedback controller using DRV2605 haptic drivers and PCA9548A I2C multiplexer to create realistic directional "pulling" sensations through calibrated haptic waveforms.

## Hardware Requirements

### Essential Components
- **Arduino Board** (Uno, Nano, Mega, or ESP32)
- **4x Adafruit DRV2605 Haptic Motor Controllers**
- **1x PCA9548A I2C Multiplexer** (8-channel)
- **4x LRA (Linear Resonant Actuator) Haptic Motors**
  - Recommended: Adafruit Product ID 1201 or similar
  - Coin-style LRA motors (8-12mm diameter)
- **I2C Pull-up Resistors** (4.7kΩ, often built into modules)
- **Power Supply**: 3-5V for motors (can use Arduino 5V if total current < 500mA)

### Why This Hardware?

**DRV2605 Advantages:**
- 123+ pre-programmed haptic waveforms (clicks, buzzes, alerts, transitions)
- Automatic overdrive and braking for crisp effects
- Library of effects optimized for LRA and ERM motors
- Precise intensity control via overdrive voltage
- I2C control (simple 2-wire interface)

**PCA9548A Multiplexer:**
- All DRV2605 chips have the same I2C address (0x5A)
- Multiplexer allows controlling 4+ identical devices
- 8 channels available (this project uses 4)

## Pin Configuration

### I2C Connections (All Devices)

```
Arduino SDA  → PCA9548A SDA → All DRV2605 SDA pins
Arduino SCL  → PCA9548A SCL → All DRV2605 SCL pins
5V           → PCA9548A VIN → All DRV2605 VIN pins
GND          → PCA9548A GND → All DRV2605 GND pins
```

### Motor Channels via PCA9548A

```
PCA9548A Channel 1 → DRV2605 #1 → Down Motor
PCA9548A Channel 2 → DRV2605 #2 → Right Motor
PCA9548A Channel 3 → DRV2605 #3 → Up Motor
PCA9548A Channel 4 → DRV2605 #4 → Left Motor
```

### Physical Motor Layout

```
           ┌─────────┐
           │    ●3   │   (Channel 3: Up)
           │   (UP)  │
           │         │
      ●4   │         │   ●2
    (LEFT) │         │  (RIGHT)
           │         │
           │ (DOWN)  │
           │    ●1   │   (Channel 1: Down)
           └─────────┘
```

## Features

- **Directional Pulling Sensations**: Realistic force feedback in 4 directions
- **Professional Haptic Effects**: Uses DRV2605's waveform library (123+ effects)
- **Per-Motor Calibration**: Compensates for individual motor variations
- **Overdrive Control**: Precise intensity adjustment (0-255)
- **I2C Multiplexing**: Control multiple identical I2C devices
- **Testing & Calibration Tools**: Built-in commands for motor tuning
- **Continuous Operation**: Repeating pull patterns with adjustable timing

## Installation

### 1. Install Required Libraries

In Arduino IDE, install these libraries via Library Manager:

```
Tools → Manage Libraries → Search and Install:
- Adafruit DRV2605 Library (by Adafruit)
- Wire Library (usually pre-installed)
```

### 2. Hardware Assembly

**Step 1: Connect PCA9548A to Arduino**
```
Arduino 5V  → PCA9548A VIN
Arduino GND → PCA9548A GND
Arduino SDA → PCA9548A SDA (A4 on Uno, 21 on ESP32)
Arduino SCL → PCA9548A SCL (A5 on Uno, 22 on ESP32)
```

**Step 2: Connect DRV2605 Boards to Multiplexer**

For each DRV2605 (repeat 4 times):
```
PCA9548A SD0-SD3 → DRV2605 SDA (one per channel)
PCA9548A SC0-SC3 → DRV2605 SCL (one per channel)
5V Power Bus      → DRV2605 VIN
GND Bus           → DRV2605 GND
```

**Step 3: Connect Motors to DRV2605**
```
Each DRV2605 has two motor terminals (+ and -)
Connect LRA motor leads (polarity doesn't matter for LRA)
```

### 3. Upload Code

1. Open the sketch in Arduino IDE
2. Select your board and port
3. Upload the code
4. Open Serial Monitor (115200 baud)

## Usage

### Basic Commands

Open Serial Monitor at **115200 baud** and enter commands:

**Direction Control:**
- `u` - Pull **UP** (continuous)
- `d` - Pull **DOWN** (continuous)
- `l` - Pull **LEFT** (continuous)
- `r` - Pull **RIGHT** (continuous)
- `s` - **STOP** pulling

**Calibration & Testing:**
- `c` - Show current **calibration** values
- `t[channel][overdrive]` - **Test** specific motor

### How Pulling Works

The system creates a pulling sensation by combining:
1. **Strong pulse** in the target direction
2. **40ms delay**
3. **Weak counter-pulse** in the opposite direction

**Example: Pulling UP**
```
Step 1: Up motor fires STRONG (overdrive 120)
   │    
   │ 40ms delay
   ▼    
Step 2: Down motor fires weak (overdrive 60)
   =    
Result: User feels pulled upward!
```

This **opponent process** creates the illusion of directional force!

### Testing & Calibration

Each motor may have slightly different characteristics. Use the test command to calibrate:

**Test Motor Syntax:**
```
t[channel][overdrive]

Examples:
t1100  → Test Channel 1 (Down) with overdrive 100
t2200  → Test Channel 2 (Right) with overdrive 200
t3150  → Test Channel 3 (Up) with overdrive 150
t4080  → Test Channel 4 (Left) with overdrive 80
```

**Calibration Process:**

1. Test each motor at different overdrive values:
   ```
   > t1050
   > t1100
   > t1150
   > t1200
   ```

2. Find the overdrive values where motors feel equal strength

3. Update the `motorCal` array in code:
   ```cpp
   MotorCalibration motorCal[5] = {
     {0, 0},         // Channel 0 (unused)
     {100, 50},      // Channel 1 - your values
     {200, 100},     // Channel 2 - your values
     {150, 75},      // Channel 3 - your values
     {150, 75}       // Channel 4 - your values
   };
   ```

4. Re-upload code and test pulling directions

### Serial Monitor Output

```
Haptic Motor Controller Ready
Commands:
  u/d/l/r - Set direction
  s - Stop
  t[channel][overdrive] - Test motor (e.g., 't1100' tests motor 1 with overdrive 100)
  c - Show current calibration

> u
Direction set to: u
Pulling ↑ Up
Pulling ↑ Up
Pulling ↑ Up

> s
Stopped

> c
Current Motor Calibration:
Channel | Strong | Weak | (Overdrive values 0-255)
--------|--------|------
   1    |   60   |  30
   2    |   255  |  120
   3    |   120  |  60
   4    |   120  |  60

> t2150
Testing motor 2 with overdrive 150
```

## Code Parameters

### Motor Calibration

Adjust overdrive values for each motor:

```cpp
MotorCalibration motorCal[5] = {
  {0, 0},       // Channel 0 (unused)
  {60, 30},     // Channel 1 (Down) - Strong, Weak
  {255, 120},   // Channel 2 (Right) - Strong, Weak
  {120, 60},    // Channel 3 (Up) - Strong, Weak
  {120, 60}     // Channel 4 (Left) - Strong, Weak
};
```

- **Strong**: Overdrive for primary direction (0-255)
- **Weak**: Overdrive for counter-pulse (0-255)
- Higher values = stronger vibration
- Typical range: 60-255 for strong, 30-120 for weak

### Timing Parameters

```cpp
void pullUp() {
  triggerMotor(3, true);   // Strong pulse
  delay(40);               // ← Adjust this (20-100ms recommended)
  triggerMotor(1, false);  // Weak counter-pulse
}

void loop() {
  // ...
  delay(500);  // ← Pause between pull cycles (200-1000ms)
}
```

### DRV2605 Waveform

Current configuration uses waveform #47 (strong click):

```cpp
drv.setWaveform(0, 47);  // Effect 47 = Strong Click 100%
```

**Alternative Waveforms to Try:**
```cpp
drv.setWaveform(0, 1);   // Strong Click - 100%
drv.setWaveform(0, 14);  // Sharp Click - 100%
drv.setWaveform(0, 47);  // Strong Click 60% (current)
drv.setWaveform(0, 52);  // Sharp Tick 1 - 100%
drv.setWaveform(0, 10);  // Double Click - 100%
```

See [DRV2605 datasheet](https://www.ti.com/lit/ds/symlink/drv2605.pdf) for full waveform library.

## Advanced Customization

### Multi-Effect Sequences

Create complex haptic patterns:

```cpp
void complexPull() {
  selectChannel(1 << 3);  // Up motor
  drv.begin();
  drv.selectLibrary(1);
  drv.setMode(DRV2605_MODE_INTTRIG);
  
  // Sequence of multiple effects
  drv.setWaveform(0, 47);   // Strong click
  drv.setWaveform(1, 52);   // Sharp tick
  drv.setWaveform(2, 58);   // Transition ramp down
  drv.setWaveform(3, 0);    // End
  
  drv.go();
}
```

### Diagonal Directions

Add diagonal pulling:

```cpp
void pullUpRight() {
  Serial.println("Pulling ↗ Up-Right");
  triggerMotor(3, true);   // Up strong
  triggerMotor(2, true);   // Right strong
  delay(40);
  triggerMotor(1, false);  // Down weak
  triggerMotor(4, false);  // Left weak
}
```

### Variable Intensity

Add intensity levels:

```cpp
enum PullStrength { LIGHT, MEDIUM, STRONG };

void pullWithIntensity(char direction, PullStrength strength) {
  uint8_t multiplier = (strength == LIGHT) ? 1 : (strength == MEDIUM) ? 2 : 3;
  // Adjust overdrive values based on multiplier
}
```

### Continuous vs. Single Pull

```cpp
bool continuousMode = true;  // Set to false for single-shot

void loop() {
  if (currentDirection) {
    executePull(currentDirection);
    
    if (continuousMode) {
      delay(500);  // Repeat
    } else {
      currentDirection = 0;  // Stop after one pull
    }
  }
}
```

## Technical Specifications

### DRV2605 Haptic Driver
- **Interface**: I2C (address 0x5A)
- **Supply Voltage**: 2.5V - 5.2V
- **Motor Drive**: Up to 250mA per motor
- **PWM Frequency**: 200Hz (LRA mode)
- **Effects Library**: 123 waveforms (6 libraries)
- **Resolution**: 8-bit overdrive control (0-255)

### PCA9548A Multiplexer
- **Interface**: I2C (address 0x70)
- **Channels**: 8 (using 4 in this project)
- **Supply Voltage**: 2.3V - 5.5V
- **Channel Selection**: Bitmask control

### LRA Motors
- **Type**: Linear Resonant Actuator
- **Resonant Frequency**: ~175Hz (typical)
- **Voltage**: 2-5V
- **Current**: 40-100mA (depending on model)
- **Response Time**: <10ms

### System Performance
- **Serial Baud Rate**: 115200
- **I2C Speed**: 100kHz (standard mode)
- **Pull Cycle Time**: ~540ms (40ms delay + 500ms pause)
- **Latency**: <50ms from command to haptic response

## Troubleshooting

### Motors Not Responding

**Check I2C Connections:**
```cpp
// Add to setup() for debugging
Serial.println("Scanning I2C bus...");
for (byte i = 1; i < 8; i++) {
  selectChannel(1 << i);
  Wire.beginTransmission(0x5A);  // DRV2605 address
  if (Wire.endTransmission() == 0) {
    Serial.print("Found DRV2605 on channel ");
    Serial.println(i);
  }
}
```

**Common Issues:**
- Verify all I2C connections (SDA, SCL, power, ground)
- Check pull-up resistors (4.7kΩ on SDA and SCL)
- Ensure PCA9548A address is 0x70 (check solder jumpers)
- Confirm DRV2605 boards are powered (LED should light)
- Test with I2C scanner sketch first

### Weak or No Vibration

**Motor Issues:**
- Verify LRA motors are properly connected
- Test motors with higher overdrive values (200-255)
- Check motor polarity (shouldn't matter for LRA but try swapping)
- Ensure motors are LRA type, not ERM (code uses LRA library)
- Measure motor current draw (should be 40-100mA when active)

**DRV2605 Configuration:**
```cpp
// Add debugging to triggerMotor()
Serial.print("Motor overdrive: ");
Serial.println(intensity);

// Try different waveforms
drv.setWaveform(0, 1);   // Strongest effect
```

### Inconsistent Effects

**Power Supply:**
- Use adequate power supply (500mA+ for 4 motors)
- Add decoupling capacitors (10µF + 0.1µF near each DRV2605)
- Ensure stable voltage (use multimeter to check)
- Avoid powering from USB if current draw is high

**Timing:**
- Increase delay between effects (try 100ms instead of 40ms)
- Check if motors are resonating at correct frequency

### Serial Commands Not Working

- Serial Monitor baud rate must be **115200** (not 9600!)
- Commands are single lowercase letters (u, d, l, r, s, c)
- Test command format: `t[channel][value]` with no spaces
- Check Arduino is receiving data (add Serial.print debugging)

### I2C Communication Errors

**Add Error Checking:**
```cpp
void selectChannel(uint8_t channelMask) {
  Wire.beginTransmission(PCA9548A_ADDRESS);
  Wire.write(channelMask);
  byte error = Wire.endTransmission();
  
  if (error != 0) {
    Serial.print("I2C Error: ");
    Serial.println(error);
  }
}
```

**Common I2C Issues:**
- Error 2: NACK on address (device not found)
- Error 4: Other error (check wiring, power)
- Long I2C wires (keep under 30cm, use twisted pair)

### Calibration Doesn't Feel Right

**Re-calibration Steps:**
1. Test each motor individually at overdrive 100
2. Find which motors feel too strong/weak
3. Adjust strong values (increase for weak motors, decrease for strong)
4. Test pulling directions
5. Adjust weak values for balanced counter-pulse
6. Repeat until all directions feel equal

**Typical Adjustments:**
```cpp
// If motor feels too weak:
{200, 100}  // Increase both values

// If motor feels too strong:
{80, 40}    // Decrease both values

// Maintain 2:1 ratio (strong:weak) for best pulling effect
```

## Applications

This haptic pulling system is ideal for:

- **VR/AR Gaming**: Directional damage, force feedback, environmental effects
- **Training Simulators**: Guidance systems, flight controls, medical simulation
- **Accessibility**: Navigation aids for visually impaired users
- **Art Installations**: Interactive haptic displays, sensory experiences
- **Medical Devices**: Rehabilitation guidance, surgical training
- **Wearables**: Smartwatch navigation, fitness coaching
- **Automotive**: Advanced driver assistance, parking guidance
- **Entertainment**: 4D experiences, haptic music, immersive theater

## Comparison with Other Haptic Systems

| Feature | This System | Basic Vibration | ERM Motors |
|---------|-------------|-----------------|------------|
| **Driver** | DRV2605 (smart) | Direct PWM | Direct PWM |
| **Motor Type** | LRA (resonant) | Basic vibration | Rotating mass |
| **Effects** | 123+ waveforms | ON/OFF only | ON/OFF only |
| **Precision** | Very high | Low | Medium |
| **Response Time** | <10ms | ~50ms | ~100ms |
| **Pulling Effect** | Yes | No | No |
| **Complexity** | High | Low | Medium |
| **Cost** | High (~$40 total) | Low (~$10) | Low (~$15) |
| **Use Case** | Professional | Prototypes | Consumer products |

## Bill of Materials (BOM)

| Component | Quantity | Approx. Cost | Part Number/Link |
|-----------|----------|--------------|------------------|
| DRV2605 Breakout | 4 | $7 each | Adafruit #2305 |
| PCA9548A Multiplexer | 1 | $7 | Adafruit #2717 |
| LRA Motor | 4 | $2 each | Adafruit #1201 |
| Arduino Uno/Nano | 1 | $10-25 | - |
| Jumper Wires | 1 pack | $5 | - |
| **Total** | - | **~$60** | - |

## Example Projects

### 1. VR Haptic Vest

```cpp
// Integrate with VR game engine
// Send direction based on damage location
void processDamage(float angle) {
  if (angle >= 315 || angle < 45) currentDirection = 'u';
  else if (angle >= 45 && angle < 135) currentDirection = 'r';
  else if (angle >= 135 && angle < 225) currentDirection = 'd';
  else currentDirection = 'l';
}
```

### 2. Navigation Wearable

```cpp
// GPS provides heading to destination
void updateNavigation(float bearing) {
  if (bearing > -45 && bearing <= 45) currentDirection = 'u';
  else if (bearing > 45 && bearing <= 135) currentDirection = 'r';
  else if (bearing > 135 || bearing <= -135) currentDirection = 'd';
  else currentDirection = 'l';
}
```

### 3. Meditation Breathing Guide

```cpp
void breathingCycle() {
  // Inhale - pull up sensation
  currentDirection = 'u';
  delay(4000);
  
  // Hold
  currentDirection = 's';
  delay(2000);
  
  // Exhale - pull down sensation
  currentDirection = 'd';
  delay(4000);
}
```

## Future Enhancements

### Add More Channels

PCA9548A has 8 channels - add 4 more motors for 8 directions:

```cpp
// Add diagonal motors on channels 5-8
const uint8_t upRight_channel = 5;
const uint8_t upLeft_channel = 6;
const uint8_t downRight_channel = 7;
const uint8_t downLeft_channel = 8;
```

### Bluetooth/WiFi Control

```cpp
#include <BluetoothSerial.h>
BluetoothSerial BT;

void loop() {
  if (BT.available()) {
    char cmd = BT.read();
    // Process command
  }
}
```

### Pattern Sequencing

```cpp
void playPattern(char pattern[]) {
  for (int i = 0; pattern[i] != '\0'; i++) {
    currentDirection = pattern[i];
    executePull();
    delay(500);
  }
}

// Usage
playPattern("udlr");  // Circle pattern
```

### Intensity Ramping

```cpp
void rampPull(char direction, uint8_t startIntensity, uint8_t endIntensity) {
  for (uint8_t i = startIntensity; i <= endIntensity; i += 10) {
    // Gradually increase pulling force
  }
}
```

## Resources

- [DRV2605 Datasheet](https://www.ti.com/lit/ds/symlink/drv2605.pdf)
- [DRV2605 Effect Library Guide](https://www.ti.com/lit/an/sloa189a/sloa189a.pdf)
- [Adafruit DRV2605 Library](https://github.com/adafruit/Adafruit_DRV2605_Library)
- [PCA9548A Datasheet](https://www.ti.com/lit/ds/symlink/pca9548a.pdf)
- [I2C Multiplexer Tutorial](https://learn.adafruit.com/adafruit-tca9548a-1-to-8-i2c-multiplexer-breakout)

## License

MIT License - feel free to use and modify for your projects.

## Credits

- Adafruit Industries for DRV2605 library and breakout boards
- Texas Instruments for DRV2605 haptic driver IC
- NXP Semiconductors for PCA9548A multiplexer

## Version History

- **v1.0**: Initial release with 4-direction pulling effect
- Professional DRV2605-based haptic control
- Calibrated overdrive for balanced motors
- Testing and calibration commands

---

**Note**: This is a professional-grade haptic system. Proper calibration is essential for optimal user experience. Take time to tune each motor for your specific hardware.