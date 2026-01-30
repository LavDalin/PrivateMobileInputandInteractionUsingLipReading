# LRA Pulling Illusion Haptic System

A sophisticated directional haptic feedback system that creates realistic "pulling" sensations using asymmetric vibration patterns. This system employs 4 LRA (Linear Resonant Actuator) motors with DRV2605L haptic drivers to simulate the perception of being physically pulled in a specific direction.

## Features

- **Directional Pulling Illusion**: Creates sensation of being pulled forward, backward, left, or right
- **Asymmetric Pulse Technology**: Long strong pulses with brief counter-pulses for realistic force perception
- **Extended Cycle Timing**: 25-second cycles prevent sensory adaptation
- **LRA Motor Support**: Optimized for Linear Resonant Actuators with precise haptic effects
- **PCA9548A Multiplexer**: Controls multiple DRV2605L drivers on single I2C bus
- **Persistent Directional Cues**: Designed for sustained navigation and guidance

## How the Pulling Illusion Works

This system uses **asymmetric haptic stimulation** to create a perceptual "pull":

### The Science Behind It

```
Pull Forward Example:
┌─────────────────────────────────────────────────────────┐
│                                                           │
│  Front Motor:  ████████████████████████ (7 seconds, 100%)│
│                                                           │
│  Back Motor:   █ (3ms, 25%) ← brief counter-pulse       │
│                ↑                                          │
│                2ms delay                                  │
│                                                           │
│  Rest Period:  _____________________ (18 seconds)        │
│                                                           │
└─────────────────────────────────────────────────────────┘
     Total Cycle: 25 seconds
```

**Perceptual Mechanism:**
1. **Primary Stimulus**: Strong 7-second vibration in target direction
2. **Counter-Stimulus**: Brief 3ms weak pulse on opposite side (creates contrast)
3. **Long Rest**: 18 seconds allows brain to process and prevents habituation
4. **Result**: Brain interprets sustained strong + brief weak as directional force

This differs from other haptic approaches:
- **NOT a continuous vibration** - Uses long pulses with rest periods
- **NOT sequential waves** - Uses simultaneous asymmetric activation
- **NOT short taps** - Uses sustained sensation (7 seconds) for persistent cue

## Hardware Requirements

### Required Components

- **Microcontroller**: ESP32, Arduino Mega, or any board with I2C support
- **I2C Multiplexer**: PCA9548A (1x) - 8-channel I2C multiplexer
- **Haptic Drivers**: Adafruit DRV2605L breakout boards (4x minimum, up to 8x)
- **Motors**: LRA (Linear Resonant Actuator) motors (4x)
  - Recommended: Precision Microdrives LRA motors
  - Typical: 2-3V, 80-150mA
  - Example models: C10-100, C13-000
- **Power Supply**: 
  - 5V for logic (ESP32/Arduino)
  - 3-5V for motors (depending on LRA specs)
  - Minimum 1A current capacity

### Why LRA Motors?

**LRA (Linear Resonant Actuator) advantages:**
- More precise haptic effects than ERM motors
- Faster response time
- Lower power consumption
- Better for sustained vibrations
- Cleaner, more distinct sensations

**vs ERM (Eccentric Rotating Mass):**
- ERM: Good for strong, buzzy vibrations
- LRA: Better for subtle, nuanced haptic illusions

## Wiring Diagram

### I2C Bus Connections

```
Microcontroller (ESP32/Arduino)
    │
    │ SDA ────────┐
    │ SCL ────────┤
    │ 3.3V/5V ────┤
    │ GND ────────┤
                  │
            ┌─────▼─────┐
            │  PCA9548A │
            │ Multiplexer│
            └─────┬─────┘
                  │
        ┌─────────┼─────────┬─────────┐
        │         │         │         │
    ┌───▼───┐ ┌──▼────┐ ┌──▼────┐ ┌──▼────┐
    │DRV2605│ │DRV2605│ │DRV2605│ │DRV2605│
    │ Ch 0  │ │ Ch 1  │ │ Ch 2  │ │ Ch 3  │
    │(FRONT)│ │(BACK) │ │(LEFT) │ │(RIGHT)│
    └───┬───┘ └───┬───┘ └───┬───┘ └───┬───┘
        │         │         │         │
    [LRA M1]  [LRA M2]  [LRA M3]  [LRA M4]
```

