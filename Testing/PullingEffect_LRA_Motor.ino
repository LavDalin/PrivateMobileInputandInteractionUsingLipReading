#include <Wire.h>
#include <Adafruit_DRV2605.h>

#define PCA9548A_ADDRESS 0x70

Adafruit_DRV2605 drv;

char currentDirection = 0;

// Motor calibration - now using overdrive values (0-255) for intensity control
// All motors use the same waveform (47 = strong click)
struct MotorCalibration {
  uint8_t strong;  // Overdrive value for strong effect (0-255)
  uint8_t weak;    // Overdrive value for weak effect (0-255)
};

// Calibration for each motor (adjust overdrive values for intensity)
MotorCalibration motorCal[5] = {
  {0, 0},       // Channel 0 (unused)
  {60, 30},     // Channel 1 (Down) - reduce since it's strongest
  {255, 120},   // Channel 2 (Right) - max overdrive since it's weakest
  {120, 60},    // Channel 3 (Up) - good at 120
  {120, 60}     // Channel 4 (Left) - good at 120
};

void selectChannel(uint8_t channelMask) {
  Wire.beginTransmission(PCA9548A_ADDRESS);
  Wire.write(channelMask);
  Wire.endTransmission();
  delay(5);
}

void triggerMotor(uint8_t channel, bool strongEffect) {
  selectChannel(1 << channel);
  drv.begin();
  drv.selectLibrary(1);               // LRA mode
  drv.setMode(DRV2605_MODE_INTTRIG);
  
  // Use same waveform (47 = strong click) for all motors
  drv.setWaveform(0, 47);
  drv.setWaveform(1, 0);              // End sequence
  
  // Control intensity by adjusting the overdrive voltage
  uint8_t intensity = strongEffect ? motorCal[channel].strong : motorCal[channel].weak;
  drv.writeRegister8(DRV2605_REG_OVERDRIVE, intensity);
  
  drv.go();
}

void pullUp() {
  Serial.println("Pulling ↑ Up");
  triggerMotor(3, true);   // Up motor (strong)
  delay(40);
  triggerMotor(1, false);  // Down motor (weak)
}

void pullDown() {
  Serial.println("Pulling ↓ Down");
  triggerMotor(1, true);   // Down motor (strong)
  delay(40);
  triggerMotor(3, false);  // Up motor (weak)
}

void pullLeft() {
  Serial.println("Pulling ← Left");
  triggerMotor(4, true);   // Left motor (strong)
  delay(40);
  triggerMotor(2, false);  // Right motor (weak)
}

void pullRight() {
  Serial.println("Pulling → Right");
  triggerMotor(2, true);   // Right motor (strong)
  delay(40);
  triggerMotor(4, false);  // Left motor (weak)
}

// Calibration function - now tests overdrive values with effect 47
void testMotor(uint8_t channel, uint8_t overdrive) {
  Serial.print("Testing motor ");
  Serial.print(channel);
  Serial.print(" with overdrive ");
  Serial.println(overdrive);
  
  selectChannel(1 << channel);
  drv.begin();
  drv.selectLibrary(1);
  drv.setMode(DRV2605_MODE_INTTRIG);
  drv.setWaveform(0, 47);  // Always use effect 47 (strong click)
  drv.setWaveform(1, 0);
  drv.writeRegister8(DRV2605_REG_OVERDRIVE, overdrive);
  drv.go();
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Haptic Motor Controller Ready");
  Serial.println("Commands:");
  Serial.println("  u/d/l/r - Set direction");
  Serial.println("  s - Stop");
  Serial.println("  t[channel][overdrive] - Test motor (e.g., 't1100' tests motor 1 with overdrive 100)");
  Serial.println("  c - Show current calibration");
}

void showCalibration() {
  Serial.println("Current Motor Calibration:");
  Serial.println("Channel | Strong | Weak | (Overdrive values 0-255)");
  Serial.println("--------|-------|------");
  for (int i = 1; i <= 4; i++) {
    Serial.print("   ");
    Serial.print(i);
    Serial.print("    |   ");
    Serial.print(motorCal[i].strong);
    Serial.print("   |  ");
    Serial.println(motorCal[i].weak);
  }
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readString();
    input.trim();
    
    if (input.length() == 1) {
      char dir = input.charAt(0);
      if (dir == 'u' || dir == 'd' || dir == 'l' || dir == 'r') {
        currentDirection = dir;
        Serial.print("Direction set to: ");
        Serial.println(dir);
      } else if (dir == 's') {
        currentDirection = 0;
        Serial.println("Stopped");
      } else if (dir == 'c') {
        showCalibration();
      }
    } else if (input.startsWith("t") && input.length() >= 4) {
      uint8_t channel = input.substring(1, 2).toInt();
      uint8_t overdrive = input.substring(2).toInt();
      if (channel >= 1 && channel <= 4 && overdrive >= 0 && overdrive <= 255) {
        testMotor(channel, overdrive);
      } else {
        Serial.println("Invalid test command. Format: t[channel][overdrive] (e.g., t1100)");
      }
    }
  }
  
  if (currentDirection) {
    switch (currentDirection) {
      case 'u': pullUp(); break;
      case 'd': pullDown(); break;
      case 'l': pullLeft(); break;
      case 'r': pullRight(); break;
    }
    delay(500); // Pause between cycles
  }
}