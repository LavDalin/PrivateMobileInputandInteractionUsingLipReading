# Arduino Directional Motor Control System

This Arduino sketch provides directional control for multiple motor groups arranged in vertical and horizontal configurations. Motors can be controlled to create directional movement patterns (up, down, left, right) with adjustable speed settings.

## Hardware Configuration

### Vertical Motor Groups (3 groups, 2 motors each)
- **Up group**: Pins 2, 3
- **Middle group**: Pins 5, 4
- **Bottom group**: Pins 6, 12

### Horizontal Motor Groups (2 groups, 3 motors each)
- **Left group**: Pins 3, 5, 12
- **Right group**: Pins 2, 4, 6

## Features

- **4 Directional Control**: Up, Down, Left, Right
- **Adjustable Speed**: Set individual speed (0-255) for each direction
- **Serial Command Interface**: Control via serial monitor
- **Sequential Group Activation**: Motors activate in groups with configurable timing
- **Pulse Control**: Each motor pulses 5 times per activation cycle

## Configuration Parameters

```cpp
delayBetweenGroups = 800;  // Delay between motor group activations (ms)
pulseDuration = 50;        // Duration of each motor pulse (ms)
upSpeed = 180;             // Default speed for upward direction
downSpeed = 180;           // Default speed for downward direction
leftSpeed = 180;           // Default speed for leftward direction
rightSpeed = 180;          // Default speed for rightward direction
```

## Serial Commands

### Direction Control
Set the active direction by sending one of these commands:
- `up` - Activate upward movement pattern
- `down` - Activate downward movement pattern
- `left` - Activate leftward movement pattern
- `right` - Activate rightward movement pattern

### Speed Control
Adjust the speed for each direction using the format:
```
speed <direction> <value>
```

**Examples:**
- `speed up 200` - Set upward speed to 200
- `speed down 150` - Set downward speed to 150
- `speed left 255` - Set leftward speed to maximum (255)
- `speed right 100` - Set rightward speed to 100

**Valid speed range:** 0-255 (PWM values)

## How It Works

### Movement Patterns

**Up Direction:**
- Activates groups in reverse order (bottom → middle → top)
- Creates an upward wave effect

**Down Direction:**
- Activates groups in forward order (top → middle → bottom)
- Creates a downward wave effect

**Right Direction:**
- Activates horizontal groups left to right

**Left Direction:**
- Activates horizontal groups in reverse (right to left)

### Pulse Sequence
Each motor group:
1. Pulses ON for 50ms (configurable)
2. Pulses OFF for 40ms
3. Repeats 5 times per activation
4. Waits 800ms before next group activation

## Setup Instructions

1. **Hardware Setup:**
   - Connect motors to the designated Arduino pins
   - Ensure proper power supply for motors (external recommended)
   - Use appropriate motor drivers if needed

2. **Upload Code:**
   - Open the sketch in Arduino IDE
   - Select your Arduino board type
   - Upload to your Arduino

3. **Serial Monitor:**
   - Open Serial Monitor (Tools → Serial Monitor)
   - Set baud rate to 9600
   - Send commands to control motors

## Usage Example

```
// Serial Monitor session:
> up
Direction set to: up
Up: Group 2, Motor 6
Up: Group 2, Motor 12
...

> speed up 220
Set UP speed to 220

> right
Direction set to: right
Right: Group 0, Motor 3
Right: Group 0, Motor 5
...
```

## Pin Overlap Note

Some pins are shared between vertical and horizontal groups (pins 2, 3, 4, 5, 6, 12). This means:
- Do not activate vertical and horizontal movements simultaneously
- The current implementation ensures only one direction is active at a time
- Switching directions is safe and immediate

## Troubleshooting

**Motors not responding:**
- Check pin connections
- Verify power supply is adequate
- Check serial monitor for debug output

**Erratic movement:**
- Verify only one direction is active
- Check for loose connections
- Ensure proper motor driver configuration

**Serial commands not working:**
- Confirm baud rate is set to 9600
- Check for correct command syntax
- Ensure no extra spaces in commands

## Customization

### Adjust Timing
Modify these constants to change behavior:
- `delayBetweenGroups`: Time between group activations
- `pulseDuration`: Length of motor ON time
- Inner delay (40ms): Adjust in control functions for pulse spacing

### Change Pin Assignments
Edit the `vertical_motorPins` and `horizontal_motorPins` arrays to match your wiring.

### Modify Pulse Count
Change the loop limit `j < 5` in control functions to adjust pulses per group.

## License

This code is provided as-is for educational and maker projects.

## Contributing

Feel free to modify and enhance this code for your specific application!