### Detailed Pin Connections

**Microcontroller to PCA9548A:**
```
ESP32/Arduino → PCA9548A
━━━━━━━━━━━━━━━━━━━━━━━
SDA (D21/A4)  → SDA
SCL (D22/A5)  → SCL
3.3V or 5V    → VIN
GND           → GND
```

**PCA9548A to DRV2605L Boards:**
```
PCA9548A Channel → DRV2605L → Motor Position
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Channel 0 (SD0/SC0) → DRV2605 #1 → FRONT
Channel 1 (SD1/SC1) → DRV2605 #2 → BACK
Channel 2 (SD2/SC2) → DRV2605 #3 → LEFT
Channel 3 (SD3/SC3) → DRV2605 #4 → RIGHT
```

**For Each DRV2605L:**
```
PCA9548A SD[n] → DRV2605L SDA
PCA9548A SC[n] → DRV2605L SCL
5V power rail  → DRV2605L VIN
Ground         → DRV2605L GND
DRV2605L Motor± → LRA Motor terminals
```

### Physical Motor Placement

```
         FRONT (Ch 0)
              ▲
              │
              │
   LEFT ◄─────●─────► RIGHT
   (Ch 2)   User    (Ch 3)
              │
              │
              ▼
         BACK (Ch 1)
```

**Recommended mounting:**
- **Wearable vest**: Motors at chest (front), upper back, shoulders
- **VR headset**: Motors around headband
- **Belt system**: Motors at front, back, left hip, right hip
- **Wheelchair**: Motors on chair back/armrests

## I2C Addresses

- **PCA9548A Multiplexer**: `0x70` (default)
- **DRV2605L Drivers**: `0x5A` (default, shared across all channels)

The multiplexer allows multiple DRV2605L devices (all at 0x5A) to coexist on one I2C bus by switching which channel is active.

## Software Requirements

### Required Libraries

Install via Arduino Library Manager:

```
1. Adafruit DRV2605 Library by Adafruit
2. Wire (built-in)
```

**Installation:**
1. Arduino IDE → Sketch → Include Library → Manage Libraries
2. Search "Adafruit DRV2605"
3. Click Install

### Compatible Boards

- ESP32 (recommended for more GPIO pins)
- Arduino Mega
- Arduino Uno/Nano (basic I2C support)
- Teensy 3.x/4.x
- Any microcontroller with I2C support

## Installation

1. **Build Hardware**: Follow wiring diagram above
2. **Install Libraries**: Adafruit DRV2605 library via Library Manager
3. **Upload Code**: Open `.ino` file and upload to board
4. **Open Serial Monitor**: Set baud rate to **115200**
5. **Test System**: Send commands to verify motor responses

## Usage

### Serial Commands

Open Serial Monitor at **115200 baud** and send single-character commands:

| Command | Direction | Effect |
|---------|-----------|--------|
| `f` | Forward | Creates pulling sensation forward (front motor strong) |
| `b` | Backward | Creates pulling sensation backward (back motor strong) |
| `l` | Left | Creates pulling sensation left (left motor strong) |
| `r` | Right | Creates pulling sensation right (right motor strong) |
| `s` | Stop | Stops all motors immediately |

### Example Session

