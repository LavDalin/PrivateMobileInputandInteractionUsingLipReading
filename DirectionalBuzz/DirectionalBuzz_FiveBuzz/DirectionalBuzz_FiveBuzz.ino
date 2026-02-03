const int vertical_motorPins[3][2] = {
  {2, 3},    // "up" group  
  {5, 4},    // "middle" group
  {6, 12}    // "bottom" group
};

const int horizontal_motorPins[2][3] = {
  {3,5,12},       // "left" group
  {2,4,6}        // "right" group
};

const int vertical_groups = 3;
const int vertical_motorsPerGroup = 2;
const int horizontal_groups = 2;
const int horizontal_motorsPerGroup = 3;

const int delayBetweenGroups = 800;
const int pulseDuration = 50;

// Default intensities (can be changed via serial)
int upSpeed = 180;
int downSpeed = 180;
int leftSpeed = 180;
int rightSpeed = 180;

bool isUpDirection = false;
bool isDownDirection = false;
bool isRightDirection = false;
bool isLeftDirection = false;

void setup() {
  for (int group = 0; group < vertical_groups; group++) {
    for (int motor = 0; motor < vertical_motorsPerGroup; motor++) {
      pinMode(vertical_motorPins[group][motor], OUTPUT);
    }
  }

  Serial.begin(9600);
  Serial.println("Enter 'up', 'down', 'left', 'right' to change direction.");
  Serial.println("Use 'speed <direction> <0-255>' to set intensity.");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toLowerCase();

    if (input == "up" || input == "down" || input == "left" || input == "right") {
      isUpDirection = (input == "up");
      isDownDirection = (input == "down");
      isRightDirection = (input == "right");
      isLeftDirection = (input == "left");
      Serial.print("Direction set to: ");
      Serial.println(input);
    } else if (input.startsWith("speed")) {
      handleSpeedCommand(input);
    } else {
      Serial.println("Invalid command. Use 'up', 'down', 'left', 'right', or 'speed <direction> <0-255>'.");
    }
  }

  if (isUpDirection)
    controlMotors_up(vertical_motorPins, vertical_groups, vertical_motorsPerGroup, upSpeed, pulseDuration);
  else if (isDownDirection)
    controlMotors_down(vertical_motorPins, vertical_groups, vertical_motorsPerGroup, downSpeed, pulseDuration);
  else if (isRightDirection)
    controlMotors_right(horizontal_motorPins, horizontal_groups, horizontal_motorsPerGroup, rightSpeed, pulseDuration);
  else if (isLeftDirection)
    controlMotors_left(horizontal_motorPins, horizontal_groups, horizontal_motorsPerGroup, leftSpeed, pulseDuration);

  delay(delayBetweenGroups);
}

// -------------------- Speed Command Parser --------------------
void handleSpeedCommand(String input) {
  input.trim();
  input.replace("speed", "");
  input.trim();

  int spaceIndex = input.indexOf(' ');
  if (spaceIndex == -1) {
    Serial.println("Invalid format. Use: speed <direction> <0-255>");
    return;
  }

  String dir = input.substring(0, spaceIndex);
  String valStr = input.substring(spaceIndex + 1);
  int val = valStr.toInt();
  if (val < 0 || val > 255) {
    Serial.println("Speed value must be between 0 and 255.");
    return;
  }

  if (dir == "up") {
    upSpeed = val;
    Serial.print("Set UP speed to ");
  } else if (dir == "down") {
    downSpeed = val;
    Serial.print("Set DOWN speed to ");
  } else if (dir == "left") {
    leftSpeed = val;
    Serial.print("Set LEFT speed to ");
  } else if (dir == "right") {
    rightSpeed = val;
    Serial.print("Set RIGHT speed to ");
  } else {
    Serial.println("Invalid direction. Use: up, down, left, right.");
    return;
  }
  Serial.println(val);
}

void controlMotors_down(const int pins[][2], int numGroups, int motorsPerGroup, int speed, int pulseTime) {
  for (int group = 0; group < numGroups; group++) {
    for (int j = 0; j < 5; j++) {
      for (int motor = 0; motor < motorsPerGroup; motor++) {
        analogWrite(pins[group][motor], speed);
        Serial.print("Down: Group ");
        Serial.print(group);
        Serial.print(", Motor ");
        Serial.println(pins[group][motor]);
      }
      delay(pulseTime);
      for (int motor = 0; motor < motorsPerGroup; motor++) {
        analogWrite(pins[group][motor], 0);
      }
      delay(40);
    }
  }
}

void controlMotors_up(const int pins[][2], int numGroups, int motorsPerGroup, int speed, int pulseTime) {
  for (int i = numGroups - 1; i >= 0; i--) {
    for (int j = 0; j < 5; j++) {
      for (int motor = 0; motor < motorsPerGroup; motor++) {
        analogWrite(pins[i][motor], speed);
        Serial.print("Up: Group ");
        Serial.print(i);
        Serial.print(", Motor ");
        Serial.println(pins[i][motor]);
      }
      delay(pulseTime);
      for (int motor = 0; motor < motorsPerGroup; motor++) {
        analogWrite(pins[i][motor], 0);
      }
      delay(40);
    }
  }
}

void controlMotors_right(const int pins[][3], int numGroups, int motorsPerGroup, int speed, int pulseTime) {
  for (int group = 0; group < numGroups; group++) {
    for (int j = 0; j < 5; j++) {
      for (int motor = 0; motor < motorsPerGroup; motor++) {
        analogWrite(pins[group][motor], speed);
        Serial.print("Right: Group ");
        Serial.print(group);
        Serial.print(", Motor ");
        Serial.println(pins[group][motor]);
      }
      delay(pulseTime);
      for (int motor = 0; motor < motorsPerGroup; motor++) {
        analogWrite(pins[group][motor], 0);
      }
      delay(40);
    }
  }
}

void controlMotors_left(const int pins[][3], int numGroups, int motorsPerGroup, int speed, int pulseTime) {
  for (int i = numGroups - 1; i >= 0; i--) {
    for (int j = 0; j < 5; j++) {
      for (int motor = 0; motor < motorsPerGroup; motor++) {
        analogWrite(pins[i][motor], speed);
        Serial.print("Left: Group ");
        Serial.print(i);
        Serial.print(", Motor ");
        Serial.println(pins[i][motor]);
      }
      delay(pulseTime);
      for (int motor = 0; motor < motorsPerGroup; motor++) {
        analogWrite(pins[i][motor], 0);
      }
      delay(40);
    }
  }
}