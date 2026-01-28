# Sequential Wave Haptic Feedback System

A directional haptic feedback system that uses a sophisticated "wave" pattern to create realistic directional pull sensations. This system employs 4 motors with sequential ramping intensities to produce intuitive directional cues.

## Features

- **4-Directional Feedback**: Up, down, left, and right directional pulls
- **Sequential Wave Pattern**: Motors fire in sequence with ramping intensity
- **Counterintuitive Design**: Strongest vibration on opposite side creates pull sensation
- **Simple PWM Control**: Direct motor control without additional drivers
- **Text-Based Commands**: Easy serial interface for direction control
- **Lightweight Code**: Minimal dependencies, fast response

## Hardware Requirements

### Required Components

- **Microcontroller**: ESP32, Arduino Uno/Nano/Mega, or compatible board with PWM support
- **Motors**: 4x vibration motors (coin motors or small ERM motors)
- **Motor Drivers**: 
  - Option 1: 4x NPN transistors (2N2222, BC547, or similar)
  - Option 2: Motor driver IC (ULN2003, L293D, or similar)
- **Resistors**: 4x 1kΩ resistors (for transistor base, if using transistors)
- **Power Supply**: Appropriate for your motors (typically 3-5V)
- **Diodes**: 4x 1N4001 or similar (flyback protection)

### Motor Specifications

Recommended motor types:
- **Coin vibration motors**: 3V, 60-80mA
- **Small ERM motors**: 3-5V, 50-100mA
- **Pancake motors**: 3V, compact size

## Pin Configuration

| Motor Position | GPIO Pin | Array Index |
|---------------|----------|-------------|
| Up (M1)       | 4        | motors[0]   |
| Right (M2)    | 12       | motors[1]   |
| Down (M3)     | 6        | motors[2]   |
| Left (M4)     | 2        | motors[3]   |

### Pin Requirements

Your microcontroller pins must support PWM (Pulse Width Modulation):
- **ESP32**: All GPIO pins support PWM
- **Arduino Uno**: Pins 3, 5, 6, 9, 10, 11
- **Arduino Mega**: Pins 2-13, 44-46

## Wiring Guide

### Option 1: Using NPN Transistors (Recommended for Beginners)

For each motor:

```
ESP32/Arduino GPIO Pin → 1kΩ Resistor → Transistor Base (pin 1)
Motor (+) → Power Supply (+) [3-5V]
Motor (-) → Transistor Collector (pin 2)
Transistor Emitter (pin 3) → Ground
Diode (cathode/band) → Motor (+)
Diode (anode) → Motor (-)
Microcontroller Ground → Power Supply Ground (COMMON GROUND)
```

**Transistor Pinout (2N2222 / BC547):**
```
    [Collector]
        |
    [Base] ← Resistor ← GPIO Pin
        |
    [Emitter] → Ground
```

### Option 2: Using ULN2003 Darlington Array

```
ULN2003 Connections:
Pin 1-4 (Inputs)  → GPIO Pins 4, 12, 6, 2
Pin 8 (Ground)    → Ground
Pin 9 (COM)       → Motor Power Supply (+)
Pin 13-16 (Out)   → Motors (-)
Motors (+)        → Motor Power Supply (+)
```

### Option 3: Using L293D Motor Driver

```
L293D Connections:
Enable 1,2 (Pin 1)  → 5V or GPIO (enable control)
Input 1-4 (2,7,10,15) → GPIO Pins 4, 12, 6, 2
Output 1-4 (3,6,11,14) → Motors
VSS (Pin 8)         → Motor Power (4.5-36V)
VS (Pin 16)         → Logic Power (5V)
Ground (4,5,12,13)  → Ground
```

### Complete Wiring Diagram (Transistor Method)

