# Simple Motor Test - Motor Blink

A beginner-friendly motor control sketch that blinks a motor on and off - the "Hello World" of motor control. Perfect for testing motor connections, verifying driver circuits, and learning PWM basics.

## Features

- **Simple On/Off Pattern**: Motor runs at half speed for 1 second, then stops for 1 second
- **PWM Speed Control**: Demonstrates basic motor speed control using analogWrite
- **Beginner Friendly**: Minimal code, easy to understand and modify
- **Hardware Troubleshooting**: Ideal first test for any motor setup
- **No External Libraries**: Uses only built-in Arduino functions

## What This Code Does

```
Motor Pattern:
ON (50% speed) → 1 second → OFF → 1 second → REPEAT

Timeline:
|████████████|            |████████████|            |
   1 second      1 second     1 second      1 second
   (ON 50%)      (OFF)        (ON 50%)      (OFF)
```

## Hardware Requirements

### Minimum Components

- **Microcontroller**: Arduino Uno/Nano/Mega, ESP32, or any PWM-capable board
- **Motor**: 
  - DC motor (3-6V)
  - Vibration motor (coin motor)
  - Small geared motor
- **Motor Driver Components**:
  - 1x NPN Transistor (2N2222, BC547, PN2222, or similar)
  - 1x 1kΩ Resistor (for transistor base)
  - 1x Diode (1N4001, 1N4007, or similar - for flyback protection)
- **Power Supply**: 
  - Battery pack (3-6V depending on motor)
  - USB power bank (5V)
  - Bench power supply

### Optional Components

- Breadboard for prototyping
- Jumper wires
- Multimeter for testing
- LED indicator (for visual feedback)

## Wiring Diagram

### Basic Transistor Circuit

```
Arduino/ESP32                  Motor Circuit
━━━━━━━━━━━                    ━━━━━━━━━━━━━

Pin 2 ──────┬─── 1kΩ ───┬─── Transistor Base (B)
            │            │
            │            │    Transistor (2N2222/BC547)
            │            │         (B)
         [LED]        [Resistor]    │
         (opt)           │      (C)─┴─(E)
            │            │       │     │
           GND          └────────┤     │
                                 │     │
                        Motor(-) │     │
                                 │     │
                        Diode ───┴─────┤
                      (Cathode/Band)   │
                           │           │
                      Motor(+)         │
                           │           │
                           │           │
                    Power Supply (+)   │
                       (3-6V)          │
                           │           │
                          GND ─────────┴──── Arduino GND
                                              (COMMON GROUND!)
```

### Detailed Component Connections

**Transistor Connections (2N2222/BC547):**
```
┌─────────────┐
│   Flat Side │
│             │
│  (E) (B) (C)│
└──┬───┬───┬──┘
   │   │   │
   │   │   └─── Collector → Motor (-)
   │   └─────── Base → 1kΩ Resistor → Pin 2
   └─────────── Emitter → Ground
```

**Diode (Flyback Protection):**
```
Motor (+) ←─── ┃│── ←─── Motor (-)
           Cathode   Anode
           (Band)

Important: Band (cathode) connects to motor positive!
```

**Complete Step-by-Step Wiring:**

1. **Transistor Base**: Arduino Pin 2 → 1kΩ resistor → Transistor Base
2. **Transistor Emitter**: Transistor Emitter → Ground
3. **Transistor Collector**: Transistor Collector → Motor negative (-)
4. **Motor Positive**: Motor positive (+) → Power supply positive (+)
5. **Flyback Diode**: 
   - Diode cathode (band) → Motor positive (+)
   - Diode anode → Motor negative (-)
6. **Common Ground**: Arduino GND → Power supply GND

### Using a Motor Driver Module (Alternative)

If using a motor driver module (L298N, L293D, TB6612):

```
Arduino Pin 2 → Motor Driver Input (IN1)
Motor Driver Output → Motor
Motor Driver VCC → Motor Power Supply (+)
Motor Driver GND → Ground
Arduino GND → Ground (COMMON GROUND!)
```

## Pin Configuration

```cpp
const int motorPin = 2;  // Change this to your actual pin
```

### PWM-Capable Pins by Board

**Arduino Uno/Nano:**
- PWM Pins: 3, 5, 6, 9, 10, 11
- Example: Change `motorPin = 2` to `motorPin = 3`

**Arduino Mega:**
- PWM Pins: 2-13, 44-46
- Pin 2 works on Mega!

**ESP32:**
- ALL GPIO pins support PWM
- Pin 2 works on ESP32!

**ESP8266:**
- PWM Pins: D1-D8 (GPIO 4, 5, 12, 13, 14, 15, 16)
- Use: `const int motorPin = D1;`

## Installation

1. **Build the circuit** following the wiring diagram above
2. **Verify connections** with a multimeter
3. **Open Arduino IDE**
4. **Load the code** (copy/paste or open .ino file)
5. **Select your board**: Tools → Board → [Your Board]
6. **Select correct port**: Tools → Port → [Your COM Port]
7. **Upload**: Click the upload button (→)
8. **Observe**: Motor should pulse on/off every second

## Usage

