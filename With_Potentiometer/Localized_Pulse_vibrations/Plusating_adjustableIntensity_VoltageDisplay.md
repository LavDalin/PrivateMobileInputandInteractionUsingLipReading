# Fixed-Timing Pulsing Vibration Motor Controller

Arduino sketch for controlling vibration motors in four directions with fixed pulse timing and adjustable intensity control via potentiometer.

## Hardware Requirements

- Arduino board (Uno, Nano, etc.)
- 6x vibration motors
- Motor driver circuit (transistors or motor driver board recommended)
- 1x potentiometer (10kΩ recommended)
- Appropriate power supply for motors

## Pin Configuration

### Motor Pins
- **Up Direction**: Pins 3, 5 (2 motors)
- **Down Direction**: Pin 12 (2 motor)
- **Right Direction**: Pin 6 (1 motor)
- **Left Direction**: Pin 9 (1 motor)

### Control Input
- **Potentiometer**: Analog Pin A0

## Features

- **4-Direction Control**: Control motors in up, down, left, right directions via Serial commands
- **Fixed Pulse Timing**: Consistent 200ms ON / 400ms OFF pulse pattern
- **Adjustable Intensity**: Real-time vibration strength control via potentiometer (0-255)
- **Non-blocking Operation**: Uses `millis()` for timing - system remains responsive
- **Voltage Monitoring**: Displays potentiometer voltage every second
- **Continuous Pulsing**: Selected motors pulse until direction is changed

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
- `down` - Activates motors on pin 12
- `left` - Activates motor on pin 9
- `right` - Activates motor on pin 6

The selected motors will pulse continuously until you enter a new direction command.

### Adjusting Intensity

Turn the potentiometer to adjust vibration strength:
- **Minimum** (fully counter-clockwise): Weakest vibration (0/255)
- **Maximum** (fully clockwise): Strongest vibration (255/255)

**Note**: Unlike adaptive systems, changing intensity only affects vibration **strength**, not pulse timing.

## Pulse Pattern

All directions use the same fixed timing:

```
Motor ON  (200ms) ████████████
Motor OFF (400ms) ____________
Motor ON  (200ms) ████████████
Motor OFF (400ms) ____________
(repeats continuously...)
```

### Timing Diagram

```
Time:     0ms    200ms   600ms   800ms   1200ms
          |       |       |       |       |
Motor:    ████████|_______|████████|_______|
          ON      OFF     ON      OFF
          (200ms) (400ms) (200ms) (400ms)
```

**Total Cycle Time**: 600ms (200ms ON + 400ms OFF)  
**Pulses Per Minute**: 100 pulses

## Serial Monitor Output

```
Enter direction (up, down, left, right). Potentiometer controls intensity.
Potentiometer Voltage: 2.45V
Pulsing motor: 3 with intensity: 128
Pulsing motor: 5 with intensity: 128
Potentiometer Voltage: 2.45V
Pulsing motor: 3 with intensity: 128
Pulsing motor: 5 with intensity: 128
```

## Code Parameters

You can modify these constants to customize behavior:

```cpp
pulseDuration = 200;           // Motor ON time (milliseconds)
pulseInterval = 400;           // Motor OFF time (milliseconds)
voltageDisplayInterval = 1000; // Voltage display frequency (milliseconds)
maxIntensity = 255;            // Maximum PWM value
```

### Customization Examples

**Faster Pulsing:**
```cpp
const unsigned long pulseDuration = 100;  // 100ms ON
const unsigned long pulseInterval = 200;  // 200ms OFF
// Result: 200 pulses per minute
```

**Slower Pulsing:**
```cpp
const unsigned long pulseDuration = 300;  // 300ms ON
const unsigned long pulseInterval = 700;  // 700ms OFF
// Result: 60 pulses per minute
```

**Equal ON/OFF:**
```cpp
const unsigned long pulseDuration = 250;  // 250ms ON
const unsigned long pulseInterval = 250;  // 250ms OFF
// Result: 50% duty cycle
```

## How It Works

1. **Initialization**: All motor pins set as outputs, potentiometer as input
2. **Direction Selection**: User sends serial command (up/down/left/right)
3. **Intensity Reading**: Potentiometer value continuously read and mapped to 0-255
4. **Pulse Generation**: 
   - Timer tracks elapsed time using `millis()`
   - Every 200ms: Motors turn ON at current intensity
   - After 200ms: Motors turn OFF
   - After 400ms OFF: Cycle repeats