```
                    +3-5V Motor Power
                         |
    ┌────────────────────┴────────────────────┐
    |                    |                     |
    |                [Motor 1 - UP]       [Motor 2 - RIGHT]
    |                    |                     |
    |                  [Diode]              [Diode]
    |                    |                     |
    |              [Transistor]          [Transistor]
    |                    |                     |
Pin 4 ──[1kΩ]───────────┘                     |
                         |                     |
Pin 12 ──[1kΩ]───────────────────────────────┘
                         |
                       Ground
(Same pattern for Motor 3 - Down on Pin 6 and Motor 4 - Left on Pin 2)

ESP32/Arduino Ground ───┴─── Motor Power Ground (COMMON GROUND!)
```

## Installation

1. **Wire Hardware**: Follow one of the wiring guides above
2. **Open Arduino IDE**: Load the `.ino` file
3. **Select Board**: Choose your microcontroller from Tools → Board
4. **Select Port**: Choose correct COM port from Tools → Port
5. **Upload Code**: Click upload button
6. **Open Serial Monitor**: Set baud rate to **115200**

## Usage

### Serial Commands

Open Serial Monitor at **115200 baud** with line ending set to "Newline" or "Both NL & CR".

| Command | Action | Effect Description |
|---------|--------|-------------------|
| `up` | Pull Up | Wave from down→left/right→up, strongest at down |
| `down` | Pull Down | Wave from up→left/right→down, strongest at up |
| `left` | Pull Left | Wave from right→down/up→left, strongest at right |
| `right` | Pull Right | Wave from left→down/up→right, strongest at left |
| `stop` | Stop All | Turns off all motors immediately |

### Example Session

```
Ready. Send: up, down, left, right, stop

> right
Received: right
[Motors fire: Up(weak) → Right(medium) → Down(strong) → Left(strongest)]

> up
Received: up
[Motors fire: Down(weak) → Right(medium) → Left(strong) → Up(strongest)]

> stop
Received: stop
All motors stopped.
```

## How It Works

### The Sequential Wave Pattern

This system uses a counterintuitive but highly effective approach:

**To pull RIGHT:**
```cpp
pulse(motors[0], 100, 30); delay(20);  // Up:    weak (100/255)
pulse(motors[1], 180, 30); delay(20);  // Right: medium (180/255)
pulse(motors[2], 220, 30); delay(20);  // Down:  strong (220/255)
pulse(motors[3], 255, 30);             // Left:  strongest (255/255)
```

### Why Opposite Side is Strongest?

The brain interprets the **strongest vibration on the opposite side** as a force pulling you toward the target direction:

- **Pull Right** → Left motor (255) strongest = sensation of pulling right
- **Pull Up** → Down motor (255) strongest = sensation of pulling up

This mimics how we perceive physical forces - resistance on one side creates the feeling of movement toward the opposite side.

### Wave Timing Breakdown

```
Total duration: ~150ms per direction command

Motor 1:  30ms pulse + 20ms gap = 50ms
Motor 2:  30ms pulse + 20ms gap = 50ms
Motor 3:  30ms pulse + 20ms gap = 50ms
Motor 4:  30ms pulse (final)    = 30ms
                                  ------
                                  180ms total
```

### Intensity Ramping

```
Motor Sequence:  1st → 2nd → 3rd → 4th (target direction)
Intensity:       100 → 180 → 220 → 255 (peak at opposite side)
Duty Cycle:      39% → 71% → 86% → 100%
```

## Configuration

### Adjusting Motor Positions

If your motors are wired to different pins, update the array:

```cpp
const int motors[4] = {4, 12, 6, 2};  // Change to your pins
//                     ↑  ↑   ↑  ↑
//                     Up Rt  Dn Lt
```

### Tuning Wave Parameters

**Pulse Duration** (how long each motor vibrates):
```cpp
void pulse(int pin, int strength, int duration) {
  analogWrite(pin, strength);
  delay(duration);  // Change from 30 to your preferred duration
  analogWrite(pin, 0);
}
```
- Shorter (10-20ms): Sharper, more distinct taps
- Longer (50-100ms): Smoother, more sustained feel

