// Pin assignments for vibration motors
const int motorUp1 = 3;  
const int motorUp2 = 5;  
const int motorDown1 = 12;  
const int motorDown2 = 11;  
const int motorLeft = 9;   
const int motorRight = 6;  

String inputDirection = ""; // Variable to store user input
const int pulseDuration = 200; // Motor ON time in milliseconds
const int pauseDuration = 300; // Pause time between pulses in milliseconds

// Function declarations
void pulseMotors(int motor1, int motor2 = -1); 
void stopAllMotors();

void setup() {
  // Initialize motor pins as outputs
  pinMode(motorUp1, OUTPUT);
  pinMode(motorUp2, OUTPUT);
  pinMode(motorDown1, OUTPUT);
  pinMode(motorDown2, OUTPUT);
  pinMode(motorLeft, OUTPUT);
  pinMode(motorRight, OUTPUT);

  // Initialize motors to OFF
  stopAllMotors();

  // Start Serial communication
  Serial.begin(9600);
  Serial.println("Enter direction: up, down, left, or right");
}

void loop() {
  // Check if data is available from Serial Monitor
  if (Serial.available() > 0) {
    inputDirection = Serial.readStringUntil('\n'); 
    inputDirection.trim(); 
    processInput(inputDirection);
  }
}

void processInput(String direction) {
  Serial.println("Processing input...");
  
  // Run the pulse pattern based on the direction
  if (direction == "up") {
    pulseMotors(motorUp1, motorUp2);
    Serial.println("Vibrating Up motors");
  } 
  else if (direction == "down") {
    pulseMotors(motorDown1, motorDown2);
    Serial.println("Vibrating Down motors");
  } 
  else if (direction == "left") {
    pulseMotors(motorLeft);
    Serial.println("Vibrating Left motor");
  } 
  else if (direction == "right") {
    pulseMotors(motorRight);
    Serial.println("Vibrating Right motor");
  } 
  else {
    Serial.println("Invalid direction! Use 'up', 'down', 'left', or 'right'.");
  }
}

void pulseMotors(int motor1, int motor2) {
  // Vibrate the motors in pulses
  for (int i = 0; i < 5; i++) { // Adjust the number of pulses as needed
    digitalWrite(motor1, HIGH);
    if (motor2 != -1) digitalWrite(motor2, HIGH);
    delay(pulseDuration);

    digitalWrite(motor1, LOW);
    if (motor2 != -1) digitalWrite(motor2, LOW);
    delay(pauseDuration);
  }
}

void stopAllMotors() {
  // Turn off all motors
  digitalWrite(motorUp1, LOW);
  digitalWrite(motorUp2, LOW);
  digitalWrite(motorDown1, LOW);
  digitalWrite(motorDown2, LOW);
  digitalWrite(motorLeft, LOW);
  digitalWrite(motorRight, LOW);
}