5. **Continuous Operation**: Pattern repeats until new direction is entered

## Key Differences from Other Vibration Controllers

| Feature | This Controller | Adaptive Controller | Sequential Controller |
|---------|----------------|---------------------|----------------------|
| **Pulse Timing** | Fixed (200/400ms) | Changes with intensity | Fixed per group |
| **Intensity Control** | Strength only | Strength + timing | Strength only |
| **Motor Activation** | Simultaneous | Simultaneous | One group at a time |
| **Pattern Type** | Simple pulsing | Dynamic pulsing | Wave pattern |
| **Complexity** | Low | Medium | High |
| **Use Case** | Basic haptic alerts | Nuanced feedback | Directional navigation |

## Advantages

**Simple & Predictable**: Fixed timing makes behavior easy to understand  
**Consistent Feel**: Same pulse rate regardless of intensity  
**Non-blocking**: System remains responsive during operation  
**Easy to Modify**: Simple timing parameters  
**Reliable**: No complex timing calculations

## Applications

This controller is ideal for:
- Basic haptic notification systems
- Alert/notification devices
- Simple directional indicators
- Training/prototyping systems
- Educational projects
- Devices requiring consistent pulse rates

## Safety Notes

**Important Safety Information**:
- **Never connect motors directly to Arduino pins** - they may draw too much current
- Use appropriate motor driver circuits (transistors or motor driver boards)
- Ensure power supply can handle all motors running simultaneously
- Add flyback diodes across motor terminals for protection
- Check total current draw doesn't exceed Arduino's limits (40mA per pin, 200mA total)

## Recommended Circuit

### Using NPN Transistors (Per Motor)

```
Arduino Pin → [1kΩ Resistor] → Transistor Base
                                    ↓
Motor (+) ← Power Supply      Collector
Motor (-) → [Diode] → GND     Emitter → GND
            (1N4001)
```

### Using L293D Motor Driver

```
Arduino Pins 3,5,6,9,12 → L293D Input Pins
L293D Output Pins → Motors
L293D VCC1 → 5V (logic)
L293D VCC2 → Motor Power Supply
L293D GND → Common Ground
```

## Troubleshooting

**Motors not vibrating:**
- Verify motor driver circuit is correct
- Check power supply voltage and current capacity
- Test motors individually with a battery
- Confirm pin assignments match your wiring

**Pulse timing seems off:**
- Check Serial Monitor for timing messages
- Verify `millis()` is working (should count up continuously)
- Look for blocking delays in your modifications

**Intensity not changing:**
- Test potentiometer with multimeter (should vary 0-5V)
- Verify potentiometer wiring (5V, GND, A0)
- Check Serial Monitor for voltage readings

**Motors stay on constantly:**
- Check for electrical shorts
- Verify motor driver circuit
- Look for code modifications that might affect timing logic

**Direction commands not working:**
- Confirm Serial Monitor baud rate is 9600
- Check command spelling (must be exact: "up", "down", "left", "right")
- Ensure commands are lowercase
- Look for extra spaces or characters

## Expansion Ideas

### Add More Directions
```cpp
const int forward_motorPins[2] = {10, 11};
// Add in processInput():
else if (direction == "forward") {
    pulseMotors(forward_motorPins, intensity, 2);
}
```

### Add Button Control
```cpp
const int buttonPin = 7;
// In setup():
pinMode(buttonPin, INPUT_PULLUP);
// In loop():
if (digitalRead(buttonPin) == LOW) {
    lastDirection = "up";  // Trigger direction
}
```

### Variable Pulse Patterns
```cpp
// Add different patterns for different intensities
if (intensity < 85) {
    pulseDuration = 100;   // Quick taps
    pulseInterval = 500;
} else if (intensity < 170) {
    pulseDuration = 200;   // Medium
    pulseInterval = 400;
} else {
    pulseDuration = 300;   // Long pulses
    pulseInterval = 300;
}
```

## Technical Specifications

- **Input Voltage**: 5V (Arduino)
- **Motor Control**: PWM (0-255)
- **Pulse Frequency**: ~1.67 Hz (100 pulses/minute)
- **Duty Cycle**: 33% (200ms ON / 600ms total)
- **Response Time**: Non-blocking (immediate)
- **Serial Baud Rate**: 9600

## License

MIT License - feel free to use and modify for your projects.

## Version History

- **v1.0**: Initial release with fixed timing pulse control
