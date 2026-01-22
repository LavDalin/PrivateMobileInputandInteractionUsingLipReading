#include <Wire.h>
#include <Adafruit_DRV2605.h>

#define TCA_I2C_ADDRESS 0x70
#define MOTOR_DRIVER_I2C_ADDRESS 0x5A

#define UP_MOTOR_CHANNEL    0
#define DOWN_MOTOR_CHANNEL  1
#define LEFT_MOTOR_CHANNEL  2
#define RIGHT_MOTOR_CHANNEL 3

const int motor_channels[] = {UP_MOTOR_CHANNEL, DOWN_MOTOR_CHANNEL, LEFT_MOTOR_CHANNEL, RIGHT_MOTOR_CHANNEL};

enum MotorPosition {
  MOTOR_UP = 0,
  MOTOR_DOWN = 1, 
  MOTOR_LEFT = 2,
  MOTOR_RIGHT = 3
};

enum Direction {
  DIR_FORWARD = 0,
  DIR_BACKWARD = 1,  
  DIR_LEFT = 2,
  DIR_RIGHT = 3,
  DIR_FORWARD_LEFT = 4,
  DIR_FORWARD_RIGHT = 5,
  DIR_BACKWARD_LEFT = 6,
  DIR_BACKWARD_RIGHT = 7,
  DIR_STOP = 8
};

const float BASE_FREQUENCY = 75.0;
const float ASYMMETRY_FACTOR = 0.3;
const int SAMPLE_RATE = 1000;
 #define TWO_PI 6.283185307179586476925286766559

unsigned long lastUpdate = 0;
float phases[4] = {0.0, 0.0, 0.0, 0.0};
int currentAmplitudes[4] = {0, 0, 0, 0};
Direction currentDirection = DIR_STOP;

Adafruit_DRV2605 drv;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  Serial.println("Phone Case Directional Haptics System");
  Serial.println("Using TCA9548A I2C Multiplexer with DRV2605 drivers");
  
  for (int i = 0; i < 4; i++) {
    selectTCAChannel(motor_channels[i]);
    
    if (!drv.begin()) {
      Serial.print("Failed to initialize motor on channel ");
      Serial.println(motor_channels[i]);
    } else {
      Serial.print("Motor initialized on channel ");
      Serial.println(motor_channels[i]);
      
      drv.selectLibrary(1);
      drv.setMode(DRV2605_MODE_REALTIME);
    }
    delay(10);
  }
  
  Serial.println("System ready!");
  Serial.println("Navigation Commands:");
  Serial.println("'f' - Forward, 'b' - Backward, 'l' - Left, 'r' - Right");
  Serial.println("'1' - Forward-Left, '2' - Forward-Right");
  Serial.println("'3' - Backward-Left, '4' - Backward-Right");
  Serial.println("'s' - Stop, 't' - Test all motors");
  Serial.println("'+' - Increase intensity, '-' - Decrease intensity");
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();
    handleNavigationCommand(cmd);
  }
  
  if (micros() - lastUpdate >= (1000000 / SAMPLE_RATE)) {
    lastUpdate = micros();
    updateHapticPattern();
  }
}

