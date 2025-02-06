const int all_motors[6] = {3, 5, 6, 9, 12};  // All motor pins

const int up_motorPins[2] = {3, 5};   // Motors for up direction
const int down_motorPins[1] = {12};   // Motors for down direction
const int right_motorPins[1] = {6};   // Motors for right direction
const int left_motorPins[1] = {9};    // Motors for left direction

const int potPin = A0;    // Potentiometer pin to control intensity

const int numberOfMotors = 6;  
const int motorsPerGroup_1 = 2;           
const int motorsPerGroup_2 = 1;       

String inputDirection = "";
String lastDirection = "";  // Store last active direction
const int maxIntensity = 255;  // Max intensity value for vibration

// Pulse timing variables
unsigned long lastPulseTime = 0;
unsigned long pulseDuration = 200;  // Duration for each pulse (milliseconds)
unsigned long pulseInterval = 400;  // Time between pulses
bool motorState = false; // Tracks ON/OFF state of pulses

// Voltage display timing
unsigned long lastVoltageDisplayTime = 0;
const unsigned long voltageDisplayInterval = 1000;  // Print voltage every second

// Fade variables
int targetIntensity = 0;
int currentIntensity = 0;
unsigned long lastFadeTime = 0;
const unsigned long fadeSpeed = 20; // Speed of intensity fade (higher = slower)

void setup() {
    for (int motor = 0; motor < numberOfMotors; motor++) {
        pinMode(all_motors[motor], OUTPUT);
    }
    pinMode(potPin, INPUT);
    Serial.begin(9600);
    Serial.println("Enter direction (up, down, left, right). Potentiometer controls intensity.");
}

void loop() {
    int potValue = analogRead(potPin);
    targetIntensity = map(potValue, 0, 1023, 0, maxIntensity);  // Map potentiometer value to intensity

    // Smooth fade effect for intensity change
    if (millis() - lastFadeTime >= fadeSpeed) {
        lastFadeTime = millis();
        // Gradually increase or decrease intensity
        if (currentIntensity < targetIntensity) {
            currentIntensity++;
        } else if (currentIntensity > targetIntensity) {
            currentIntensity--;
        }
    }

    // Adjust pulse duration and interval based on intensity
    pulseDuration = map(currentIntensity, 0, maxIntensity, 50, 200);  // Shorter pulses for lower intensity
    pulseInterval = map(currentIntensity, 0, maxIntensity, 200, 400);  // Adjust interval too

    // Print voltage at regular intervals
    unsigned long currentTime = millis();
    if (currentTime - lastVoltageDisplayTime >= voltageDisplayInterval) {
        lastVoltageDisplayTime = currentTime;
        float voltage = (potValue / 1023.0) * 5.0;
        Serial.print("Potentiometer Voltage: ");
        Serial.print(voltage, 2);
        Serial.println("V");
    }

    // Read new direction command
    if (Serial.available() > 0) {
        inputDirection = Serial.readStringUntil('\n');
        inputDirection.trim();

        if (inputDirection != lastDirection) {  
            lastDirection = inputDirection;  // Update direction
            stopAllMotors();  // Stop current motors before switching
        }
    }

    // Handle pulsed vibration
    if (currentTime - lastPulseTime >= (motorState ? pulseDuration : pulseInterval)) {
        motorState = !motorState;  // Toggle motor state
        lastPulseTime = currentTime;
        
        if (motorState) {
            processInput(lastDirection, currentIntensity); // Activate motors
        } else {
            stopAllMotors();  // Pause motors
        }
    }
}

void processInput(String direction, int intensity) {
    if (direction == "up") {
        pulseMotors(up_motorPins, intensity, motorsPerGroup_1);
    } else if (direction == "down") {
        pulseMotors(down_motorPins, intensity, motorsPerGroup_2);
    } else if (direction == "left") {
        pulseMotors(left_motorPins, intensity, motorsPerGroup_2);
    } else if (direction == "right") {
        pulseMotors(right_motorPins, intensity, motorsPerGroup_2);
    } else {
        stopAllMotors();
    }
}

void pulseMotors(const int pins[], int intensity, int numberOfMotor) {
    for (int motor = 0; motor < numberOfMotor; motor++) {
        analogWrite(pins[motor], intensity);  // Set motor intensity
        Serial.print("Pulsing motor: ");
        Serial.print(pins[motor]);
        Serial.print(" with intensity: ");
        Serial.println(intensity);
    }
}

void stopAllMotors() {
    for (int motor = 0; motor < numberOfMotors; motor++) {
        analogWrite(all_motors[motor], 0);  // Turn off all motors
        digitalWrite(all_motors[motor], LOW);
    }
}
