# Phone Case Directional Haptics Navigation System

Advanced haptic feedback system using DRV2605 drivers with TCA9548A I2C multiplexer for phone case navigation. Features real-time asymmetric waveform generation for smooth, continuous directional feedback.

## Hardware Requirements

### Essential Components
- **Arduino Board** (Uno, Nano, Mega, or ESP32)
- **4x Adafruit DRV2605 Haptic Motor Controllers**
- **1x TCA9548A I2C Multiplexer** (8-channel)
- **4x LRA (Linear Resonant Actuator) Haptic Motors**
  - Recommended: Adafruit Product ID 1201 or similar
  - Coin-style LRA motors (8-12mm diameter)
- **I2C Pull-up Resistors** (4.7kΩ, often built into modules)
- **Power Supply**: 3-5V for motors

### Why This Hardware?

**DRV2605 in Real-Time Mode:**
- Generates custom waveforms on-the-fly
- 127 intensity levels (0-127)
- Precise control over vibration patterns
- Smooth, continuous haptic feedback
- I2C control interface

**TCA9548A Multiplexer:**
- All DRV2605 chips share I2C address 0x5A
- Multiplexer enables control of 4 identical devices
- 8 channels available (using 4 for this project)

## Pin Configuration

### I2C Connections

```
Arduino SDA  → TCA9548A SDA → All DRV2605 SDA pins
Arduino SCL  → TCA9548A SCL → All DRV2605 SCL pins
5V           → TCA9548A VIN → All DRV2605 VIN pins
GND          → TCA9548A GND → All DRV2605 GND pins
```

### Motor Channel Assignment

```
TCA9548A Channel 0 → DRV2605 #1 → UP Motor (Forward)
TCA9548A Channel 1 → DRV2605 #2 → DOWN Motor (Backward)
TCA9548A Channel 2 → DRV2605 #3 → LEFT Motor
TCA9548A Channel 3 → DRV2605 #4 → RIGHT Motor
```

### Physical Motor Layout (Phone Case)

```
           ┌─────────────┐
           │      ●0     │   (Channel 0: Forward/Up)
           │   (FORWARD) │
           │             │
      ●2   │             │   ●3
    (LEFT) │             │  (RIGHT)
           │             │
           │  (BACKWARD) │
           │      ●1     │   (Channel 1: Backward/Down)
           └─────────────┘
           
           Phone held in portrait mode
```

## Features

- **Real-Time Waveform Generation**: Custom asymmetric sine waves at 75Hz
- **8-Direction Navigation**: Cardinal + 4 diagonal directions
- **Continuous Haptic Feedback**: 1000Hz update rate for smooth sensations
- **GPS Integration Ready**: Pre-built navigation command processor
- **Bluetooth Command Support**: Parse navigation commands from mobile apps
- **Dynamic Intensity Control**: Adjust vibration strength in real-time
- **Pattern Modes**: Simultaneous or sequential motor activation
- **Motor Testing**: Built-in test routine for all motors
- **System Status Display**: Real-time monitoring of all motor states

## Installation

### 1. Install Required Libraries

In Arduino IDE:
```
Tools → Manage Libraries → Install:
- Adafruit DRV2605 Library
- Wire Library (pre-installed)
```

### 2. Hardware Assembly

**Step 1: Connect TCA9548A to Arduino**
```
Arduino 5V  → TCA9548A VIN
Arduino GND → TCA9548A GND
Arduino SDA → TCA9548A SDA (A4 on Uno, 21 on ESP32)
Arduino SCL → TCA9548A SCL (A5 on Uno, 22 on ESP32)
```

**Step 2: Connect DRV2605 Boards**
```
TCA9548A SD0 → DRV2605 #1 SDA (Forward motor)
TCA9548A SD1 → DRV2605 #2 SDA (Backward motor)
TCA9548A SD2 → DRV2605 #3 SDA (Left motor)
TCA9548A SD3 → DRV2605 #4 SDA (Right motor)

TCA9548A SC0-SC3 → All DRV2605 SCL pins
5V Power Bus      → All DRV2605 VIN pins
GND Bus           → All DRV2605 GND pins
```

**Step 3: Connect Motors**
```
Each DRV2605 → LRA motor (polarity doesn't matter)
```