```
Initializing LRA Pulling Illusion System...
DRV2605L initialized on channel 0
DRV2605L initialized on channel 1
DRV2605L initialized on channel 2
DRV2605L initialized on channel 3
System initialized!
Send: f=forward, r=right, b=back, l=left, s=stop

> f
Command: Pulling FORWARD
[Front motor activates for 7 seconds with strong pulse]
[Back motor gives 3ms counter-pulse]
[18 second rest]
[Cycle repeats]

> r
Command: Pulling RIGHT
[Right motor activates for 7 seconds]
[Left motor gives 3ms counter-pulse]

> s
Command: STOP
[All motors stop]
```

## Configuration

### Motor Positions

If your motors are connected to different channels:

```cpp
enum MotorPosition {
  FRONT = 0,  // Change to your channel
  BACK  = 1,
  LEFT  = 2,
  RIGHT = 3
};
```

### Timing Parameters

**Pulse Duration** (how long primary motor vibrates):
```cpp
const unsigned long PULSE_ON_TIME = 7000;  // 7 seconds (default)

// Adjust for different effects:
// 3000-5000ms: Shorter, more frequent cues
// 7000-10000ms: Sustained, persistent sensation
// 15000+ms: Very long guidance (may cause adaptation)
```

**Rest Period** (gap between pulses):
```cpp
const unsigned long PULSE_OFF_TIME = 18000;  // 18 seconds (default)

// Adjust for different scenarios:
// 5000-10000ms: Frequent updates (rapid navigation)
// 18000-25000ms: Periodic reminders (long-distance navigation)
// 30000+ms: Occasional cues (ambient awareness)
```

**Total cycle time:**
```cpp
const unsigned long CYCLE_TIME = PULSE_ON_TIME + PULSE_OFF_TIME;
// Default: 7000 + 18000 = 25000ms (25 seconds)
```

### Intensity Levels

Modify the intensity in `activateMotor()` function:

```cpp
// Current settings:
activateMotor(direction, 100, 7000);      // Primary: 100% for 7s
activateMotor(oppositeDir, 25, 3);        // Counter: 25% for 3ms

// Try these alternatives:

// Subtle pull:
activateMotor(direction, 75, 5000);       // 75% for 5s
activateMotor(oppositeDir, 15, 3);        // 15% for 3ms

// Strong pull:
activateMotor(direction, 100, 10000);     // 100% for 10s
activateMotor(oppositeDir, 50, 5);        // 50% for 5ms

// Gentle guidance:
activateMotor(direction, 50, 4000);       // 50% for 4s
activateMotor(oppositeDir, 10, 2);        // 10% for 2ms
```

### Waveform Selection

The code maps intensity to DRV2605L waveforms:

```cpp
uint8_t waveform;
if (intensity >= 100) waveform = 47;      // Strong Click 1
else if (intensity >= 75) waveform = 46;  // Strong Click 2
else if (intensity >= 50) waveform = 45;  // Strong Click 3
else waveform = 12;                        // Soft Fuzz

// Available DRV2605L effects for LRA (Library 6):
// 47: Strong Click 100%
// 46: Strong Click 60%
// 45: Strong Click 30%
// 12: Soft Fuzz - 60%
// 1: Sharp Click - 100%
// 10: Long Double Sharp Click Strong 1 - 100%
```

**To change waveform library:**
```cpp
drv.selectLibrary(6);  // Library 6 optimized for LRA
// Library 1: ERM motors
// Library 6: LRA motors (recommended for this project)
```

### Counter-Pulse Timing

Adjust the delay between primary and counter pulses:

```cpp
void createAsymmetricPulse(Direction direction) {
  activateMotor((MotorPosition)direction, 100, 7000);
  delay(2);  // Change this delay (milliseconds)
  activateMotor((MotorPosition)oppositeDir, 25, 3);
}

// Timing effects:
// 0-1ms: Nearly simultaneous (stronger contrast)
// 2-5ms: Slight delay (current default, balanced)
// 10-50ms: Noticeable sequence (may feel like two separate events)
// 100+ms: Too separated (loses illusion effect)
```

## Advanced Features

### Adding Diagonal Directions

