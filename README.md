Arduino sketch for controlling multiple vibration motors in different directions with adjustable intensity and automatic pulsing.

## Hardware Requirements

- Arduino board (Nano, Uno, Mega, etc.)
- 5x vibration motors
- Motor driver circuit (transistors or motor driver board recommended)
- 1x potentiometer (10kΩ recommended)
- Appropriate power supply for motors

## Pin Configuration

### Motor Pins
- **Up Direction**: Pins 3, 5 (2 motors)
- **Down Direction**: Pin 12 (1 motor)
- **Right Direction**: Pin 6 (1 motor)
- **Left Direction**: Pin 9 (1 motor)

### Control Input
- **Potentiometer**: Analog Pin A0

## Features

- **Directional Control**: Control motors in 4 directions (up, down, left, right) via Serial commands
- **Intensity Control**: Real-time intensity adjustment via potentiometer (0-255)
- **Smooth Fading**: Gradual intensity transitions for smooth vibration changes
- **Automatic Pulsing**: Motors pulse ON/OFF automatically with adaptive timing
- **Adaptive Pulse Timing**: Pulse duration and interval adjust based on intensity
- **Voltage Monitoring**: Displays potentiometer voltage every second in Serial Monitor

## Setup

1. Connect vibration motors to the specified pins through appropriate motor driver circuitry
2. Connect potentiometer to analog pin A0:
   - One outer pin to 5V
   - Other outer pin to GND
   - Middle pin to A0
3. Upload the sketch to your Arduino
4. Open Serial Monitor (9600 baud)

## Usage

### Controlling Direction

Enter one of the following commands in the Serial Monitor:

- `up` - Activates motors on pins 3 and 5
- `down` - Activates motor on pin 12
- `left` - Activates motor on pin 9
- `right` - Activates motor on pin 6

The motors will continue pulsing in the selected direction until a new command is entered.

### Adjusting Intensity

Turn the potentiometer to adjust vibration intensity:
- **Minimum** (fully counter-clockwise): Low intensity, rapid short pulses
- **Maximum** (fully clockwise): High intensity, longer sustained pulses

### Pulse Behavior

The system automatically pulses motors ON and OFF:
- **Low Intensity**: 50ms ON, 200ms OFF (rapid tapping)
- **High Intensity**: 200ms ON, 400ms OFF (sustained pulses)
- Timing smoothly adjusts between these extremes based on potentiometer position

## Serial Monitor Output

The Serial Monitor displays:
```
Potentiometer Voltage: 2.45V
Pulsing motor: 3 with intensity: 128
Pulsing motor: 5 with intensity: 128
```

## Code Parameters

```cpp
pulseDuration = 200;        // Base pulse ON time (milliseconds)
pulseInterval = 400;        // Base pulse OFF time (milliseconds)
fadeSpeed = 20;             // Intensity transition speed (ms between steps)
voltageDisplayInterval = 1000;  // Voltage display frequency (ms)
```

## How It Works

1. **Direction Selection**: User sends serial command to select motor group
2. **Intensity Reading**: Potentiometer value is read and mapped to 0-255
3. **Smooth Fading**: Current intensity gradually adjusts toward target intensity
4. **Adaptive Pulsing**: Pulse timing automatically adjusts based on intensity level
5. **Motor Control**: Selected motors pulse ON/OFF continuously

## Safety Notes

⚠️ **Important Safety Information**:
- Always use appropriate current-limiting circuitry (transistors or motor driver boards)
- Do not connect motors directly to Arduino pins - they may draw too much current
- Ensure your power supply can handle all motors running simultaneously
- Consider using a separate power supply for motors if using multiple high-current motors

## Troubleshooting

**Motors not vibrating**:
- Check motor driver circuit connections
- Verify power supply is adequate
- Ensure motors are connected to correct pins

**Erratic behavior**:
- Check potentiometer connections
- Verify Serial Monitor baud rate is set to 9600
- Make sure motor power supply is stable

**Intensity not changing**:
- Test potentiometer with a multimeter
- Verify potentiometer is connected to A0
- Check that potentiometer is properly wired (5V, GND, signal)

## Example Circuit

```
Arduino Pin 3 ──→ [Driver] ──→ Motor 1 (Up)
Arduino Pin 5 ──→ [Driver] ──→ Motor 2 (Up)
Arduino Pin 6 ──→ [Driver] ──→ Motor 3 (Right)
Arduino Pin 9 ──→ [Driver] ──→ Motor 4 (Left)
Arduino Pin 12 ─→ [Driver] ──→ Motor 5 (Down)

Potentiometer:
   5V ──→ [Outer Pin]
   A0 ──→ [Middle Pin]
  GND ──→ [Outer Pin]
```

## License

MIT License - feel free to use and modify for your projects.