**Gap Between Motors** (wave speed):
```cpp
pulse(motors[0], 100, 30); 
delay(20);  // Change this gap
pulse(motors[1], 180, 30);
```
- Shorter gaps (5-10ms): Faster wave, more urgent
- Longer gaps (30-50ms): Slower wave, more deliberate

**Intensity Ramp** (strength progression):
```cpp
void pullRight() {
  pulse(motors[0], 100, 30); // Weakest - adjust these values
  pulse(motors[1], 180, 30); // Medium-weak
  pulse(motors[2], 220, 30); // Medium-strong
  pulse(motors[3], 255, 30); // Strongest (opposite side)
}
```

Intensity guidelines:
- Keep 100-140 range for subtle effects
- Use 200-255 for strong, noticeable effects
- Maintain gradual progression for smooth wave

### Custom Intensity Profiles

**Linear Ramp:**
```cpp
pulse(motors[0], 100, 30);  // Equal steps
pulse(motors[1], 150, 30);  // +50
pulse(motors[2], 200, 30);  // +50
pulse(motors[3], 255, 30);  // +55
```

**Exponential Ramp:**
```cpp
pulse(motors[0], 64, 30);   // 25%
pulse(motors[1], 128, 30);  // 50%
pulse(motors[2], 192, 30);  // 75%
pulse(motors[3], 255, 30);  // 100%
```

**Gentle Ramp:**
```cpp
pulse(motors[0], 150, 30);  // Start higher
pulse(motors[1], 180, 30);  // Smaller steps
pulse(motors[2], 210, 30);
pulse(motors[3], 240, 30);  // Less contrast
```

## Advanced Features

### Adding Diagonal Directions

Add these functions for 8-way directional control:

```cpp
void pullUpRight() {
  pulse(motors[2], 100, 30); delay(20);  // Down: weak
  pulse(motors[3], 150, 30); delay(20);  // Left: medium-weak
  pulse(motors[0], 200, 30); delay(20);  // Up: medium-strong
  pulse(motors[1], 255, 30);             // Right: strongest
}

void pullUpLeft() {
  pulse(motors[2], 100, 30); delay(20);  // Down: weak
  pulse(motors[1], 150, 30); delay(20);  // Right: medium-weak
  pulse(motors[0], 200, 30); delay(20);  // Up: medium-strong
  pulse(motors[3], 255, 30);             // Left: strongest
}

void pullDownRight() {
  pulse(motors[0], 100, 30); delay(20);  // Up: weak
  pulse(motors[3], 150, 30); delay(20);  // Left: medium-weak
  pulse(motors[2], 200, 30); delay(20);  // Down: medium-strong
  pulse(motors[1], 255, 30);             // Right: strongest
}

void pullDownLeft() {
  pulse(motors[0], 100, 30); delay(20);  // Up: weak
  pulse(motors[1], 150, 30); delay(20);  // Right: medium-weak
  pulse(motors[2], 200, 30); delay(20);  // Down: medium-strong
  pulse(motors[3], 255, 30);             // Left: strongest
}
```

Update `loop()` to handle new commands:
```cpp
if (command == "up-right" || command == "upright") pullUpRight();
else if (command == "up-left" || command == "upleft") pullUpLeft();
// ... etc
```

### Continuous vs Single-Shot Mode

**Current Behavior**: Single wave per command

**For Continuous Mode** (repeats until stopped):
```cpp
bool isActive = false;
String activeDirection = "";

void loop() {
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command == "stop") {
      isActive = false;
      stopAll();
    } else {
      activeDirection = command;
      isActive = true;
    }
  }
  
  if (isActive) {
    if (activeDirection == "right") pullRight();
    else if (activeDirection == "left") pullLeft();
    // ... etc
    delay(100); // Gap between repetitions
  }
}
```

### Intensity Control via Serial

Add dynamic intensity adjustment:

```cpp
int globalIntensity = 255; // Max intensity

void loop() {
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    command.trim();
    
    // Handle intensity commands: "intensity 180"
    if (command.startsWith("intensity ")) {
      globalIntensity = command.substring(10).toInt();
      globalIntensity = constrain(globalIntensity, 50, 255);
      Serial.println("Intensity set to: " + String(globalIntensity));
      return;
    }
    // ... rest of command handling
  }
}

void pullRight() {
  pulse(motors[0], map(100, 0, 255, 0, globalIntensity), 30); delay(20);
  pulse(motors[1], map(180, 0, 255, 0, globalIntensity), 30); delay(20);
  pulse(motors[2], map(220, 0, 255, 0, globalIntensity), 30); delay(20);
  pulse(motors[3], globalIntensity, 30);
}
```

### Bluetooth Control (ESP32 Only)

Add wireless control:

```cpp
#include <BluetoothSerial.h>
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("HapticVest"); // Bluetooth device name
  // ... rest of setup
}

void loop() {
  // Check both Serial and Bluetooth
  if (Serial.available() || SerialBT.available()) {
    if (Serial.available()) {
      command = Serial.readStringUntil('\n');
    } else {
      command = SerialBT.readStringUntil('\n');
    }
    // ... rest of loop
  }
}
```

## Troubleshooting

### Motors Not Vibrating

**Check Power Supply:**
- Verify motor voltage rating (typically 3-5V)
- Ensure sufficient current capacity (300-400mA for 4 motors)
- Confirm common ground between microcontroller and motor power