### 3. Upload Code

1. Open sketch in Arduino IDE
2. Select board and port
3. Upload code
4. Open Serial Monitor (115200 baud)

## Usage

### Navigation Commands

Open Serial Monitor at **115200 baud** and enter:

**Cardinal Directions:**
- `f` - **Forward** (up motor)
- `b` - **Backward** (down motor)
- `l` - **Left** (left motor)
- `r` - **Right** (right motor)

**Diagonal Directions:**
- `1` - **Forward-Left** (up + left motors)
- `2` - **Forward-Right** (up + right motors)
- `3` - **Backward-Left** (down + left motors)
- `4` - **Backward-Right** (down + right motors)

**Control Commands:**
- `s` - **Stop** all motors
- `t` - **Test** all motors sequentially
- `+` - **Increase** intensity (+15)
- `-` - **Decrease** intensity (-15)
- `p` - **Print** system status
- `q` - **Toggle** pattern mode (simultaneous/sequential)

### Real-Time Waveform Generation

This system generates **asymmetric sine waves** at 75Hz for natural haptic feedback:

```
Normal Sine Wave:
    ╱─╲
   ╱   ╲
  ╱     ╲
 ╱       ╲

Asymmetric Wave (This System):
    ╱╲
   ╱  ╲____
  ╱       ╲____
 ╱            ╲

30% rise time (sharp attack)
70% fall time (gradual decay)
= More directionally perceptible
```

**Why Asymmetric?**
- Creates sharper, more noticeable directional cues
- Better perceived by human tactile receptors
- Feels more "directional" than symmetric waves

### Update Rate

The system updates haptic feedback at **1000Hz** (every 1ms):
- Smooth, continuous vibration (not pulsed)
- Phase-accurate waveform generation
- No perceptible "stepping" in intensity

### Intensity Control

**Default intensity:** 100  
**Range:** 10-127 (DRV2605 real-time mode limit)

```
> +
Intensity: 115

> +
Intensity: 127 (max)

> -
Intensity: 112
```

Intensity scales all active motors proportionally.

### Pattern Modes

**Simultaneous Mode (Default):**
- All motors for a direction fire together
- Best for directional navigation
- Example: Forward = up motor vibrates continuously

**Sequential Mode:**
- Motors fire in sequence (currently not fully implemented in main loop)
- Good for attention-getting patterns
- Toggle with `q` command

## GPS Integration

Built-in GPS navigation command processor:

```cpp
processGPSDirection("TURN_LEFT");      // Activate left motor
processGPSDirection("TURN_RIGHT");     // Activate right motor
processGPSDirection("GO_STRAIGHT");    // Activate forward motor
processGPSDirection("SLIGHT_LEFT");    // Forward-left diagonal
processGPSDirection("SLIGHT_RIGHT");   // Forward-right diagonal
processGPSDirection("U_TURN");         // Activate backward motor
processGPSDirection("ARRIVED");        // Stop all motors
```

**Integration Example:**
```cpp
// In your GPS navigation code
void updateNavigation() {
  String nextDirection = getGPSDirection();
  processGPSDirection(nextDirection);
}
```

## Bluetooth Command Protocol

Parse navigation commands from mobile apps:

**Command Format:**
```
NAV:[DIRECTION]    - Set navigation direction
INT:[VALUE]        - Set intensity (10-127)
```

**Examples:**
```cpp
parseBluetoothCommand("NAV:TURN_LEFT");
parseBluetoothCommand("NAV:GO_STRAIGHT");
parseBluetoothCommand("INT:80");
```

**Add to your code:**
```cpp
#include <BluetoothSerial.h>
BluetoothSerial BT;

void setup() {
  BT.begin("PhoneCaseNav");
  // ... rest of setup
}

void loop() {
  if (BT.available()) {
    String cmd = BT.readString();
    parseBluetoothCommand(cmd);
  }
  // ... rest of loop
}
```

## Code Parameters

### Waveform Settings

```cpp
const float BASE_FREQUENCY = 75.0;      // Vibration frequency (Hz)
const float ASYMMETRY_FACTOR = 0.3;     // 30% rise, 70% fall
const int SAMPLE_RATE = 1000;           // 1000 updates per second
```