### Testing the Motor

1. **Upload the code** to your microcontroller
2. **Power up** the circuit
3. **Expected behavior**:
   - Motor runs at moderate speed for 1 second
   - Motor stops completely for 1 second
   - Pattern repeats indefinitely

### Verifying PWM Output (Without Motor)

Connect an LED instead of a transistor base:

```cpp
const int motorPin = 13;  // Use built-in LED pin

void loop() {
  analogWrite(motorPin, 128);  // LED half brightness
  delay(1000);
  analogWrite(motorPin, 0);    // LED off
  delay(1000);
}
```

## Configuration & Experiments

### Changing Motor Speed

```cpp
analogWrite(motorPin, 128);  // 50% speed (default)

// Try these values:
analogWrite(motorPin, 64);   // 25% speed (slow)
analogWrite(motorPin, 128);  // 50% speed (medium)
analogWrite(motorPin, 192);  // 75% speed (fast)
analogWrite(motorPin, 255);  // 100% speed (maximum)
```

### Changing Timing

```cpp
void loop() {
  analogWrite(motorPin, 128);
  delay(1000);  // Change this: motor ON duration
  
  analogWrite(motorPin, 0);
  delay(1000);  // Change this: motor OFF duration
}

// Examples:
// Fast blink: delay(200) and delay(200)
// Long pulse: delay(2000) and delay(500)
// Quick tap: delay(100) and delay(1000)
```

### PWM Speed Ramp (Smooth Speed Change)

```cpp
void loop() {
  // Ramp up from 0 to 255
  for (int speed = 0; speed <= 255; speed++) {
    analogWrite(motorPin, speed);
    delay(10);  // 10ms per step = ~2.5 seconds total
  }
  
  // Ramp down from 255 to 0
  for (int speed = 255; speed >= 0; speed--) {
    analogWrite(motorPin, speed);
    delay(10);
  }
}
```

### Pulsing Effect (Breathing)

```cpp
void loop() {
  // Quick pulse sequence
  for (int i = 0; i < 3; i++) {
    analogWrite(motorPin, 200);
    delay(100);
    analogWrite(motorPin, 0);
    delay(100);
  }
  
  delay(1000);  // Wait before next sequence
}
```

### Different Speed Patterns

```cpp
void loop() {
  // Slow speed
  analogWrite(motorPin, 100);
  delay(500);
  
  // Medium speed
  analogWrite(motorPin, 150);
  delay(500);
  
  // Fast speed
  analogWrite(motorPin, 255);
  delay(500);
  
  // Off
  analogWrite(motorPin, 0);
  delay(1000);
}
```

## Understanding PWM

### What is PWM (Pulse Width Modulation)?

PWM rapidly switches the pin HIGH and LOW to create an average voltage:

```
0% Duty Cycle (OFF):
|______|______|______|  Average: 0V

50% Duty Cycle (Half Speed):
|▀▀▀___|▀▀▀___|▀▀▀___|  Average: 2.5V

100% Duty Cycle (ON):
|▀▀▀▀▀▀|▀▀▀▀▀▀|▀▀▀▀▀▀|  Average: 5V
```

### PWM Value Chart

| analogWrite() | Duty Cycle | Average Voltage (5V) | Motor Speed |
|---------------|------------|---------------------|-------------|
| 0             | 0%         | 0V                  | Stopped     |
| 64            | 25%        | 1.25V               | Slow        |
| 128           | 50%        | 2.5V                | Medium      |
| 192           | 75%        | 3.75V               | Fast        |
| 255           | 100%       | 5V                  | Maximum     |

### PWM Frequency

- **Arduino Uno/Nano**: ~490 Hz (pins 3,9,10,11) or ~980 Hz (pins 5,6)
- **ESP32**: Configurable, default ~5000 Hz
- Higher frequency = smoother motor operation

## Troubleshooting

### Motor Doesn't Spin

**Check Power Supply:**
```
1. Verify motor voltage rating (check motor datasheet)
2. Use multimeter to measure supply voltage
3. Ensure power supply can provide enough current (check motor current rating)
```

**Check Wiring:**
```
1. Verify all connections match diagram
2. Ensure common ground between Arduino and motor power
3. Check transistor orientation (flat side indicates pin order)
4. Confirm diode polarity (band = cathode = motor positive)
```

**Test Transistor:**
```
1. Measure voltage at transistor base when code runs (should be ~0.6-0.7V when ON)
2. Measure voltage at collector (should switch between 0V and supply voltage)
3. Try a different transistor (might be damaged)
```

**Test Motor Directly:**
```
Temporarily connect motor directly to power supply:
Motor (+) → Power (+)
Motor (-) → Power (-)

If motor doesn't spin → motor is broken
If motor spins → circuit issue
```

### Motor Spins Constantly (Doesn't Turn Off)

**Possible causes:**
- Transistor stuck ON (damaged)
- Short circuit in wiring
- Wrong pin assignment in code

**Debug steps:**
```cpp
void loop() {
  digitalWrite(motorPin, HIGH);  // Force ON
  delay(1000);
  digitalWrite(motorPin, LOW);   // Force OFF
  delay(1000);
}
```