Extend the system for 8-way directional control:

```cpp
enum Direction {
  DIR_FORWARD      = 0,
  DIR_RIGHT        = 3,
  DIR_BACKWARD     = 1,
  DIR_LEFT         = 2,
  DIR_FORWARD_RIGHT = 10,  // New
  DIR_FORWARD_LEFT  = 11,  // New
  DIR_BACKWARD_RIGHT = 12, // New
  DIR_BACKWARD_LEFT  = 13, // New
  DIR_STOP         = 255
};

void createAsymmetricPulse(Direction direction) {
  if (direction == DIR_STOP) return;

  // Handle diagonal directions
  if (direction == DIR_FORWARD_RIGHT) {
    activateMotor(FRONT, 100, 7000);
    activateMotor(RIGHT, 100, 7000);
    delay(2);
    activateMotor(BACK, 25, 3);
    activateMotor(LEFT, 25, 3);
  }
  // ... similar for other diagonals
  else {
    // Original cardinal direction logic
    Direction oppositeDir = (Direction)((direction + 2) % 4);
    activateMotor((MotorPosition)direction, 100, 7000);
    delay(2);
    activateMotor((MotorPosition)oppositeDir, 25, 3);
  }
}

// Add serial command handling:
case 'q': startPullingForce(DIR_FORWARD_RIGHT); break;
case 'e': startPullingForce(DIR_FORWARD_LEFT); break;
// etc...
```

### Variable Intensity Control

Add dynamic intensity adjustment via serial:

```cpp
int globalIntensity = 100;  // Global intensity level

void handleSerialCommands() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    // Handle intensity command: "intensity 75"
    if (input.startsWith("intensity ")) {
      globalIntensity = input.substring(10).toInt();
      globalIntensity = constrain(globalIntensity, 25, 100);
      Serial.print("Intensity set to: ");
      Serial.println(globalIntensity);
      return;
    }
    
    // Handle direction commands
    char command = input.charAt(0);
    // ... rest of command handling
  }
}

void createAsymmetricPulse(Direction direction) {
  activateMotor((MotorPosition)direction, globalIntensity, 7000);
  delay(2);
  activateMotor((MotorPosition)oppositeDir, globalIntensity / 4, 3);
}
```

### GPS Integration for Navigation

```cpp
#include <TinyGPS++.h>

TinyGPSPlus gps;
double targetLat, targetLon;  // Destination coordinates

void loop() {
  handleSerialCommands();
  updateGPSData();
  
  if (pullingActive) {
    Direction heading = calculateBearing();
    createAsymmetricPulse(heading);
  }
}

Direction calculateBearing() {
  double currentLat = gps.location.lat();
  double currentLon = gps.location.lng();
  
  double bearing = TinyGPSPlus::courseTo(
    currentLat, currentLon,
    targetLat, targetLon
  );
  
  // Convert bearing (0-360°) to Direction
  if (bearing >= 315 || bearing < 45) return DIR_FORWARD;
  if (bearing >= 45 && bearing < 135) return DIR_RIGHT;
  if (bearing >= 135 && bearing < 225) return DIR_BACKWARD;
  return DIR_LEFT;
}
```

### Bluetooth/WiFi Control (ESP32)

```cpp
#include <BluetoothSerial.h>
BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("HapticNav");  // Bluetooth device name
  
  // ... rest of setup
}

void handleSerialCommands() {
  // Check both Serial and Bluetooth
  if (Serial.available()) {
    processCommand(Serial.read());
  }
  if (SerialBT.available()) {
    processCommand(SerialBT.read());
  }
}

void processCommand(char command) {
  switch (command) {
    case 'f': startPullingForce(DIR_FORWARD); break;
    // ... etc
  }
}
```

### Adaptive Timing Based on Movement