**Frequency Adjustment:**
```cpp
const float BASE_FREQUENCY = 60.0;   // Slower, deeper feel
const float BASE_FREQUENCY = 90.0;   // Faster, sharper feel
```

**Asymmetry Adjustment:**
```cpp
const float ASYMMETRY_FACTOR = 0.5;  // Symmetric (50/50)
const float ASYMMETRY_FACTOR = 0.2;  // Very sharp attack (20/80)
const float ASYMMETRY_FACTOR = 0.4;  // Balanced sharp (40/60)
```

### Direction Amplitudes

```cpp
// Cardinal directions (single motor)
currentAmplitudes[MOTOR_UP] = 120;     // Forward

// Diagonal directions (two motors)
currentAmplitudes[MOTOR_UP] = 90;      // Forward-left
currentAmplitudes[MOTOR_LEFT] = 90;    // (both at ~70% strength)
```

**Why lower for diagonals?**
- Prevents overwhelming sensation
- Balances total haptic energy
- Maintains directional clarity

## Advanced Customization

### Add More Directions

```cpp
enum Direction {
  // ... existing directions
  DIR_CIRCLE_CW = 9,
  DIR_CIRCLE_CCW = 10
};

void createCirclePattern() {
  static const int sequence[] = {MOTOR_UP, MOTOR_RIGHT, MOTOR_DOWN, MOTOR_LEFT};
  static int step = 0;
  
  for (int i = 0; i < 4; i++) currentAmplitudes[i] = 0;
  currentAmplitudes[sequence[step]] = 120;
  
  step = (step + 1) % 4;
}
```

### Variable Frequency

```cpp
void setFrequency(float freq) {
  BASE_FREQUENCY = freq;
  // Waveform automatically adjusts
}

// Usage:
setFrequency(60.0);  // Low rumble
setFrequency(100.0); // Sharp buzz
```

### Custom Waveforms

Replace `generateAsymmetricWave()` with:

```cpp
// Square wave
int generateSquareWave(float phase) {
  return (fmod(phase, TWO_PI) < PI) ? 127 : 0;
}

// Triangle wave
int generateTriangleWave(float phase) {
  float normalized = fmod(phase, TWO_PI) / TWO_PI;
  if (normalized < 0.5) {
    return (int)(normalized * 2.0 * 127);
  } else {
    return (int)((2.0 - normalized * 2.0) * 127);
  }
}
```

### Intensity Ramping

```cpp
void rampIntensity(int startAmp, int endAmp, int durationMs) {
  int steps = durationMs;
  float increment = (float)(endAmp - startAmp) / steps;
  
  for (int i = 0; i < steps; i++) {
    int amp = startAmp + (int)(increment * i);
    for (int m = 0; m < 4; m++) {
      if (currentAmplitudes[m] > 0) {
        currentAmplitudes[m] = amp;
      }
    }
    delay(1);
  }
}
```

## Serial Monitor Output

```
Phone Case Directional Haptics System
Using TCA9548A I2C Multiplexer with DRV2605 drivers
Motor initialized on channel 0
Motor initialized on channel 1
Motor initialized on channel 2
Motor initialized on channel 3
System ready!
Navigation Commands:
'f' - Forward, 'b' - Backward, 'l' - Left, 'r' - Right
'1' - Forward-Left, '2' - Forward-Right
'3' - Backward-Left, '4' - Backward-Right
's' - Stop, 't' - Test all motors
'+' - Increase intensity, '-' - Decrease intensity

> f
Direction: Forward

> 2
Direction: Forward-Right

> +
Intensity: 115

> p
=== Phone Case Navigation System Status ===
Current Direction: Forward-Right
Motor States:
  UP (Ch0): 103/127
  DOWN (Ch1): 0/127
  LEFT (Ch2): 0/127
  RIGHT (Ch3): 103/127
```

## Technical Specifications

### System Performance
- **Update Rate**: 1000Hz (1ms per cycle)
- **Waveform Frequency**: 75Hz (adjustable)
- **Latency**: <10ms from command to haptic output
- **Intensity Levels**: 118 (10-127)
- **Directions**: 8 (4 cardinal + 4 diagonal)

