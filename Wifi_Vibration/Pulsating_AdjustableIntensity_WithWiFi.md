# ESP32 WiFi-Enabled 8-Direction Haptic Vibration Controller

Advanced haptic feedback system using ESP32 microcontroller with WiFi connectivity, supporting 8-directional control (4 cardinal + 4 diagonal directions) with adaptive pulse timing and manual intensity adjustment.

## Hardware Requirements

- **ESP32 Development Board** (Arduino Nano ESP32, ESP32-WROOM-32 or similar)
- 6x vibration motors
- Motor driver circuit (L293D, L298N, or transistor array)
- Appropriate power supply for motors (typically 3-5V depending on motors)
- USB cable for programming
- WiFi network access

## Pin Configuration

### Motor Assignment
- **Pin 3**: Top-Left motor
- **Pin 5**: Top-Right motor
- **Pin 6**: Left motor
- **Pin 12**: Right motor
- **Pin 2**: Bottom-Left motor
- **Pin 4**: Bottom-Right motor

### Motor Layout Diagram
```
        3 ●━━━━━━━━━● 5
        ┃   (TOP)    ┃
        ┃            ┃
    6 ● ┃            ┃ ● 12
(LEFT)  ┃            ┃  (RIGHT)
        ┃            ┃
        ┃  (BOTTOM)  ┃
        2 ●━━━━━━━━━● 4
```

## Features

- **WiFi Connectivity**: ESP32 connects to WiFi network on startup
- **8-Direction Control**: Cardinal (up/down/left/right) + Diagonal (up_right/up_left/down_right/down_left)
- **Adaptive Pulse Timing**: Pulse duration and interval automatically adjust based on intensity
- **Manual Intensity Control**: Set vibration strength via serial commands (0-255)
- **Smooth Fading**: Gradual intensity transitions for natural feel
- **Hardware PWM**: Uses ESP32's LEDC peripheral for stable, flicker-free motor control
- **Non-blocking Operation**: Responsive system using `millis()` timing

## Setup

### 1. Hardware Connections

Connect motors through appropriate motor driver circuit:

```
ESP32 Pin 3  → [Driver] → Motor 1 (Top-Left)
ESP32 Pin 5  → [Driver] → Motor 2 (Top-Right)
ESP32 Pin 6  → [Driver] → Motor 3 (Left)
ESP32 Pin 12 → [Driver] → Motor 4 (Right)
ESP32 Pin 2  → [Driver] → Motor 5 (Bottom-Left)
ESP32 Pin 4  → [Driver] → Motor 6 (Bottom-Right)
```

### 2. WiFi Configuration

Update WiFi credentials in the code:

```cpp
const char* ssid = "YourNetworkName";
const char* password = "YourPassword";
```

### 3. Upload Code

1. Install ESP32 board support in Arduino IDE
2. Select your ESP32 board (typically "ESP32 Dev Module")
3. Connect ESP32 via USB
4. Upload the sketch
5. Open Serial Monitor (9600 baud)

### 4. Verify Connection

Serial Monitor should display:
```
Connecting to Wi-Fi...
...
Wi-Fi connected!
IP Address: 192.168.1.XXX
Enter direction (up, down, left, right, up_right, up_left, down_right, down_left). 
Use 'intensity [value]' to control intensity.
```

## Usage

### Direction Commands

Enter commands in Serial Monitor:

**Cardinal Directions:**
- `up` - Activates top motors (pins 3, 5)
- `down` - Activates bottom motors (pins 2, 4)
- `left` - Activates left motor (pin 6)
- `right` - Activates right motor (pin 12)

**Diagonal Directions:**
- `up_right` - Activates top-right motors (pins 3, 12)
- `up_left` - Activates top-left motors (pins 5, 6)
- `down_right` - Activates bottom-right motors (pins 4, 12)
- `down_left` - Activates bottom-left motors (pins 2, 6)

### Intensity Control

Set vibration strength (0-255):

```
intensity 50    → Very weak vibration
intensity 100   → Weak vibration
intensity 150   → Medium vibration (default)
intensity 200   → Strong vibration
intensity 255   → Maximum vibration
```

### Example Session

```
> up
Pulsing motor: 3 with intensity: 150
Pulsing motor: 5 with intensity: 150

> intensity 200
Intensity set to: 200

> down_right
Pulsing motor: 4 with intensity: 200
Pulsing motor: 12 with intensity: 200
```

## Adaptive Pulse Timing

The system automatically adjusts pulse timing based on intensity for optimal haptic feedback:

### Low Intensity (0-50)
```
Motor ON:  50ms   ███
Motor OFF: 200ms  ________
Pattern: Quick taps, long pauses
```

### Medium Intensity (100-150)
```
Motor ON:  125ms  ████████
Motor OFF: 300ms  ____________
Pattern: Balanced pulses
```