```cpp
#include <Wire.h>
#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 mpu;
bool userMoving = false;

void loop() {
  detectMovement();
  
  if (userMoving && pullingActive) {
    // Shorter cycles when user is moving
    PULSE_ON_TIME = 3000;   // 3 seconds
    PULSE_OFF_TIME = 7000;  // 7 seconds
  } else {
    // Longer cycles when stationary
    PULSE_ON_TIME = 7000;   // 7 seconds
    PULSE_OFF_TIME = 18000; // 18 seconds
  }
  
  updatePullingForce();
}

void detectMovement() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  float accelMagnitude = sqrt(
    a.acceleration.x * a.acceleration.x +
    a.acceleration.y * a.acceleration.y +
    a.acceleration.z * a.acceleration.z
  );
  
  userMoving = (abs(accelMagnitude - 9.8) > 1.0);  // Threshold
}
```

## Troubleshooting

### "Failed to initialize PCA9548A!"

**Causes:**
- Incorrect I2C wiring
- Wrong I2C address
- Power supply issue
- Pull-up resistor problems

**Solutions:**
```cpp
// Run I2C scanner to detect devices
void scanI2C() {
  Serial.println("Scanning I2C bus...");
  for (byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found device at 0x");
      Serial.println(addr, HEX);
    }
  }
}
```
- Should detect PCA9548A at 0x70
- Add 2.2kΩ - 4.7kΩ pull-up resistors to SDA/SCL if needed

### "Failed to initialize DRV2605L on channel X"

**Check:**
1. DRV2605L connected to correct multiplexer channel
2. DRV2605L has power (LED indicator on board)
3. I2C address is 0x5A (default)
4. LRA motor properly connected

**Test individual channel:**
```cpp
void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Test specific channel
  selectI2CChannel(0);  // Test channel 0
  
  if (drv.begin()) {
    Serial.println("Channel 0 OK!");
  } else {
    Serial.println("Channel 0 FAILED!");
  }
}
```

### Motors Don't Vibrate

**Verify motor type setting:**
```cpp
void initializeAllDRV2605L() {
  // Make sure this is set correctly
  drv.useLRA();  // For LRA motors (this code)
  // drv.useERM();  // For ERM motors (wrong for this project)
}
```

**Check motor connections:**
- LRA motors have polarity (unlike ERM motors)
- Try swapping motor wires if no vibration
- Test motor with multimeter (should show ~10-50Ω resistance)

**Test with simple waveform:**
```cpp
void testMotor(uint8_t channel) {
  selectI2CChannel(channel);
  drv.setWaveform(0, 47);  // Strong click
  drv.setWaveform(1, 0);
  drv.go();
  delay(1000);
}
```

### Weak or Inconsistent Vibration

**Increase intensity:**
```cpp
activateMotor((MotorPosition)direction, 100, 7000);  // Max intensity
```

**Try different waveforms:**
```cpp
// Instead of auto-selection, force strong waveform
drv.setWaveform(0, 47);  // Strongest LRA effect
```

**Check power supply:**
- LRA motors need stable voltage
- Verify 3-5V supply (check motor datasheet)
- Ensure adequate current (150mA per motor minimum)

### Pulling Illusion Doesn't Feel Right

**Timing is critical for this effect:**

```cpp
// If pull feels weak:
PULSE_ON_TIME = 10000;    // Longer primary pulse
PULSE_OFF_TIME = 15000;   // Shorter rest

// If pull feels overwhelming:
PULSE_ON_TIME = 5000;     // Shorter primary pulse
PULSE_OFF_TIME = 25000;   // Longer rest

// If direction is unclear:
activateMotor(direction, 100, 7000);      // Stronger primary
activateMotor(oppositeDir, 50, 5);        // Stronger counter-pulse
```

**Motor placement matters:**
- Motors must be clearly at FRONT, BACK, LEFT, RIGHT positions
- Motors too close together reduce directional clarity
- Test with motors on flat surface first before mounting

### Only Some Motors Work

