# Continuous Vibration Pulser

Arduino sketch for controlling two vibration motors with adjustable pulse timing via serial communication.

## Hardware Requirements

- Arduino board (Uno, Nano, etc.)
- 2x vibration motors
- Motor driver circuit (recommended for proper current handling)
- Appropriate power supply for motors

## Pin Configuration

- **Motor 1**: Digital Pin 3
- **Motor 2**: Digital Pin 2

## Features

- Simultaneous control of two vibration motors
- Continuous pulsing pattern (vibrate → pause → repeat)
- Real-time pulse duration adjustment via Serial Monitor
- Default settings: 150ms vibration, 1000ms pause

## Setup

1. Connect your vibration motors to pins 2 and 3 through appropriate motor driver circuitry
2. Upload the sketch to your Arduino (located in `vibration_pulser/vibration_pulser.ino`)
3. Open Serial Monitor (9600 baud)

## Usage

### Default Operation
The motors will pulse automatically:
- **ON** for 150ms
- **OFF** for 1000ms
- Repeat continuously

### Adjusting Pulse Duration
1. Open the Serial Monitor
2. Enter a number (in milliseconds) to change how long the motors vibrate
3. Press Enter
4. The new duration takes effect immediately

**Example**: Enter `500` to make motors vibrate for 500ms per pulse

## Code Parameters

```cpp
vibrationDuration = 150;  // Motor ON time (milliseconds)
pauseDuration = 1000;     // Motor OFF time (milliseconds)
```

## Notes

- Only the vibration duration can be changed via serial input
- Pause duration remains fixed at 1000ms (modify in code if needed)
- Motors are driven simultaneously on both pins
- Ensure your power supply can handle both motors running at once

## Safety

⚠️ **Important**: Always use appropriate current-limiting circuitry when driving motors from Arduino pins. Consider using a motor driver board or transistors to prevent damage to your Arduino.

## License

MIT License - feel free to use and modify for your projects.