### Hardware Specs
- **DRV2605**: I2C address 0x5A, real-time mode
- **TCA9548A**: I2C address 0x70
- **I2C Speed**: 100kHz (standard mode)
- **Serial Baud**: 115200
- **Motor Drive**: Up to 250mA per DRV2605
- **Supply Voltage**: 2.5V - 5.2V

### Memory Usage
- **RAM**: ~500 bytes (phase tracking + amplitudes)
- **Flash**: ~12KB (with all functions)
- **Arduino Uno**: Compatible 
- **Arduino Nano**: Compatible 
- **ESP32**: Compatible 

## Troubleshooting

### Motors Not Responding

**I2C Communication Check:**
```cpp
// Add to setup() for debugging
void scanI2C() {
  Serial.println("Scanning I2C bus...");
  for (byte ch = 0; ch < 8; ch++) {
    selectTCAChannel(ch);
    Wire.beginTransmission(0x5A);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found DRV2605 on channel ");
      Serial.println(ch);
    }
  }
}
```

**Common Issues:**
- Verify I2C connections (SDA, SCL, power, ground)
- Check 4.7kΩ pull-up resistors on SDA and SCL
- Confirm TCA9548A address is 0x70
- Test with I2C scanner first
- Ensure all DRV2605 boards are powered

### Weak or No Vibration

**Amplitude Issues:**
```cpp
// Increase base amplitude
currentAmplitudes[MOTOR_UP] = 127;  // Max amplitude

// Check frequency matches motor resonance
const float BASE_FREQUENCY = 175.0;  // Try motor's resonant frequency
```

**Motor Compatibility:**
- Ensure motors are LRA type (not ERM)
- Check motor resonant frequency (~175Hz typical)
- Verify motor connections (+ and - terminals)
- Test with simple waveform first

### Jittery or Inconsistent Vibration

**Timing Issues:**
- Check if `SAMPLE_RATE` is too high for your Arduino
- Reduce to 500Hz if using Arduino Uno
- Use ESP32 for stable 1000Hz updates

**Power Supply:**
- Use adequate power (500mA+ for 4 motors)
- Add 10µF + 0.1µF capacitors near each DRV2605
- Ensure stable voltage under load

### Serial Commands Not Working

- Baud rate must be **115200** (not 9600!)
- Commands are single characters (lowercase)
- No line ending required for single commands
- Check Arduino is receiving data

### High CPU Usage

```cpp
// Reduce sample rate if Arduino struggles
const int SAMPLE_RATE = 500;  // 500Hz instead of 1000Hz

// Or use larger update interval
if (micros() - lastUpdate >= 2000) {  // 2ms = 500Hz
```

## Applications

This phone case navigation system is perfect for:

- **Pedestrian Navigation**: Turn-by-turn directions without looking at screen
- **Cycling Navigation**: Safe directional cues while riding
- **Running/Jogging**: Haptic guidance during exercise
- **Accessibility**: Navigation for visually impaired users
- **Mobile Gaming**: Directional feedback for games
- **In-Car Navigation**: Haptic backup for GPS directions
- **Urban Exploration**: Silent navigation in quiet areas
- **AR Applications**: Directional hints for augmented reality

## Comparison with Other Haptic Systems

| Feature | This System | Preset Effects | Basic Vibration |
|---------|-------------|----------------|-----------------|
| **Waveform** | Custom asymmetric | 123 presets | ON/OFF |
| **Update Rate** | 1000Hz continuous | Event-driven | Event-driven |
| **Smoothness** | Very smooth | Discrete clicks | Rough |
| **Directions** | 8 (with diagonals) | Varies | Limited |
| **Intensity Levels** | 118 (10-127) | Varies | Limited |
| **Feel** | Fluid, wave-like | Sharp clicks | Basic buzz |
| **Complexity** | High | Medium | Low |
| **Power Usage** | Medium-High | Low-Medium | Low |
| **Best For** | Navigation | Notifications | Alerts |

## Example Integrations

### Google Maps Integration

```cpp
// Parse Google Maps directions API
void processGoogleMapsDirection(String instruction) {
  if (instruction.contains("Turn left")) {
    setDirectionalPattern(DIR_LEFT);
  } else if (instruction.contains("Turn right")) {
    setDirectionalPattern(DIR_RIGHT);
  } else if (instruction.contains("slight left")) {
    setDirectionalPattern(DIR_FORWARD_LEFT);
  } else if (instruction.contains("slight right")) {
    setDirectionalPattern(DIR_FORWARD_RIGHT);
  } else if (instruction.contains("Continue")) {
    setDirectionalPattern(DIR_FORWARD);
  } else if (instruction.contains("arrived")) {
    setDirectionalPattern(DIR_STOP);
  }
}
```