**Test each channel individually:**
```cpp
void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  for (int ch = 0; ch < 4; ch++) {
    Serial.print("Testing channel ");
    Serial.println(ch);
    
    selectI2CChannel(ch);
    if (drv.begin()) {
      drv.setWaveform(0, 47);
      drv.go();
      delay(1000);
      Serial.println(" -> OK");
    } else {
      Serial.println(" -> FAILED");
    }
    delay(500);
  }
}
```

### Serial Commands Not Responding

**Check baud rate:**
- Must be 115200 in Serial Monitor

**Test communication:**
```cpp
void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    Serial.print("Received: ");
    Serial.println(c);
  }
}
```

**Verify command format:**
- Send single character only: `f` not `forward`
- No line breaks or spaces needed

## Applications

### Navigation & Wayfinding

**GPS Navigation:**
- Long-distance hiking with haptic compass
- Urban navigation without looking at phone
- Boat/maritime navigation

**Indoor Navigation:**
- Museum/gallery wayfinding
- Hospital/airport guidance
- Warehouse worker direction assistance

### Accessibility

**Visual Impairment:**
- Persistent directional guidance
- Obstacle avoidance cues
- Path-keeping assistance

**Cognitive Support:**
- Memory aids for route following
- Attention direction for task completion

### Virtual Reality / Gaming

**VR Navigation:**
- Open-world exploration guidance
- Quest objective direction
- Environmental force simulation (wind, gravity)

**Gaming:**
- Stealth game proximity alerts
- Racing optimal line guidance
- Flight simulator trim forces

### Training & Simulation

**Flight Training:**
- Persistent force feedback for control inputs
- Trim indication
- Heading correction cues

**Driving Simulation:**
- Lane-keeping assistance feedback
- Steering correction
- Vehicle dynamics simulation

### Medical & Therapy

**Balance Training:**
- Postural correction cues
- Gait training feedback
- Vestibular rehabilitation

**Physical Therapy:**
- Movement direction coaching
- Range of motion guidance

### Safety & Industrial

**Hazard Awareness:**
- Directional danger alerts (approaching vehicles, machinery)
- Evacuation guidance in emergencies

**Hands-Free Operation:**
- Machinery operation guidance
- Drone piloting feedback

### Research Applications

**Perceptual Studies:**
- Haptic illusion research
- Multisensory integration studies
- Spatial cognition experiments

**Human Factors:**
- Attention direction studies
- Workload reduction research

## Technical Specifications

### Timing Characteristics

- **Primary Pulse Duration**: 7000ms (7 seconds, configurable)
- **Counter-Pulse Duration**: 3ms (configurable)
- **Counter-Pulse Delay**: 2ms after primary start
- **Rest Period**: 18000ms (18 seconds, configurable)
- **Total Cycle Time**: 25000ms (25 seconds)
- **Command Response**: <10ms

### Power Consumption

- **Idle**: 50-80mA (microcontroller + multiplexer)
- **Single LRA Active**: 80-150mA per motor
- **During Primary Pulse**: 150-250mA (one motor at high intensity)
- **Peak (all motors)**: 400-600mA (rare, only during diagonal pulls)
- **Recommended Supply**: 5V 1.5A minimum

### DRV2605L Specifications

- **Supply Voltage**: 2.5V - 5.2V
- **I2C Address**: 0x5A (fixed)
- **Max Motor Current**: 250mA
- **LRA Frequency Range**: 45Hz - 300Hz
- **Effect Library**: 123 waveforms (Library 6 for LRA)

### PCA9548A Specifications

- **Supply Voltage**: 2.3V - 5.5V
- **I2C Address**: 0x70 - 0x77 (selectable)
- **Channels**: 8 independent I2C buses
- **Max I2C Speed**: 400kHz

## Comparison with Other Haptic Systems

### vs. Sequential Wave Pattern

**Sequential Wave:**
- Short pulses (30ms) across multiple motors
- Rapid progression (180ms total)
- Good for: Quick directional taps, gaming

