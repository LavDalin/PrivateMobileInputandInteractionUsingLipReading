# Sequential Directional Vibration Motor Controller

Arduino sketch for controlling vibration motors in sequential patterns across four directions (up, down, left, right) with adjustable intensity control via potentiometer.

## Hardware Requirements

- Arduino board (Nano, Uno, Mega, etc.)
- 5x vibration motors
- Motor driver circuit (transistors or motor driver board recommended)
- 1x potentiometer (10kΩ recommended)
- Appropriate power supply for motors

## Pin Configuration

### Vertical Motor Groups (Up/Down Control)
- **Group 1 (Top)**: Pins 3, 5 (2 motors)
- **Group 2 (Bottom)**: Pin 12 (1 motor, duplicated in array)

### Horizontal Motor Groups (Left/Right Control)
- **Left Group**: Pin 9 (1 motor)
- **Right Group**: Pin 6 (1 motor)

### Control Input
- **Potentiometer**: Analog Pin A0

## Features

- **Sequential Motor Control**: Motors activate in sequence to create directional movement sensation
- **4-Direction Control**: Up, Down, Left, Right via Serial commands
- **Intensity Control**: Real-time speed/intensity adjustment via potentiometer (0-255)
- **Directional Wave Patterns**: 
  - **Up**: Motors fire bottom-to-top
  - **Down**: Motors fire top-to-bottom
  - **Left**: Motors fire right-to-left
  - **Right**: Motors fire left-to-right
- **Continuous Operation**: Selected pattern repeats until direction is changed

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

```
up    - Activates motors in bottom-to-top sequence
down  - Activates motors in top-to-bottom sequence
left  - Activates motors in right-to-left sequence
right - Activates motors in left-to-right sequence
```

### Adjusting Intensity

Turn the potentiometer to adjust vibration intensity:
- **Minimum** (fully counter-clockwise): Low intensity (weak vibration)
- **Maximum** (fully clockwise): High intensity (strong vibration)

### Sequential Pattern Behavior

Each direction creates a wave-like sensation:

**UP Direction:**
```
Step 1: Bottom motors (pin 12) activate
        ↓ 300ms pulse
Step 2: Top motors (pins 3, 5) activate
        ↓ 300ms pulse
        ↓ 800ms delay
Repeat...
```

**DOWN Direction:**
```
Step 1: Top motors (pins 3, 5) activate
        ↓ 300ms pulse
Step 2: Bottom motors (pin 12) activate
        ↓ 300ms pulse
        ↓ 800ms delay
Repeat...
```

**LEFT Direction:**
```
Step 1: Right motor (pin 6) activates
        ↓ 300ms pulse
Step 2: Left motor (pin 9) activates
        ↓ 300ms pulse
        ↓ 800ms delay
Repeat...
```

**RIGHT Direction:**
```
Step 1: Left motor (pin 9) activates
        ↓ 300ms pulse
Step 2: Right motor (pin 6) activates
        ↓ 300ms pulse
        ↓ 800ms delay
Repeat...
```

## Serial Monitor Output

The Serial Monitor displays the active motor groups:

```
Direction set to: UP
Up: Group 1, Motor 12
Up: Group 0, Motor 3
Up: Group 0, Motor 5
```

## Code Parameters

You can modify these values in the code to customize behavior:

```cpp
pulseDuration = 300;         // Duration each motor group vibrates (ms)
delayBetweenGroups = 800;    // Delay between complete sequences (ms)
```

### Timing Diagram

```
|<-- pulseDuration -->|<-- delayBetweenGroups -->|
    Motor Group 1          Motor Group 2              Wait     Repeat
    ████████████           ████████████           ____________   ...
```

## How It Works

1. **Direction Selection**: User sends serial command (up/down/left/right)
2. **Intensity Reading**: Potentiometer value is continuously read and mapped to 0-255
3. **Sequential Activation**: Motor groups activate one at a time in the chosen direction
4. **Pattern Repeat**: After all groups have fired, the system waits and repeats

### Direction Logic

- **UP**: Groups activate in reverse order (bottom to top)
- **DOWN**: Groups activate in forward order (top to bottom)
- **LEFT**: Groups activate in reverse order (right to left)
- **RIGHT**: Groups activate in forward order (left to right)

## Customization

### Adding More Motor Groups

To add a middle group for vertical control:

```cpp
const int vertical_motorPins[3][2] = {
  {3, 5},    // "top" group  
  {6, 9},    // "middle" group (uncomment this line)
  {12, 12}   // "bottom" group
};

const int vertical_groups = 3;  // Update group count
```

### Adjusting Timing

```cpp
const int pulseDuration = 200;       // Faster pulses
const int delayBetweenGroups = 500;  // Shorter delay between sequences
```

## Safety Notes

⚠️ **Important Safety Information**:
- **Never connect motors directly to Arduino pins** - use transistors or motor driver boards
- Ensure adequate current capacity for all motors
- Use a separate power supply for motors if needed
- Add flyback diodes across motor terminals to protect against voltage spikes
- Check motor current draw doesn't exceed driver ratings

## Recommended Circuit Components

- **NPN Transistors**: 2N2222 or equivalent (one per motor)
- **Diodes**: 1N4001 or equivalent (one per motor for flyback protection)
- **Resistors**: 1kΩ base resistors for transistors
- **Motor Driver Board**: L293D or L298N (alternative to transistors)

## Example Circuit

```
Arduino Pin 3 ──→ [1kΩ] ──→ [Transistor Base] ──→ Motor 1 ──→ Power Supply
                              ↓ (with flyback diode)
                             GND

Potentiometer:
   5V ──→ [Outer Pin 1]
   A0 ──→ [Middle Pin (Wiper)]
  GND ──→ [Outer Pin 2]
```

## Troubleshooting

**Motors not vibrating**:
- Verify motor driver connections
- Check power supply voltage and current capacity
- Test individual motors with multimeter
- Confirm correct pin assignments

**Sequential pattern not working**:
- Check Serial Monitor baud rate (must be 9600)
- Verify correct direction command syntax
- Look for serial output to confirm commands are received

**Weak vibration at full potentiometer**:
- Check potentiometer wiring
- Verify potentiometer value is being read (check Serial output)
- Ensure adequate power supply for motors
- Check motor driver can handle required current

**Pattern too fast or too slow**:
- Adjust `pulseDuration` for individual pulse length
- Adjust `delayBetweenGroups` for overall pattern speed

## Applications

This sequential pattern controller is ideal for:
- Haptic navigation feedback devices
- Directional alerts in wearable devices
- Gaming haptic vests or suits
- Training/simulation equipment
- Accessibility/sensory feedback systems

## License

MIT License - feel free to use and modify for your projects.
