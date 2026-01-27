# I2C Haptic Driver with Multiplexer

A directional haptic feedback system using DRV2605 haptic motor drivers with a TCA9548A I2C multiplexer. This system provides directional "pull" sensations using coordinated haptic effects across 4 motors.

## Features

- **4-Directional Haptic Feedback**: Up, down, left, and right directional cues
- **DRV2605 Integration**: Professional haptic motor drivers with built-in effects library
- **I2C Multiplexer Support**: Control multiple DRV2605 devices using TCA9548A
- **Coordinated Effects**: Primary strong clicks with secondary weak feedback for realistic directional pull
- **Built-in Testing**: Individual motor testing mode for diagnostics
- **ERM/LRA Support**: Compatible with both Eccentric Rotating Mass and Linear Resonant Actuator motors

## Hardware Requirements

### Required Components

- **Microcontroller**: ESP32, Arduino, or any board with I2C support
- **I2C Multiplexer**: TCA9548A (1x)
- **Haptic Drivers**: Adafruit DRV2605 breakout boards (4x)
- **Motors**: ERM or LRA haptic motors (4x)
  - Coin vibration motors (ERM)
  - Linear Resonant Actuators (LRA)
- **Power Supply**: 3.3V/5V depending on your board

### Hardware Connections

#### I2C Bus Wiring

```
Microcontroller → TCA9548A Multiplexer
------------------------------------
SDA → SDA
SCL → SCL
3.3V → VIN
GND → GND
```

#### TCA9548A to DRV2605 Connections

```
TCA9548A Channel → DRV2605 Board → Motor Position
------------------------------------------------
Channel 0        → DRV2605 #1     → LEFT
Channel 1        → DRV2605 #2     → RIGHT
Channel 3        → DRV2605 #3     → UP
Channel 4        → DRV2605 #4     → DOWN
```

For each DRV2605:
```
TCA9548A SD[n] → DRV2605 SDA
TCA9548A SC[n] → DRV2605 SCL
Power rail     → DRV2605 VIN
Ground         → DRV2605 GND
DRV2605 Motor± → Haptic Motor
```

#### I2C Addresses

- **TCA9548A Multiplexer**: `0x70` (default)
- **DRV2605 Drivers**: `0x5A` (default, shared across channels)

## Software Requirements

### Required Libraries

Install these libraries via Arduino Library Manager:

```
Adafruit DRV2605 Library by Adafruit
Wire (built-in with Arduino IDE)
```

#### Installation Steps

1. Open Arduino IDE
2. Go to **Sketch → Include Library → Manage Libraries**
3. Search for "Adafruit DRV2605"
4. Click **Install**

### Board Support

Compatible with:
- ESP32
- Arduino Uno/Nano/Mega
- Teensy
- Any microcontroller with I2C support

## Installation

1. **Wire Hardware**: Follow the connection diagram above
2. **Install Libraries**: Install required libraries via Library Manager
3. **Upload Code**: Open the `.ino` file and upload to your board
4. **Open Serial Monitor**: Set baud rate to **115200**

## Usage

### Serial Commands

Open Serial Monitor at **115200 baud** and send single-character commands:

| Command | Action |
|---------|--------|
| `u` | Pull Up - Strong click at top, weak click at bottom |
| `d` | Pull Down - Strong click at bottom, weak click at top |
| `l` | Pull Left - Strong click on left, weak click on right |
| `r` | Pull Right - Strong click on right, weak click on left |
| `s` | Stop all motors |
| `t` | Test all motors individually (diagnostic mode) |

### Example Session

```
I2C Haptic Driver with Multiplexer Setup
I2C Bus Initialized!
Initializing DRV2605 driver on MUX channel 3... OK!
Initializing DRV2605 driver on MUX channel 4... OK!
Initializing DRV2605 driver on MUX channel 0... OK!
Initializing DRV2605 driver on MUX channel 1... OK!
Driver setup complete.
Commands: u/d/l/r for direction, 's' to stop, 't' to test all motors

> u
Direction set to: u
Pulling ↑ Up
Triggered effect 47 on channel 3
Triggered effect 21 on channel 4

> t
Testing all motors individually...
Testing UP motor (channel 3)
Triggered effect 47 on channel 3
Testing DOWN motor (channel 4)
Triggered effect 47 on channel 4
Testing LEFT motor (channel 0)
Triggered effect 47 on channel 0
Testing RIGHT motor (channel 1)
Triggered effect 47 on channel 1
Test complete.
```