If still constant → hardware issue (check transistor/wiring)

### Motor Spins Weakly

**Increase PWM value:**
```cpp
analogWrite(motorPin, 255);  // Try full speed first
```

**Check voltage:**
- Motor may be under-powered
- Verify supply voltage matches motor rating
- Battery may be depleted

**Reduce transistor base resistor:**
```
Change from 1kΩ to 470Ω for stronger transistor saturation
```

### Motor Direction is Wrong

For DC motors, simply swap the motor wires:
```
Motor (+) ↔ Motor (-)
```

### Code Uploads But Nothing Happens

**Verify pin selection:**
```cpp
const int motorPin = 2;  // Make sure this matches your wiring
```

**Check board selection:**
- Tools → Board → Must match your actual hardware

**Add debug output:**
```cpp
void loop() {
  Serial.println("Motor ON");
  analogWrite(motorPin, 128);
  delay(1000);
  
  Serial.println("Motor OFF");
  analogWrite(motorPin, 0);
  delay(1000);
}

void setup() {
  pinMode(motorPin, OUTPUT);
  Serial.begin(9600);  // Add this
}
```

Open Serial Monitor (115200 baud) to see if code is running.

### Transistor Gets Hot

**Normal:** Slight warmth is okay
**Too hot to touch:** Problem!

**Solutions:**
- Use a heat sink
- Upgrade to higher-current transistor (TIP120, TIP122)
- Verify motor isn't drawing excessive current
- Check for short circuits

## Next Steps & Learning Path

Once this basic code works, try these progressions:

### 1. Speed Control via Serial
```cpp
void loop() {
  if (Serial.available()) {
    int speed = Serial.parseInt();
    speed = constrain(speed, 0, 255);
    analogWrite(motorPin, speed);
    Serial.print("Speed set to: ");
    Serial.println(speed);
  }
}
```

### 2. Button Control
```cpp
const int buttonPin = 7;

void loop() {
  if (digitalRead(buttonPin) == HIGH) {
    analogWrite(motorPin, 200);
  } else {
    analogWrite(motorPin, 0);
  }
}
```

### 3. Potentiometer Speed Control
```cpp
const int potPin = A0;

void loop() {
  int potValue = analogRead(potPin);
  int motorSpeed = map(potValue, 0, 1023, 0, 255);
  analogWrite(motorPin, motorSpeed);
  delay(10);
}
```

### 4. Multiple Motors
Expand to 4 motors for directional haptic feedback (like your other projects!)

### 5. Advanced Projects
- Directional wave haptic feedback
- I2C motor drivers with DRV2605
- Bluetooth-controlled motors

## Safety Guidelines


**Important Safety Information:**

- **Never connect motors directly to Arduino pins** - Always use a transistor or motor driver
- **Always use a flyback diode** - Protects against voltage spikes
- **Common ground is essential** - Arduino GND must connect to motor power GND
- **Check voltage ratings** - Don't exceed motor voltage specifications
- **Monitor current draw** - Ensure power supply can handle motor current
- **Verify polarity** - Double-check all connections before powering on
- **Start with low speeds** - Test at 50% before going to full power
- **Watch for overheating** - Shut down if components get too hot

## Common Motor Specifications

### Coin Vibration Motors
- **Voltage**: 3-4V DC
- **Current**: 60-100mA
- **Typical Use**: Haptic feedback, phone vibration

### Small DC Motors
- **Voltage**: 3-6V DC
- **Current**: 100-300mA
- **Typical Use**: Toys, small robots

### Geared Motors
- **Voltage**: 3-12V DC
- **Current**: 150-500mA
- **Typical Use**: Robot wheels, precise movement

## Applications

This simple motor test is the foundation for:

- **Haptic Feedback Systems** - Vibration alerts and directional cues
- **Robotics** - Motor driver testing before complex robot builds
- **Wearables** - Vibration notifications in smart devices
- **Toys & Props** - Basic motor control for DIY projects
- **Educational** - Learning PWM and motor control fundamentals
- **Prototyping** - Quick motor functionality verification

## Resources

### Tutorials
- [Arduino PWM Tutorial](https://www.arduino.cc/en/Tutorial/Foundations/PWM)
- [Motor Control Basics](https://learn.adafruit.com/adafruit-arduino-lesson-13-dc-motors)
- [Transistor as Switch](https://www.electronics-tutorials.ws/transistor/tran_4.html)

### Datasheets
- [2N2222 Transistor](https://www.onsemi.com/pdf/datasheet/p2n2222a-d.pdf)
- [1N4001 Diode](https://www.vishay.com/docs/88503/1n4001.pdf)

### Tools
- [Fritzing](https://fritzing.org/) - Circuit design software
- [Tinkercad Circuits](https://www.tinkercad.com/circuits) - Online Arduino simulator

## License

This project is open source and available under the MIT License.

## Contributing

This is a beginner educational project. Suggestions for improving clarity or adding helpful examples are welcome!

---

**Congratulations!** You've successfully controlled a motor with PWM. This is the foundation for all the advanced haptic feedback systems you can build next!