### High Intensity (200-255)
```
Motor ON:  200ms  ████████████
Motor OFF: 400ms  ________________
Pattern: Strong sustained pulses
```

**Formula:**
- Pulse Duration = map(intensity, 0→255, 50→200 ms)
- Pulse Interval = map(intensity, 0→255, 200→400 ms)

## Direction-to-Motor Mapping

| Direction | Active Motors | Pins |
|-----------|--------------|------|
| **up** | Top (both) | 3, 5 |
| **down** | Bottom (both) | 2, 4 |
| **left** | Left | 6 |
| **right** | Right | 12 |
| **up_right** | Top-Right + Right | 3, 12 |
| **up_left** | Top-Left + Left | 5, 6 |
| **down_right** | Bottom-Right + Right | 4, 12 |
| **down_left** | Bottom-Left + Left | 2, 6 |

## Code Parameters

Customize these constants for different behavior:

```cpp
// WiFi Settings
const char* ssid = "HCI-Group";           // Network name
const char* password = "HCI95343";        // Network password

// Motor Settings
const int pwmFreq = 5000;                 // PWM frequency (Hz)
const int pwmResolution = 8;              // PWM resolution (8-bit = 0-255)

// Intensity
int intensityLevel = 150;                 // Default intensity
const int maxIntensity = 255;             // Maximum intensity

// Timing (base values before adaptive scaling)
unsigned long pulseDuration = 200;        // Base ON time
unsigned long pulseInterval = 400;        // Base OFF time
const unsigned long fadeSpeed = 20;       // Intensity fade speed (ms/step)
```

## Advanced Customization

### Adjust PWM Frequency

Higher frequency = smoother vibration, lower frequency = more responsive:

```cpp
const int pwmFreq = 1000;   // Lower frequency (more responsive)
const int pwmFreq = 10000;  // Higher frequency (smoother)
```

### Change Fade Speed

```cpp
const unsigned long fadeSpeed = 10;   // Faster transitions
const unsigned long fadeSpeed = 50;   // Slower, more gradual
```

### Modify Adaptive Timing Range

```cpp
// In loop(), change these lines:
pulseDuration = map(currentIntensity, 0, maxIntensity, 30, 250);  // Wider range
pulseInterval = map(currentIntensity, 0, maxIntensity, 150, 500); // Longer pauses
```

## Technical Specifications

- **Microcontroller**: ESP32 (Dual-core Xtensa LX6, 240MHz)
- **WiFi**: 802.11 b/g/n (2.4 GHz)
- **PWM Channels**: 6 (LEDC peripheral)
- **PWM Frequency**: 5000 Hz (configurable)
- **PWM Resolution**: 8-bit (0-255)
- **Serial Baud Rate**: 9600
- **Timing Method**: Non-blocking (millis())
- **Max Motor Current per Pin**: 40mA (use driver circuit!)

## Safety & Best Practices

**Critical Safety Information**:

### Motor Driver Required
- **NEVER connect motors directly to ESP32 pins**
- ESP32 pins can only source ~40mA max
- Vibration motors typically draw 50-150mA
- Direct connection **WILL damage your ESP32**

### Recommended Driver Circuits

**Option 1: L293D Motor Driver IC**
```
ESP32 → L293D Input → L293D Output → Motor
VCC1 (5V logic) + VCC2 (motor voltage)
```

**Option 2: NPN Transistor Array (ULN2003)**
```
ESP32 Pin → ULN2003 Input → Motor → Power Supply
Built-in flyback diodes
```

**Option 3: Individual Transistors (per motor)**
```
ESP32 → 1kΩ → [2N2222 Base]
                [Collector] → Motor(+) → VCC
                [Emitter] → GND
Motor(-) → [1N4001 diode] → GND
```