## Configuration

### Motor Type Selection

**For ERM Motors (Eccentric Rotating Mass):**
```cpp
drv.useERM(); // Default setting in code
```

**For LRA Motors (Linear Resonant Actuators):**
```cpp
drv.useLRA(); // Change this in initializeMotorDriver() function
```

### Haptic Effects

The code uses two pre-defined effects from the DRV2605 library:

```cpp
const uint8_t EFFECT_STRONG_CLICK = 47; // Strong Click 1
const uint8_t EFFECT_WEAK_CLICK   = 21; // Soft Bump 2
```

#### Available DRV2605 Effects (Library 1)

You can customize by changing the effect numbers:

| Effect # | Name | Description |
|----------|------|-------------|
| 1 | Strong Click 100% | Sharp strong click |
| 47 | Strong Click 1 | Strong haptic click |
| 21 | Soft Bump 2 | Gentle bump sensation |
| 7 | Soft Bump 100% | Soft continuous bump |
| 14 | Buzz 1 | Buzzing sensation |
| 52 | Pulsing Strong 1 | Strong pulsing |

See the [DRV2605 datasheet](https://www.ti.com/lit/ds/symlink/drv2605.pdf) for complete effect library (123 effects available).

### Channel Mapping

If your motors are connected to different channels, modify these definitions:

```cpp
#define UP_MOTOR_CHANNEL    3  // Change to your channel
#define DOWN_MOTOR_CHANNEL  4
#define LEFT_MOTOR_CHANNEL  0
#define RIGHT_MOTOR_CHANNEL 1
```

### Timing Adjustments

**Delay between primary and secondary effect:**
```cpp
delay(40); // Adjust this value in pullUp(), pullDown(), etc.
```

**Repeat interval:**
```cpp
delay(500); // Change this in loop() for faster/slower repetition
```

## How It Works

### Directional Pull Mechanism

Each direction uses a **coordinated dual-motor approach**:

1. **Primary Motor**: Strong click effect in the pull direction
2. **Secondary Motor**: Weak click effect on opposite side (40ms delay)
3. This creates a perceptual "pull" sensation toward the primary motor

Example for "Pull Up":
```cpp
void pullUp() {
  triggerMotor(UP_MOTOR_CHANNEL, EFFECT_STRONG_CLICK);    // Top motor: strong
  delay(40);
  triggerMotor(DOWN_MOTOR_CHANNEL, EFFECT_WEAK_CLICK);     // Bottom motor: weak
}
```

### I2C Multiplexer Operation

The TCA9548A allows multiple DRV2605 devices (all sharing address 0x5A) to coexist on one I2C bus:

1. Select channel using `tcaselect(channel)`
2. All I2C commands now route to devices on that channel
3. Switch channels to communicate with different motors

## Troubleshooting

### "Failed to find DRV2605 on channel X"

**Possible causes:**
- Incorrect wiring on I2C bus (SDA/SCL swapped)
- Missing pull-up resistors (usually built-in on breakout boards)
- Wrong I2C address (default is 0x5A)
- Power supply issue

**Solutions:**
- Use I2C scanner sketch to detect devices
- Check all connections with multimeter
- Verify 3.3V/5V power supply is stable
- Ensure common ground between all devices

### Motors Not Vibrating

**Check:**
- Motor polarity doesn't usually matter for vibration motors, but verify connections
- DRV2605 is receiving power (LED indicator on board)
- Motor is compatible (coin vibration motors typically work at 3V)
- Effect library is set correctly: `drv.selectLibrary(1)`

### Weak or Inconsistent Vibration

**Try:**
- Switch to a stronger effect number (e.g., effect 1 instead of 47)
- Verify motor is ERM type and code uses `drv.useERM()`
- Check motor power supply capacity
- Reduce delay times for more responsive feedback

### One Motor Works, Others Don't

**Debug steps:**
1. Run the test command: `t`
2. Check which channels respond
3. Verify TCA9548A channel connections
4. Swap DRV2605 boards to isolate hardware issues

### I2C Communication Errors

- Add pull-up resistors (2.2kΩ - 4.7kΩ) to SDA and SCL if needed
- Reduce I2C bus length (keep wires short)
- Lower I2C clock speed: `Wire.setClock(100000);` in setup()

## Applications

### Navigation & Guidance
- Turn-by-turn haptic navigation
- Indoor wayfinding for accessibility
- Drone flight direction feedback

### Gaming & VR
- Directional damage indicators
- Force feedback for movement
- Immersive environmental cues

### Accessibility
- Tactile direction guidance for visually impaired users
- Alert direction indicators
- Non-visual UI navigation

### Robotics
- Telepresence force feedback
- Remote operation guidance
- Safety alert directionality

### Medical & Rehabilitation
- Balance training systems
- Gait correction feedback
- Motor skill rehabilitation

## Advanced Customization

### Adding Diagonal Directions

Add support for 8-way directions:

```cpp
void pullUpRight() {
  triggerMotor(UP_MOTOR_CHANNEL, EFFECT_STRONG_CLICK);
  delay(20);
  triggerMotor(RIGHT_MOTOR_CHANNEL, EFFECT_STRONG_CLICK);
  delay(40);
  triggerMotor(DOWN_MOTOR_CHANNEL, EFFECT_WEAK_CLICK);
  triggerMotor(LEFT_MOTOR_CHANNEL, EFFECT_WEAK_CLICK);
}
```

### Custom Effect Sequences

Create multi-step haptic patterns:

```cpp
void customPattern() {
  tcaselect(UP_MOTOR_CHANNEL);
  drv.setWaveform(0, 47);  // Strong click
  drv.setWaveform(1, 21);  // Soft bump
  drv.setWaveform(2, 14);  // Buzz
  drv.setWaveform(3, 0);   // End
  drv.go();
}
```

### Bluetooth/WiFi Control

Integrate with wireless communication:

```cpp
// Example for ESP32 Bluetooth
#include <BluetoothSerial.h>
BluetoothSerial SerialBT;

void loop() {
  if (SerialBT.available()) {
    char dir = SerialBT.read();
    // Process direction commands
  }
}
```

## Technical Specifications

### DRV2605 Specifications
- **Supply Voltage**: 2.5V - 5.2V
- **I2C Address**: 0x5A (default)
- **Max Motor Current**: 250mA
- **PWM Frequency**: 200Hz (adjustable)
- **Effect Library**: 123 pre-programmed effects

### TCA9548A Specifications
- **Supply Voltage**: 1.65V - 5.5V
- **I2C Address**: 0x70 - 0x77 (selectable)
- **Channels**: 8 independent I2C buses
- **Max I2C Speed**: 400kHz

## License

This project is open source and available under the MIT License.

## Resources

- [Adafruit DRV2605 Guide](https://learn.adafruit.com/adafruit-drv2605-haptic-controller-breakout)
- [TCA9548A Datasheet](https://www.ti.com/lit/ds/symlink/tca9548a.pdf)
- [DRV2605 Effect Library](https://www.ti.com/lit/ds/symlink/drv2605.pdf) (See Section 11.2)
- [ERM vs LRA Motor Guide](https://www.precisionmicrodrives.com/erm-vs-lra-motor-comparison)

## Contributing

Contributions welcome! Please submit pull requests or open issues for:
- Bug fixes
- New directional patterns
- Additional motor configurations
- Documentation improvements

---

**Safety Note**: Ensure all power supplies and connections are properly rated for your motors. Do not exceed the DRV2605's 250mA motor current limit per channel.