### Fitness App Integration

```cpp
// Wearable fitness guidance
void fitnessGuidance(String workout) {
  if (workout == "SPEED_UP") {
    setDirectionalPattern(DIR_FORWARD);
    // Forward = increase pace
  } else if (workout == "SLOW_DOWN") {
    setDirectionalPattern(DIR_BACKWARD);
    // Backward = decrease pace
  } else if (workout == "TURN_AROUND") {
    // Alternate left/right for attention
    setDirectionalPattern(DIR_LEFT);
    delay(200);
    setDirectionalPattern(DIR_RIGHT);
  }
}
```

### AR Game Integration

```cpp
// Augmented reality directional hints
void arDirectionHint(float angle) {
  // Convert 360° angle to 8 directions
  int sector = (int)((angle + 22.5) / 45.0) % 8;
  
  Direction dirs[] = {
    DIR_FORWARD, DIR_FORWARD_RIGHT, DIR_RIGHT, DIR_BACKWARD_RIGHT,
    DIR_BACKWARD, DIR_BACKWARD_LEFT, DIR_LEFT, DIR_FORWARD_LEFT
  };
  
  setDirectionalPattern(dirs[sector]);
}
```

## Bill of Materials (BOM)

| Component | Quantity | Approx. Cost | Part Number |
|-----------|----------|--------------|-------------|
| DRV2605 Breakout | 4 | $7 each | Adafruit #2305 |
| TCA9548A Multiplexer | 1 | $7 | Adafruit #2717 |
| LRA Motor | 4 | $2 each | Adafruit #1201 |
| Arduino Nano | 1 | $10-15 | - |
| Jumper Wires | 1 pack | $5 | - |
| Phone Case | 1 | Varies | Custom/3D printed |
| **Total** | - | **~$60-65** | - |

## Future Enhancements

### Pattern Library

```cpp
// Pre-programmed navigation patterns
void loadPatternLibrary() {
  patterns[0] = createAlertPattern();
  patterns[1] = createWarningPattern();
  patterns[2] = createSuccessPattern();
}
```

### Machine Learning Integration

```cpp
// Adapt haptics to user preferences
void learnUserPreferences() {
  // Track which directions user responds to fastest
  // Adjust intensity accordingly
}
```

### Battery Optimization

```cpp
// Reduce power consumption
void enterLowPowerMode() {
  // Lower sample rate when battery is low
  SAMPLE_RATE = 250;  // 250Hz saves power
}
```

### Multi-User Profiles

```cpp
// Store preferences per user
struct UserProfile {
  int baseIntensity;
  float preferredFrequency;
  bool useSequential;
};
```

## Resources

- [DRV2605 Datasheet](https://www.ti.com/lit/ds/symlink/drv2605.pdf)
- [TCA9548A Datasheet](https://www.ti.com/lit/ds/symlink/tca9548a.pdf)
- [Adafruit DRV2605 Guide](https://learn.adafruit.com/adafruit-drv2605-haptic-controller-breakout)
- [I2C Multiplexer Tutorial](https://learn.adafruit.com/adafruit-tca9548a-1-to-8-i2c-multiplexer-breakout)
- [LRA Motor Guide](https://www.ti.com/lit/an/sloa189a/sloa189a.pdf)

## License

MIT License - free to use and modify for your projects.

## Credits

- Adafruit Industries for DRV2605 library and hardware
- Texas Instruments for DRV2605 haptic driver IC
- NXP Semiconductors for TCA9548A multiplexer

## Version History

- **v1.0**: Initial release with real-time waveform generation
- Asymmetric sine wave at 75Hz
- 8-direction navigation support
- GPS and Bluetooth integration ready
- 1000Hz update rate for smooth feedback

---

**Note**: This is an advanced haptic system requiring proper hardware setup. Real-time mode provides smooth, continuous feedback ideal for navigation applications. Test thoroughly before deploying in wearable devices.