### Power Supply Guidelines
- Use separate power supply for motors (don't power from ESP32)
- Common ground between ESP32 and motor supply
- Adequate current capacity (6 motors × ~100mA = ~600mA minimum)
- Add decoupling capacitors (100µF) near motor supply

## WiFi Configuration Options

### Static IP (Optional)

Add to `setup()` before `WiFi.begin()`:

```cpp
IPAddress local_IP(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
}
```

### Multiple Network Support

```cpp
const char* networks[][2] = {
    {"Network1", "Password1"},
    {"Network2", "Password2"}
};

// Try each network in setup()
```

## Troubleshooting

### ESP32 Won't Connect to WiFi
- ✓ Check SSID and password are correct
- ✓ Verify 2.4GHz WiFi (ESP32 doesn't support 5GHz)
- ✓ Check router allows new devices
- ✓ Move ESP32 closer to router
- ✓ Check Serial Monitor for error messages

### Motors Not Vibrating
- ✓ Verify motor driver circuit connections
- ✓ Check power supply is adequate (voltage & current)
- ✓ Test motors individually with battery
- ✓ Confirm pin assignments match your wiring
- ✓ Check Serial Monitor for "Pulsing motor" messages

### Erratic Motor Behavior
- ✓ Add decoupling capacitors (100µF + 0.1µF)
- ✓ Ensure common ground between ESP32 and motors
- ✓ Check for loose connections
- ✓ Verify power supply is stable under load
- ✓ Add flyback diodes across motors

### Commands Not Working
- ✓ Serial Monitor baud rate must be 9600
- ✓ Commands are case-sensitive (use lowercase)
- ✓ Check for extra spaces or characters
- ✓ Use underscore in diagonal commands (up_right, not "up right")

### Intensity Not Changing Smoothly
- ✓ Verify `fadeSpeed` parameter (adjust if needed)
- ✓ Check intensity is within 0-255 range
- ✓ Ensure motors can respond to PWM changes

### WiFi Disconnects Randomly
- ✓ Add WiFi reconnection code in loop()
- ✓ Check router stability
- ✓ Increase router transmit power if possible
- ✓ Use static IP to avoid DHCP issues

## Applications

This 8-direction haptic controller is ideal for:

- **VR/AR Gaming**: Directional damage indicators, environmental feedback
- **Haptic Vests**: Full-body immersive experiences
- **Training Simulators**: Tactical feedback, guidance systems
- **Accessibility Devices**: Spatial navigation for visually impaired
- **Entertainment**: 4D cinema experiences, haptic music
- **Medical Devices**: Physical therapy, sensory rehabilitation
- **Automotive**: Driver alert systems, parking sensors
- **Art Installations**: Interactive haptic displays

## Future Enhancements

### Web Server Control (WiFi)

Add web server for remote control:

```cpp
#include <WebServer.h>
WebServer server(80);

// In setup():
server.on("/direction", handleDirection);
server.begin();

// In loop():
server.handleClient();
```

### Bluetooth Control

Add Bluetooth for mobile app control:

```cpp
#include <BluetoothSerial.h>
BluetoothSerial BT;

// Replace Serial commands with BT commands
```

### Pattern Sequences

Add pre-programmed patterns:

```cpp
void circlePattern() {
    executeDirection("up", 150, 300);
    executeDirection("up_right", 150, 300);
    executeDirection("right", 150, 300);
    executeDirection("down_right", 150, 300);
    // ... continue circle
}
```

### MQTT Integration

Connect to IoT platforms:

```cpp
#include <PubSubClient.h>
// Subscribe to MQTT topics for remote control
```

## Comparison with Other Controllers

| Feature | This (ESP32) | Arduino Adaptive | Arduino Sequential |
|---------|--------------|------------------|-------------------|
| **Platform** | ESP32 | Arduino Uno | Arduino Uno |
| **WiFi** | Built-in | No | No |
| **Directions** | 8 (with diagonals) | 4 (cardinal) | 4 (cardinal) |
| **Motors** | 6 | 5 | 5 |
| **Intensity** | Serial command | Potentiometer | Potentiometer |
| **Pulse Pattern** | Adaptive | Adaptive | Sequential |
| **PWM Method** | Hardware (LEDC) | Software (analogWrite) | Software |
| **Complexity** | High | Medium | High |
| **Use Case** | Professional/VR | General haptics | Navigation |

## Example Projects

### 1. VR Gaming Damage Indicator
```cpp
// In game engine, send serial command based on damage direction
// Example: Player hit from upper-right
// Send: "up_right\n" with "intensity 200\n"
```

### 2. Navigation Aid for Visually Impaired
```cpp
// GPS provides direction to destination
// Convert to haptic feedback
if (turnLeft) sendCommand("left");
if (turnRight) sendCommand("right");
if (goStraight) sendCommand("up");
```

### 3. Meditation/Focus Device
```cpp
// Breathing pattern guidance
void breathingPattern() {
    // Inhale - rising sensation
    for (int i = 0; i < 4; i++) {
        executeDirection("up", 100, 1000);
    }
    // Exhale - falling sensation
    for (int i = 0; i < 4; i++) {
        executeDirection("down", 100, 1000);
    }
}
```

## ESP32 Board Setup (Arduino IDE)

1. Open Arduino IDE
2. Go to **File → Preferences**
3. Add to **Additional Board Manager URLs**:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Go to **Tools → Board → Board Manager**
5. Search for "ESP32"
6. Install "ESP32 by Espressif Systems"
7. Select **Tools → Board → ESP32 Arduino → ESP32 Dev Module**

## License

MIT License - feel free to use and modify for your projects.

## Credits & Acknowledgments

- ESP32 Arduino Core by Espressif Systems
- WiFi library for ESP32
- LEDC (LED Control) peripheral for PWM

## Version History

- **v1.0**: Initial release with 8-direction WiFi control
- ESP32-based haptic feedback system
- Adaptive pulse timing
- Manual intensity control via serial commands

---

**Note**: This is an advanced project requiring proper electronics knowledge. Always use appropriate motor driver circuits and never exceed current ratings of your ESP32 board.