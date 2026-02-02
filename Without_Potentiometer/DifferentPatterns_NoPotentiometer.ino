// Pin configurations for vertical and horizontal motor groups
const int vertical_motorPins[3][2] = {
  {3, 5},  // "up" group  
  {6, 9},  // "middle" group
  {12, 12} // "bottom" group
};

const int horizontal_motorPins[2][1] = {
  {9},  // "left" group
  {6}   // "right" group
};

// Number of motor groups
const int vertical_groups = 3;  
const int vertical_motorsPerGroup = 2;  
const int horizontal_groups = 2;  
const int horizontal_motorsPerGroup = 1;  

// Common settings
const int maxSpeed = 255;  // Maximum motor speed
const int pulseTime = 100; // Default pulse duration

// User selection flags
bool isWave = false;
bool isHeartbeat = false;
bool isBuzz = false;
bool isRandom = false;
bool isPulse = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Enter a pattern: 'wave', 'heartbeat', 'buzz', 'random', 'pulse'");

  for (int group = 0; group < vertical_groups; group++) {
    for (int motor = 0; motor < vertical_motorsPerGroup; motor++) {
      pinMode(vertical_motorPins[group][motor], OUTPUT);
    }
  }
}

void loop() {
  // Read Serial input
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    // Reset all flags
    isWave = isHeartbeat = isBuzz = isRandom = isPulse = false;

    if (command == "wave") {
      isWave = true;
      Serial.println("Wave pattern activated!");
    } else if (command == "heartbeat") {
      isHeartbeat = true;
      Serial.println("Heartbeat pattern activated!");
    } else if (command == "buzz") {
      isBuzz = true;
      Serial.println("Buzz pattern activated!");
    } else if (command == "random") {
      isRandom = true;
      Serial.println("Random pattern activated!");
    } else if (command == "pulse") {
      isPulse = true;
      Serial.println("Pulse pattern activated!");
    } else {
      Serial.println("Invalid command! Choose from: 'wave', 'heartbeat', 'buzz', 'random', 'pulse'");
    }
  }

  // Run the selected vibration pattern
  if (isWave) {
    rumble_wave(vertical_motorPins, vertical_groups, vertical_motorsPerGroup, maxSpeed, pulseTime);
  } else if (isHeartbeat) {
    rumble_heartbeat(vertical_motorPins, vertical_groups, vertical_motorsPerGroup, maxSpeed);
  } else if (isBuzz) {
    rumble_buzz(vertical_motorPins, vertical_groups, vertical_motorsPerGroup, maxSpeed);
  } else if (isRandom) {
    rumble_random(vertical_motorPins, vertical_groups, vertical_motorsPerGroup, maxSpeed);
  } else if (isPulse) {
    rumble_pulse(vertical_motorPins, vertical_groups, vertical_motorsPerGroup, maxSpeed, pulseTime);
  }
}

// ---------------------------------------
// Patterns
// ---------------------------------------

// Wave Effect
void rumble_wave(const int pins[][2], int numGroups, int motorsPerGroup, int maxSpeed, int pulseTime) {
  for (int i = 0; i < numGroups; i++) {
    for (int speed = 50; speed <= maxSpeed; speed += 25) {
      for (int motor = 0; motor < motorsPerGroup; motor++) {
        analogWrite(pins[i][motor], speed);
      }
      delay(50);
    }
    for (int speed = maxSpeed; speed >= 50; speed -= 25) {
      for (int motor = 0; motor < motorsPerGroup; motor++) {
        analogWrite(pins[i][motor], speed);
      }
      delay(50);
    }
    for (int motor = 0; motor < motorsPerGroup; motor++) {
      analogWrite(pins[i][motor], 0);
    }
    delay(100);
  }
}

// Heartbeat Effect
void rumble_heartbeat(const int pins[][2], int numGroups, int motorsPerGroup, int speed) {
  for (int i = 0; i < numGroups; i++) {
    for (int motor = 0; motor < motorsPerGroup; motor++) {
      analogWrite(pins[i][motor], speed);
    }
    delay(100);
    for (int motor = 0; motor < motorsPerGroup; motor++) {
      analogWrite(pins[i][motor], 0);
    }
    delay(100);
    
    for (int motor = 0; motor < motorsPerGroup; motor++) {
      analogWrite(pins[i][motor], speed);
    }
    delay(300);
    for (int motor = 0; motor < motorsPerGroup; motor++) {
      analogWrite(pins[i][motor], 0);
    }
    //delay(300);
  }
}

// Buzz Effect
void rumble_buzz(const int pins[][2], int numGroups, int motorsPerGroup, int speed) {
  for (int i = 0; i < numGroups; i++) {
    for (int j = 0; j < 5; j++) {
      for (int motor = 0; motor < motorsPerGroup; motor++) {
        analogWrite(pins[i][motor], speed);
      }
      delay(50);
      for (int motor = 0; motor < motorsPerGroup; motor++) {
        analogWrite(pins[i][motor], 0);
      }
      delay(50);
    }
    delay(200);
  }
}

// Random Chaos Rumble
void rumble_random(const int pins[][2], int numGroups, int motorsPerGroup, int maxSpeed) {
  for (int i = 0; i < numGroups; i++) {
    int randomSpeed = random(50, maxSpeed);
    int randomTime = random(100, 500);

    for (int motor = 0; motor < motorsPerGroup; motor++) {
      analogWrite(pins[i][motor], randomSpeed);
    }
    delay(randomTime);
    for (int motor = 0; motor < motorsPerGroup; motor++) {
      analogWrite(pins[i][motor], 0);
    }
    delay(random(100, 500));
  }
}

// Pulse Effect
void rumble_pulse(const int pins[][2], int numGroups, int motorsPerGroup, int speed, int pulseTime) {
  for (int i = 0; i < numGroups; i++) {
    for (int motor = 0; motor < motorsPerGroup; motor++) {
      analogWrite(pins[i][motor], speed);
    }
    delay(pulseTime);
    for (int motor = 0; motor < motorsPerGroup; motor++) {
      analogWrite(pins[i][motor], 0);
    }
    delay(pulseTime);
  }
}