**Pulling Illusion:**
- Long pulse (7s) with brief counter
- Extended cycles (25s total)
- Good for: Sustained guidance, navigation

### vs. Continuous Pulsing

**Continuous Pulsing:**
- Repeating on/off every 200-400ms
- Attention-grabbing
- Good for: Alerts, notifications

**Pulling Illusion:**
- Single long pulse per cycle
- Subtle, persistent sensation
- Good for: Background guidance, ambient cues

### vs. Simple PWM Control

**PWM Control:**
- Direct motor control, no effects library
- Requires manual timing
- Good for: Learning, prototyping, budget builds

**DRV2605L System:**
- Professional haptic effects
- Precise waveform control
- Good for: Production devices, research, optimal perception

## Performance Optimization

### Reducing Latency

```cpp
// Current: Using delays
delay(2);
drv.go();
delay(durationMs);

// Optimized: Non-blocking
unsigned long startTime = millis();
drv.go();
while (millis() - startTime < durationMs) {
  // Can handle other tasks here
}
```

### Power Saving

```cpp
// Sleep mode between cycles
void updatePullingForce() {
  if (!pullingActive) {
    // Put system in low power mode
    esp_sleep_enable_timer_wakeup(CYCLE_TIME * 1000);
    esp_light_sleep_start();
    return;
  }
  // ... rest of function
}
```

### Memory Optimization

```cpp
// Use PROGMEM for constant arrays if memory-constrained
const uint8_t waveforms[] PROGMEM = {47, 46, 45, 12};
```

## Safety & Usage Guidelines

**Important Safety Information:**

**Usage Duration:**
- Take 10-minute breaks every hour
- Prolonged haptic stimulation can cause sensory adaptation
- Excessive use may lead to skin irritation

**Intensity:**
- Start with lower intensities (50-75%)
- Gradually increase to find comfortable level
- Never exceed pain threshold

**Medical Considerations:**
- Consult physician if you have:
  - Pacemaker or implanted medical devices
  - Neurological conditions
  - Skin sensitivities
- Not recommended for:
  - Pregnant individuals (without medical approval)
  - Individuals with epilepsy (consult doctor)

**Mounting:**
- Ensure secure motor attachment
- Avoid placing motors over sensitive areas
- Use breathable materials for prolonged wear

**Electrical Safety:**
- Verify all connections before powering on
- Use appropriate wire gauge for current requirements
- Keep system dry (no water exposure)

## License

This project is open source and available under the MIT License.

## Contributing

Contributions welcome! Areas for improvement:
- Advanced psychophysical calibration algorithms
- Machine learning for personalized intensity
- Additional directional illusion patterns
- Mobile app integration
- Real-time navigation system integration

## Resources

### Documentation
- [DRV2605L Datasheet](https://www.ti.com/lit/ds/symlink/drv2605l.pdf)
- [PCA9548A Datasheet](https://www.ti.com/lit/ds/symlink/pca9548a.pdf)
- [Adafruit DRV2605 Guide](https://learn.adafruit.com/adafruit-drv2605-haptic-controller-breakout)
- [LRA Motor Selection Guide](https://www.precisionmicrodrives.com/lra-linear-resonant-actuators)

### Research Papers
- "Asymmetric Vibration: A New Method for Conveying Direction" - research on haptic illusions
- "Haptic Navigation for the Visually Impaired"
- "Psychophysics of Directional Haptic Perception"

### Tools
- [I2C Scanner Sketch](https://playground.arduino.cc/Main/I2cScanner/)
- [TI EVM Software](https://www.ti.com/tool/DRV2605LEVM-CT) - For testing DRV2605L effects

---

**Note**: This system creates a perceptual illusion through carefully timed asymmetric haptic stimulation. Results may vary between individuals. Experimentation with timing parameters is encouraged to optimize for your specific application and user population.