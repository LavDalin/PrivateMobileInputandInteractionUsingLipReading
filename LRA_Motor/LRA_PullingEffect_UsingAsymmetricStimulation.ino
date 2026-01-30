#include <Wire.h>
#include <Adafruit_DRV2605.h>

// PCA9548A I2C Multiplexer address
#define PCA9548A_ADDR 0x70

// Motor positions
enum MotorPosition {
  FRONT = 0,
  RIGHT = 3, 
  BACK = 1,
  LEFT = 2
};

enum Direction {
  DIR_FORWARD  = FRONT,
  DIR_RIGHT    = RIGHT,
  DIR_BACKWARD = BACK,
  DIR_LEFT     = LEFT,
  DIR_STOP     = 255
};

Adafruit_DRV2605 drv;

const unsigned long PULSE_ON_TIME = 7000;
const unsigned long PULSE_OFF_TIME = 18000;
const unsigned long CYCLE_TIME = PULSE_ON_TIME + PULSE_OFF_TIME;

unsigned long lastCycleTime = 0;
bool pullingActive = false;
Direction currentDirection = DIR_STOP;

bool initializedChannels[8] = { false };

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin();

  Serial.println("Initializing LRA Pulling Illusion System...");

  if (!initializePCA9548A()) {
    Serial.println("Failed to initialize PCA9548A!");
    while (1);
  }

  initializeAllDRV2605L();

  Serial.println("System initialized!");
  Serial.println("Send: f=forward, r=right, b=back, l=left, s=stop");
}

void loop() {
  handleSerialCommands();
  updatePullingForce();
}

bool initializePCA9548A() {
  Wire.beginTransmission(PCA9548A_ADDR);
  return (Wire.endTransmission() == 0);
}

void selectI2CChannel(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(PCA9548A_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

void initializeAllDRV2605L() {
  for (int channel = 0; channel < 8; channel++) {
    selectI2CChannel(channel);
    delay(10);

    if (drv.begin()) {
      drv.selectLibrary(1); 
      drv.setMode(DRV2605_MODE_INTTRIG);
      initializedChannels[channel] = true;

      Serial.print("DRV2605L initialized on channel ");
      Serial.println(channel);
    } else {
      initializedChannels[channel] = false;
      Serial.print("Failed to initialize DRV2605L on channel ");
      Serial.println(channel);
    }
  }
}

void handleSerialCommands() {
  while (Serial.available()) {
    char command = Serial.read();
    switch (command) {
      case 'f':
        Serial.println("Command: Pulling FORWARD");
        startPullingForce(DIR_FORWARD);
        break;
      case 'r':
        Serial.println("Command: Pulling RIGHT");
        startPullingForce(DIR_RIGHT);
        break;
      case 'b':
        Serial.println("Command: Pulling BACKWARD");
        startPullingForce(DIR_BACKWARD);
        break;
      case 'l':
        Serial.println("Command: Pulling LEFT");
        startPullingForce(DIR_LEFT);
        break;
      case 's':
        Serial.println("Command: STOP");
        startPullingForce(DIR_STOP);
        break;
      default:
        Serial.print("Unknown command: ");
        Serial.println(command);
        break;
    }
  }
}

void startPullingForce(Direction direction) {
  currentDirection = direction;
  pullingActive = (direction != DIR_STOP);
  lastCycleTime = micros();

  if (!pullingActive) {
    stopAllMotors();
  }
}

void updatePullingForce() {
  if (!pullingActive) return;

  unsigned long currentTime = micros();
  if (currentTime - lastCycleTime >= CYCLE_TIME) {
    lastCycleTime = currentTime;
    createAsymmetricPulse(currentDirection);
  }
}

void createAsymmetricPulse(Direction direction) {
  if (direction == DIR_STOP) return;

  Direction oppositeDir = (Direction)((direction + 2) % 4);

  activateMotor((MotorPosition)direction, 100, PULSE_ON_TIME / 1000);
  delay(2);
  activateMotor((MotorPosition)oppositeDir, 25, 3);
}

void activateMotor(MotorPosition motor, uint8_t intensity, uint8_t durationMs) {
  if (!initializedChannels[motor]) {
    Serial.print("Channel ");
    Serial.print(motor);
    Serial.println(" not initialized, skipping motor activation.");
    return;
  }

  selectI2CChannel(motor);
  delayMicroseconds(100);

  uint8_t waveform;
  if (intensity >= 100) waveform = 47;
  else if (intensity >= 75) waveform = 46;
  else if (intensity >= 50) waveform = 45;
  else waveform = 12;

  drv.setWaveform(0, waveform);
  drv.setWaveform(1, 0);
  drv.go();
  delay(durationMs);
}

void stopAllMotors() {
  for (int i = 0; i < 8; i++) {
    if (initializedChannels[i]) {
      selectI2CChannel(i);
      delayMicroseconds(100);
      drv.stop();
    }
  }
}