void selectTCAChannel(uint8_t channel) {
  if (channel > 7) return;
  
  Wire.beginTransmission(TCA_I2C_ADDRESS);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

void updateHapticPattern() {
  for (int motor = 0; motor < 4; motor++) {
    if (currentAmplitudes[motor] > 0) {
      
      int waveValue = generateAsymmetricWave(phases[motor]);
      int scaledValue = (waveValue * currentAmplitudes[motor]) / 127;
      
      selectTCAChannel(motor_channels[motor]);
      drv.setRealtimeValue(scaledValue);
      
      phases[motor] += TWO_PI * BASE_FREQUENCY / SAMPLE_RATE;
      if (phases[motor] >= TWO_PI) {
        phases[motor] -= TWO_PI;
      }
    } else {
    
      selectTCAChannel(motor_channels[motor]);
      drv.setRealtimeValue(0);
    }
  }
}

int generateAsymmetricWave(float phase) {
  float normalizedPhase = phase / TWO_PI;
  float waveValue;
  
  if (normalizedPhase < ASYMMETRY_FACTOR) {
  
    float localPhase = normalizedPhase / ASYMMETRY_FACTOR;
    waveValue = sin(localPhase * PI);
  } else {
  
    float localPhase = (normalizedPhase - ASYMMETRY_FACTOR) / (1.0 - ASYMMETRY_FACTOR);
    waveValue = sin(PI + localPhase * PI);
  }
  

  return (int)((waveValue + 1.0) * 63.5);
}

void setDirectionalPattern(Direction dir) {
  currentDirection = dir;
  

  for (int i = 0; i < 4; i++) {
    currentAmplitudes[i] = 0;
    phases[i] = 0.0; 
  }
  
  switch (dir) {
    case DIR_FORWARD:
      currentAmplitudes[MOTOR_UP] = 120;
      Serial.println("Direction: Forward");
      break;
      
    case DIR_BACKWARD:
      currentAmplitudes[MOTOR_DOWN] = 120;
      Serial.println("Direction: Backward");
      break;
      
    case DIR_LEFT:
      currentAmplitudes[MOTOR_LEFT] = 120;
      Serial.println("Direction: Left");
      break;
      
    case DIR_RIGHT:
      currentAmplitudes[MOTOR_RIGHT] = 120;
      Serial.println("Direction: Right");
      break;
      
    case DIR_FORWARD_LEFT:
      currentAmplitudes[MOTOR_UP] = 90;
      currentAmplitudes[MOTOR_LEFT] = 90;
      Serial.println("Direction: Forward-Left");
      break;
      
    case DIR_FORWARD_RIGHT:
      currentAmplitudes[MOTOR_UP] = 90;
      currentAmplitudes[MOTOR_RIGHT] = 90;
      Serial.println("Direction: Forward-Right");
      break;
      
    case DIR_BACKWARD_LEFT:
      currentAmplitudes[MOTOR_DOWN] = 90;
      currentAmplitudes[MOTOR_LEFT] = 90;
      Serial.println("Direction: Backward-Left");
      break;
      
    case DIR_BACKWARD_RIGHT:
      currentAmplitudes[MOTOR_DOWN] = 90;
      currentAmplitudes[MOTOR_RIGHT] = 90;
      Serial.println("Direction: Backward-Right");
      break;
      
    case DIR_STOP:
      Serial.println("Direction: Stop");
      break;
  }
}

void createSequentialPattern(Direction dir) {
  static int patternStep = 0;
  static unsigned long lastPatternUpdate = 0;
  
  if (millis() - lastPatternUpdate > 150) { 
    lastPatternUpdate = millis();
    
    for (int i = 0; i < 4; i++) {
      currentAmplitudes[i] = 0;
    }
    
    switch (dir) {
      case DIR_FORWARD:
        
        currentAmplitudes[MOTOR_UP] = 127;
        break;
        
      case DIR_LEFT:
        
        if (patternStep == 0) currentAmplitudes[MOTOR_UP] = 100;
        else if (patternStep == 1) currentAmplitudes[MOTOR_LEFT] = 127;
        else if (patternStep == 2) currentAmplitudes[MOTOR_DOWN] = 100;
        break;
        
      case DIR_RIGHT:
     
        if (patternStep == 0) currentAmplitudes[MOTOR_UP] = 100;
        else if (patternStep == 1) currentAmplitudes[MOTOR_RIGHT] = 127;
        else if (patternStep == 2) currentAmplitudes[MOTOR_DOWN] = 100;
        break;
        
      case DIR_BACKWARD:
        currentAmplitudes[MOTOR_DOWN] = 127;
        break;
    }
    
    patternStep = (patternStep + 1) % 3;
  }
}

void handleNavigationCommand(char cmd) {
  static int intensity = 100;
  
  switch (cmd) {
    case 'f': setDirectionalPattern(DIR_FORWARD); break;
    case 'b': setDirectionalPattern(DIR_BACKWARD); break;
    case 'l': setDirectionalPattern(DIR_LEFT); break;
    case 'r': setDirectionalPattern(DIR_RIGHT); break;
    case '1': setDirectionalPattern(DIR_FORWARD_LEFT); break;
    case '2': setDirectionalPattern(DIR_FORWARD_RIGHT); break;
    case '3': setDirectionalPattern(DIR_BACKWARD_LEFT); break;
    case '4': setDirectionalPattern(DIR_BACKWARD_RIGHT); break;
    case 's': setDirectionalPattern(DIR_STOP); break;
    
    case 't':
      testAllMotors();
      break;
      
    case '+':
      intensity = min(intensity + 15, 127);
      adjustIntensity(intensity);
      Serial.print("Intensity: "); Serial.println(intensity);
      break;
      
    case '-':
      intensity = max(intensity - 15, 10);
      adjustIntensity(intensity);
      Serial.print("Intensity: "); Serial.println(intensity);
      break;
      
    case 'p':
      printSystemStatus();
      break;
      
    case 'q':
      static bool useSequential = false;
      useSequential = !useSequential;
      Serial.print("Pattern mode: ");
      Serial.println(useSequential ? "Sequential" : "Simultaneous");
      break;
  }
}

void adjustIntensity(int newIntensity) {
  float scaleFactor = (float)newIntensity / 100.0;
  
  for (int i = 0; i < 4; i++) {
    if (currentAmplitudes[i] > 0) {
      currentAmplitudes[i] = (int)(newIntensity * scaleFactor);
      currentAmplitudes[i] = constrain(currentAmplitudes[i], 0, 127);
    }
  }
}

void testAllMotors() {
  Serial.println("Testing all motors sequentially...");
  
  const char* motorNames[] = {"UP", "DOWN", "LEFT", "RIGHT"};
  
  for (int motor = 0; motor < 4; motor++) {
    for (int i = 0; i < 4; i++) {
      currentAmplitudes[i] = 0;
    }

    currentAmplitudes[motor] = 100;
    
    Serial.print("Testing "); Serial.print(motorNames[motor]);
    Serial.print(" motor (Channel "); Serial.print(motor_channels[motor]); Serial.println(")");

    unsigned long testStart = millis();
    while (millis() - testStart < 1000) {
      updateHapticPattern();
      delayMicroseconds(1000);
    }
    
    delay(200);
  }
  
  setDirectionalPattern(DIR_STOP);
  Serial.println("Motor test complete");
}

void printSystemStatus() {
  Serial.println("=== Phone Case Navigation System Status ===");
  
  Serial.print("Current Direction: ");
  const char* dirNames[] = {"Forward", "Backward", "Left", "Right", 
                           "Forward-Left", "Forward-Right", 
                           "Backward-Left", "Backward-Right", "Stop"};
  Serial.println(dirNames[currentDirection]);
  
  Serial.println("Motor States:");
  const char* motorNames[] = {"UP", "DOWN", "LEFT", "RIGHT"};
  for (int i = 0; i < 4; i++) {
    Serial.print("  "); Serial.print(motorNames[i]);
    Serial.print(" (Ch"); Serial.print(motor_channels[i]); Serial.print("): ");
    Serial.print(currentAmplitudes[i]); Serial.println("/127");
  }
}


void processGPSDirection(String navCommand) {
 
  if (navCommand == "TURN_LEFT") {
    setDirectionalPattern(DIR_LEFT);
  } else if (navCommand == "TURN_RIGHT") {
    setDirectionalPattern(DIR_RIGHT);
  } else if (navCommand == "GO_STRAIGHT") {
    setDirectionalPattern(DIR_FORWARD);
  } else if (navCommand == "SLIGHT_LEFT") {
    setDirectionalPattern(DIR_FORWARD_LEFT);
  } else if (navCommand == "SLIGHT_RIGHT") {
    setDirectionalPattern(DIR_FORWARD_RIGHT);
  } else if (navCommand == "U_TURN") {
    setDirectionalPattern(DIR_BACKWARD);
  } else if (navCommand == "ARRIVED") {
    setDirectionalPattern(DIR_STOP);
  }
}

void parseBluetoothCommand(String btCommand) {
  btCommand.trim();
  btCommand.toUpperCase();
  
  if (btCommand.startsWith("NAV:")) {
    String direction = btCommand.substring(4);
    processGPSDirection(direction);
  } else if (btCommand.startsWith("INT:")) {
    int intensity = btCommand.substring(4).toInt();
    intensity = constrain(intensity, 10, 127);
    adjustIntensity(intensity);
  }
}