**Verify Wiring:**
- Test transistor connections with multimeter
- Check for reversed motor polarity (shouldn't matter for vibration motors, but verify)
- Ensure flyback diodes are oriented correctly (band toward positive)

**Test Individual Motors:**
```cpp
void setup() {
  // ... existing setup
  
  // Test each motor
  for (int i = 0; i < 4; i++) {
    analogWrite(motors[i], 200);
    delay(500);
    analogWrite(motors[i], 0);
    delay(500);
  }
}
```

### Weak Vibration

**Increase PWM Values:**
```cpp
// Change from:
pulse(motors[0], 100, 30);

// To:
pulse(motors[0], 150, 30);  // Higher starting point
```

**Check Voltage:**
- Motors may be under-powered
- Verify power supply voltage matches motor rating
- Use separate power supply for motors (not USB power)

**Verify Transistor Saturation:**
- Base resistor might be too high (try 470Ω instead of 1kΩ)
- Transistor might be weak (try 2N2222A or TIP120 for more current)

### Only Some Motors Work

**Identify Pattern:**
```cpp
void setup() {
  Serial.begin(115200);
  
  // Test each motor individually
  Serial.println("Testing Motor 1 (Up) - Pin 4");
  analogWrite(motors[0], 255); delay(1000); analogWrite(motors[0], 0);
  
  Serial.println("Testing Motor 2 (Right) - Pin 12");
  analogWrite(motors[1], 255); delay(1000); analogWrite(motors[1], 0);
  
  Serial.println("Testing Motor 3 (Down) - Pin 6");
  analogWrite(motors[2], 255); delay(1000); analogWrite(motors[2], 0);
  
  Serial.println("Testing Motor 4 (Left) - Pin 2");
  analogWrite(motors[3], 255); delay(1000); analogWrite(motors[3], 0);
  
  Serial.println("Test complete");
}
```

**Common Causes:**
- Specific GPIO pins may not support PWM (check your board's pinout)
- Individual transistor/motor failures
- Loose connections on specific channels

### Direction Feels Backwards

This can happen due to motor placement. Simply swap motor assignments:

```cpp
// If "right" feels like "left", swap these:
const int motors[4] = {4, 2, 6, 12};  // Swapped positions 1 and 3
//                     ↑  ↑  ↑  ↑
//                     Up Lt Dn Rt (corrected)
```

### Serial Commands Not Responding

**Verify Baud Rate:**
- Ensure Serial Monitor is set to **115200**

**Check Line Ending:**
- Set to "Newline" or "Both NL & CR"

**Test Connection:**
```cpp
void loop() {
  if (Serial.available()) {
    Serial.println("Received: " + Serial.readStringUntil('\n'));
  }
}
```

## Applications

### Navigation & Wayfinding
- **GPS Navigation**: Haptic turn-by-turn directions
- **Indoor Navigation**: Museum/airport guidance without looking at phone
- **Hiking/Outdoor**: Compass-like directional cues

### Gaming & Entertainment
- **VR/AR**: Directional damage indicators, environmental cues
- **Racing Games**: Track direction, optimal racing line
- **Flight Simulators**: Wind direction, aircraft orientation

### Accessibility
- **Visual Impairment**: Non-visual navigation and alerts
- **Hearing Impairment**: Directional alert system
- **Elderly Care**: Fall prevention directional cues

### Sports & Fitness
- **Cycling**: Navigation without looking away from road
- **Running**: Pace/direction coaching
- **Team Sports**: Tactical positioning feedback

### Safety & Industrial
- **Drone Operation**: Flight direction feedback
- **Machinery Operation**: Hazard direction warnings
- **Emergency Response**: Evacuation direction guidance

### Research & Development
- **Human-Computer Interaction**: Haptic interface studies
- **Spatial Cognition**: Direction perception research
- **Multisensory Integration**: Cross-modal perception studies

## Performance Specifications

### Timing Characteristics
- **Command Response**: <10ms from serial input
- **Single Wave Duration**: 180ms (default settings)
- **Inter-Motor Delay**: 20ms (configurable)
- **Pulse Duration**: 30ms (configurable)

### Power Consumption
- **Idle**: <50mA (microcontroller only)
- **Single Motor**: 60-100mA typical
- **All Motors Simultaneous**: 240-400mA
- **Recommended Power Supply**: 5V 1A minimum

### Motor Specifications
- **PWM Frequency**: Default (Arduino: ~490Hz, ESP32: 5kHz)
- **Resolution**: 8-bit (0-255 intensity levels)
- **Duty Cycle Range**: 39-100% during wave

## Comparison with Other Haptic Systems

### vs. Continuous Pulsing (First Code Example)
**Advantages:**
- More natural directional sensation
- Lower power consumption (sequential vs simultaneous)
- Smoother perceptual experience
- Easier to distinguish directions

**Trade-offs:**
- Longer response time (180ms vs continuous)
- More complex timing coordination

### vs. I2C Driver System (DRV2605)
**Advantages:**
- Simpler hardware (no I2C multiplexer needed)
- Lower cost (no driver ICs)
- More flexible timing control
- Easier to debug

**Trade-offs:**
- Fewer pre-programmed effects
- Manual PWM tuning required
- No built-in motor diagnostics

## License

This project is open source and available under the MIT License.

## Contributing

Contributions welcome! Areas for improvement:
- Additional directional patterns
- Mobile app integration
- Gesture recognition integration
- Machine learning for adaptive intensity
- Power optimization techniques

## Resources

- [Arduino PWM Tutorial](https://www.arduino.cc/en/Tutorial/Foundations/PWM)
- [ESP32 LEDC PWM Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html)
- [Motor Driver Circuits](https://www.electronics-tutorials.ws/blog/relay-switch-circuit.html)
- [Haptic Perception Research](https://en.wikipedia.org/wiki/Haptic_perception)

## Safety Notes

**Important Safety Information:**

- Never exceed motor voltage ratings
- Always use flyback diodes to protect microcontroller
- Ensure common ground between all power supplies
- Do not drive motors directly from GPIO pins (use transistors/drivers)
- Start with lower intensities and increase gradually
- Take breaks to avoid sensory fatigue during extended use

---

**Note**: This system is designed for educational and personal projects. For commercial applications, consider professional haptic driver ICs and consult with haptic design specialists for optimal user experience.