#include <Wire.h>
#include <Adafruit_DRV2605.h>

// --- I2C Device Configuration ---
#define TCA_I2C_ADDRESS 0x70
#define MOTOR_DRIVER_I2C_ADDRESS 0x5A

// --- User Channel Mapping ---
#define UP_MOTOR_CHANNEL    3
#define DOWN_MOTOR_CHANNEL  4
#define LEFT_MOTOR_CHANNEL  0
#define RIGHT_MOTOR_CHANNEL 1
const int motor_channels[] = {UP_MOTOR_CHANNEL, DOWN_MOTOR_CHANNEL, LEFT_MOTOR_CHANNEL, RIGHT_MOTOR_CHANNEL};

// --- Haptic Effect Parameters ---
const uint8_t EFFECT_STRONG_CLICK = 47; // Strong Click 1
const uint8_t EFFECT_WEAK_CLICK   = 21; // Soft Bump 2

// --- Object Creation ---
Adafruit_DRV2605 drv = Adafruit_DRV2605();
char currentDirection = 0;

/**
 * @brief Select channel on TCA9548A multiplexer
 */
void tcaselect(uint8_t i) {
  if (i > 7) return;
  
  Wire.beginTransmission(TCA_I2C_ADDRESS);
  Wire.write(1 << i);
  Wire.endTransmission();
  delay(10); // Increased delay for better stability
}

/**
 * @brief Initialize DRV2605 on specific channel with consistent settings
 */
bool initializeMotorDriver(uint8_t channel) {
  tcaselect(channel);
  
  if (!drv.begin()) {
    Serial.print("Failed to find DRV2605 on channel ");
    Serial.println(channel);
    return false;
  }
  
  // Consistent initialization for all motors
  drv.selectLibrary(1);
  drv.setMode(DRV2605_MODE_INTTRIG);
  drv.useERM(); // Explicitly set motor type - change to useLRA() if using Linear Resonant Actuators
  
  // Set consistent drive time and other parameters
  drv.setWaveform(0, 0); // Clear any existing waveforms
  drv.setWaveform(1, 0);
  
  return true;
}

/**
 * @brief Trigger haptic effect with enhanced error checking
 */
void triggerMotor(uint8_t channel, uint8_t effect) {
  tcaselect(channel);
  
  // Re-initialize to ensure consistent state
  if (!initializeMotorDriver(channel)) {
    return;
  }
  
  drv.setWaveform(0, effect);
  drv.setWaveform(1, 0); // End sequence
  drv.go();
  
  Serial.print("Triggered effect ");
  Serial.print(effect);
  Serial.print(" on channel ");
  Serial.println(channel);
}

/**
 * @brief Stop motor with proper cleanup
 */
void stopMotor(uint8_t channel) {
  tcaselect(channel);
  if (drv.begin()) {
    drv.stop();
    drv.setWaveform(0, 0); // Clear waveform
  }
}

/**
 * @brief Stop all motors
 */
void stopAllMotors() {
  for (int i = 0; i < 4; i++) {
    stopMotor(motor_channels[i]);
  }
}

/**
 * @brief Test individual motor function
 */
void testMotor(uint8_t channel, const char* direction) {
  Serial.print("Testing ");
  Serial.print(direction);
  Serial.print(" motor (channel ");
  Serial.print(channel);
  Serial.println(")");
  
  triggerMotor(channel, EFFECT_STRONG_CLICK);
  delay(1000); // Wait between tests
}

// --- Directional Pull Functions ---
void pullUp() {
  Serial.println("Pulling ↑ Up");
  triggerMotor(UP_MOTOR_CHANNEL, EFFECT_STRONG_CLICK);
  delay(40);
  triggerMotor(DOWN_MOTOR_CHANNEL, EFFECT_WEAK_CLICK);
}

void pullDown() {
  Serial.println("Pulling ↓ Down");
  triggerMotor(DOWN_MOTOR_CHANNEL, EFFECT_STRONG_CLICK);
  delay(40);
  triggerMotor(UP_MOTOR_CHANNEL, EFFECT_WEAK_CLICK);
}

void pullLeft() {
  Serial.println("Pulling ← Left");
  triggerMotor(LEFT_MOTOR_CHANNEL, EFFECT_STRONG_CLICK);
  delay(40);
  triggerMotor(RIGHT_MOTOR_CHANNEL, EFFECT_WEAK_CLICK);
}

void pullRight() {
  Serial.println("Pulling → Right");
  triggerMotor(RIGHT_MOTOR_CHANNEL, EFFECT_STRONG_CLICK);
  delay(40);
  triggerMotor(LEFT_MOTOR_CHANNEL, EFFECT_WEAK_CLICK);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("\nI2C Haptic Driver with Multiplexer Setup");
  Wire.begin();
  Serial.println("I2C Bus Initialized!");

  // Initialize all motor drivers
  for (int i = 0; i < 4; i++) {
    int channel = motor_channels[i];
    Serial.print("Initializing DRV2605 driver on MUX channel ");
    Serial.print(channel);
    
    if (initializeMotorDriver(channel)) {
      Serial.println("... OK!");
    } else {
      Serial.println("... FAILED!");
    }
  }
  
  Serial.println("Driver setup complete.");
  Serial.println("Commands: u/d/l/r for direction, 's' to stop, 't' to test all motors");
}

void loop() {
  if (Serial.available()) {
    char dir = Serial.read();
    
    // Handle test command
    if (dir == 't') {
      Serial.println("Testing all motors individually...");
      testMotor(UP_MOTOR_CHANNEL, "UP");
      testMotor(DOWN_MOTOR_CHANNEL, "DOWN");
      testMotor(LEFT_MOTOR_CHANNEL, "LEFT");
      testMotor(RIGHT_MOTOR_CHANNEL, "RIGHT");
      Serial.println("Test complete.");
      return;
    }
    
    if (dir == 'u' || dir == 'd' || dir == 'l' || dir == 'r' || dir == 's') {
      currentDirection = dir;
      Serial.print("Direction set to: ");
      Serial.println(dir);
    }
  }

  if (currentDirection) {
    switch (currentDirection) {
      case 'u': pullUp(); break;
      case 'd': pullDown(); break;
      case 'l': pullLeft(); break;
      case 'r': pullRight(); break;
      case 's':
        stopAllMotors();
        currentDirection = 0;
        Serial.println("Motors stopped.");
        break;
    }
    delay(500);
  }
}