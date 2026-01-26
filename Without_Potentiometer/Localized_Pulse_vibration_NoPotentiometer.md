# Haptic Motor Array Controller

A directional haptic feedback system that controls 6 motors to provide 8-directional vibration patterns. Ideal for wearable devices, VR/AR controllers, navigation systems, or accessibility applications.

## Features

- **8-Directional Feedback**: Supports up, down, left, right, and four diagonal directions
- **Adjustable Intensity**: Control vibration strength from 10-255
- **Pulsing Pattern**: Rhythmic on/off cycles for comfortable haptic feedback
- **Serial Control**: Easy command interface via serial monitor
- **PWM Control**: Smooth motor control using ESP32's LEDC peripheral

## Hardware Requirements

- ESP32 development board
- 6 haptic/vibration motors (typically coin or ERM motors)
- Motor driver circuit (transistors or motor driver IC)
- Power supply appropriate for your motors

## Pin Configuration

| Motor Position | GPIO Pin |
|---------------|----------|
| Motor 1       | 2        |
| Motor 2       | 3        |
| Motor 3       | 4        |
| Motor 4       | 5        |
| Motor 5       | 6        |
| Motor 6       | 12       |

### Direction Mappings

- **Up**: Motors on pins 2, 3
- **Down**: Motors on pins 6, 12
- **Left**: Motor on pin 5
- **Right**: Motor on pin 4
- **Up-Right**: Motors on pins 6, 5
- **Up-Left**: Motors on pins 12, 3
- **Down-Right**: Motors on pins 4, 5
- **Down-Left**: Motors on pins 2, 3

## Installation

1. Clone or download this repository
2. Open the `.ino` file in Arduino IDE
3. Install the ESP32 board package if not already installed:
   - Go to **File > Preferences**
   - Add to Additional Board Manager URLs: `https://dl.espressif.com/dl/package_esp32_index.json`
   - Go to **Tools > Board > Boards Manager**
   - Search for "ESP32" and install
4. Select your ESP32 board from **Tools > Board**
5. Connect your ESP32 via USB
6. Upload the code

## Usage

### Serial Commands

Open the Serial Monitor at **115200 baud** and enter commands:

#### Direction Commands
```
up
down
left
right
up_right
up_left
down_right
down_left
```

#### Intensity Control
```
intensity 150
```
Replace `150` with any value between 10 and 255:
- `10`: Minimum intensity (gentle vibration)
- `255`: Maximum intensity (strong vibration)

### Example Session

```
Enter direction (up, down, left, right, up_right, up_left, down_right, down_left)
> up
Pulsing motor: 2 with intensity: 150
Pulsing motor: 3 with intensity: 150

> intensity 200
Intensity set to: 200

> down_right
Pulsing motor: 4 with intensity: 200
Pulsing motor: 5 with intensity: 200
```

## Configuration

You can modify these parameters in the code:

```cpp
int intensityLevel = 150;           // Default intensity (10-255)
const int maxIntensity = 255;       // Maximum allowed intensity
const int minIntensity = 10;        // Minimum allowed intensity

unsigned long pulseDuration = 200;  // Motor ON time (milliseconds)
unsigned long pulseInterval = 400;  // Motor OFF time (milliseconds)

const int pwmFreq = 5000;          // PWM frequency (Hz)
const int pwmResolution = 8;       // PWM resolution (8-bit = 0-255)
```

### Adjusting Pulse Timing

- **Faster pulse**: Decrease `pulseDuration` and `pulseInterval`
- **Slower pulse**: Increase `pulseDuration` and `pulseInterval`
- **Longer vibration**: Increase `pulseDuration`
- **Longer pause**: Increase `pulseInterval`

## Wiring Guide

### Basic Wiring (with transistors)

For each motor:
```
ESP32 GPIO Pin → 1kΩ Resistor → Transistor Base (2N2222 or similar)
Motor (+) → Power Supply (+)
Motor (-) → Transistor Collector
Transistor Emitter → Ground
ESP32 Ground → Power Supply Ground (common ground)
```

### Using a Motor Driver IC

Alternatively, use a motor driver IC like:
- DRV2605 (haptic motor driver)
- ULN2003 (Darlington array)
- L293D (motor driver)

Connect ESP32 GPIO pins to the driver inputs and motors to the driver outputs.

## Troubleshooting

**Motors not vibrating:**
- Check power supply voltage matches motor specifications
- Verify all ground connections are common
- Test motor driver circuit with a multimeter
- Ensure GPIO pins are correctly configured

**Weak vibration:**
- Increase intensity value
- Check motor power supply voltage
- Verify motor driver can handle required current

**Serial commands not working:**
- Verify baud rate is set to 115200
- Check Serial Monitor line ending is set to "Newline"
- Ensure ESP32 is properly connected via USB

## Application Ideas

- **Navigation aid**: Directional guidance for visually impaired users
- **Gaming controller**: Immersive feedback for VR/AR experiences
- **Wearable compass**: Tactile direction indication
- **Alert system**: Directional notifications for incoming messages
- **Rehabilitation device**: Motor skill training and therapy
- **Drone controller**: Intuitive flight direction feedback

## License

This project is open source and available under the MIT License.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## Author

Created for ESP32-based haptic feedback applications.

---

**Note**: Always ensure your power supply and motor driver circuit can safely handle the current requirements of your motors to prevent damage to the ESP32.