// 2D array for motor groups
const int vertical_motorPins[2][2] = {
  {3, 5},   // "up" group  
  //{6, 9},  // "middle" group
  {12, 12}   // "bottom" group
};

const int horizontal_motorPins[2][2] = {
  {9},   // "left" group
  {6}   // "right" group
};

const int vertical_groups = 2;               // Number of motor groups
const int vertical_motorsPerGroup = 2;
const int horizontal_groups = 2;             
const int horizontal_motorsPerGroup = 2;       // Motors in each group

const int pulseDuration = 300;      // Duration of vibration pulse (milliseconds)
const int delayBetweenGroups = 800; // Delay between motor groups (milliseconds)

int potPin = A0;   // Potentiometer pin
int potValue = 0;  // Potentiometer value

bool isUpDirection = false;
bool isDownDirection = false;
bool isRightDirection = false;
bool isLeftDirection = false;
 
  // Setting motor pins as outputs
void setup() {
  for (int group = 0; group < vertical_groups; group++) {
    for (int motor = 0; motor < vertical_motorsPerGroup; motor++) {
      pinMode(vertical_motorPins[group][motor], OUTPUT);
    }
  }

  pinMode(potPin, INPUT); // Setting potentiometer pin as input

  Serial.begin(9600); // Initializing serial communication
  Serial.println("Enter 'up' or 'down' to change the direction:");
}

// Checking for serial input
void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "up") {
      isUpDirection = true;
      isDownDirection = false;
      isRightDirection = false;
      isLeftDirection = false; 
      Serial.println("Direction set to: UP");
    } 
    else if (command == "down") {
      isDownDirection = true;
      isUpDirection = false;
      isRightDirection = false;
      isLeftDirection = false;
      Serial.println("Direction set to: DOWN");
    } 
    else if (command == "right") {
      isRightDirection = true;
      isUpDirection = false;
      isDownDirection = false;
      isLeftDirection = false;
      Serial.println("Direction set to: RIGHT");
    } 
    else if (command == "left") {
      isLeftDirection = true;
      isUpDirection = false;
      isDownDirection = false;
      isRightDirection = false;
      Serial.println("Direction set to: LEFT");
    }
    else {
      Serial.println("Invalid command. Enter 'up' or 'down' or 'right' or 'left'.");
    }
  }

  // Map potentiometer value to PWM range (0-255)
  potValue = analogRead(potPin);
  int motorSpeed = map(potValue, 0, 1023, 0, 255);

  // Control motors based on the current direction
  if(isUpDirection){
    controlMotors_up(vertical_motorPins, vertical_groups, vertical_motorsPerGroup, motorSpeed, pulseDuration);
  } 
  else if(isDownDirection){
    controlMotors_down(vertical_motorPins, vertical_groups, vertical_motorsPerGroup, motorSpeed, pulseDuration);
  }

  else if(isRightDirection){
    controlMotors_right(horizontal_motorPins, horizontal_groups, horizontal_motorsPerGroup, motorSpeed, pulseDuration);
  } 
  else if(isLeftDirection){
    controlMotors_left(horizontal_motorPins, horizontal_groups, horizontal_motorsPerGroup, motorSpeed, pulseDuration);
  }

  delay(delayBetweenGroups); // Delay before repeating the sequence
}

// Function to control motors in downward direction
void controlMotors_down(const int pins[][2], int numGroups, int motorsPerGroup, int speed, int pulseTime) {
  for (int group = 0; group < numGroups; group++) {
    for (int motor = 0; motor < motorsPerGroup; motor++) {
      analogWrite(pins[group][motor], speed); // Turn on the motor
      Serial.print("Down: Group ");
      Serial.print(group);
      Serial.print(", Motor ");
      Serial.println(pins[group][motor]);
    }

    delay(pulseTime);

    for (int motor = 0; motor < motorsPerGroup; motor++) {
      analogWrite(pins[group][motor], 0); // Turning off the motor
    }

    delay(2);
  }
}

// Function to control motors in upward direction
void controlMotors_up(const int pins[][2], int numGroups, int motorsPerGroup, int speed, int pulseTime) {
  for (int i = numGroups - 1; i >= 0; i--) {
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

    delay(2);
  }
}


void controlMotors_right(const int pins[][2], int numGroups, int motorsPerGroup, int speed, int pulseTime) {
  for (int group = 0; group < numGroups; group++) {
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

    delay(2);
  }
}


void controlMotors_left(const int pins[][2], int numGroups, int motorsPerGroup, int speed, int pulseTime) {
  for (int i = numGroups - 1; i >= 0; i--) {
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

    delay(2);
  }
}