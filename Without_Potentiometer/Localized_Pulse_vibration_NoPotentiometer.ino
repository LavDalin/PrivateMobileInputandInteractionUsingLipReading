const int all_motors[6] = {2,3,4,5,6,12}; 
const int up_motorPins[2] = {2, 3};   
const int down_motorPins[2] = {6, 12};  
const int right_motorPins[1] = {4};   
const int left_motorPins[1] = {5};   

const int up_right_motorPins[2] = {6, 5};  
const int up_left_motorPins[2] = {12, 3};   
const int down_right_motorPins[2] = {4, 5}; 
const int down_left_motorPins[2] = {2, 3};  

const int numberOfMotors = 6;
const int pwmFreq = 5000;
const int pwmResolution = 8;  

String inputDirection = "";
String lastDirection = "";  

int intensityLevel = 150;
const int maxIntensity = 255;  
const int minIntensity = 10;   

unsigned long lastPulseTime = 0;
unsigned long pulseDuration = 200;  
unsigned long pulseInterval = 400;  
bool motorState = false; 

void setup() {
    Serial.begin(115200);
    Serial.println("Enter direction (up, down, left, right, up_right, up_left, down_right, down_left). Use 'intensity [value]' to control intensity.");

    for (int i = 0; i < numberOfMotors; i++) {
        ledcSetup(i, pwmFreq, pwmResolution);
        ledcAttachPin(all_motors[i], i);
    }
}

void loop() {
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();

        if (command.startsWith("intensity ")) {
            intensityLevel = command.substring(10).toInt();
            intensityLevel = constrain(intensityLevel, minIntensity, maxIntensity);
            Serial.print("Intensity set to: ");
            Serial.println(intensityLevel);
        } else {
            inputDirection = command;
        }

        if (inputDirection != lastDirection) {  
            lastDirection = inputDirection;  
            stopAllMotors();
        }
    }

    unsigned long currentTime = millis();
    if (currentTime - lastPulseTime >= (motorState ? pulseDuration : pulseInterval)) {
        motorState = !motorState;  
        lastPulseTime = currentTime;
        
        if (motorState) {
            processInput(lastDirection, intensityLevel);
        } else {
            stopAllMotors();
        }
    }
}

void processInput(String direction, int intensity) {
    if (direction == "up") {
        pulseMotors(up_motorPins, intensity, 2);
    } else if (direction == "down") {
        pulseMotors(down_motorPins, intensity, 2);
    } else if (direction == "left") {
        pulseMotors(left_motorPins, intensity, 1);
    } else if (direction == "right") {
        pulseMotors(right_motorPins, intensity, 1);
    } else if (direction == "up_right") {
        pulseMotors(up_right_motorPins, intensity, 2);
    } else if (direction == "up_left") {
        pulseMotors(up_left_motorPins, intensity, 2);
    } else if (direction == "down_right") {
        pulseMotors(down_right_motorPins, intensity, 2);
    } else if (direction == "down_left") {
        pulseMotors(down_left_motorPins, intensity, 2);
    } else {
        stopAllMotors();
    }
}

void pulseMotors(const int pins[], int intensity, int numberOfMotors) {
    for (int i = 0; i < numberOfMotors; i++) {
        int pwmChannel = getPWMChannel(pins[i]);
        ledcWrite(pwmChannel, intensity);
        Serial.print("Pulsing motor: ");
        Serial.print(pins[i]);
        Serial.print(" with intensity: ");
        Serial.println(intensity);
    }
}

void stopAllMotors() {
    for (int i = 0; i < numberOfMotors; i++) {
        ledcWrite(i, 0);
    }
}

int getPWMChannel(int pin) {
    for (int i = 0; i < numberOfMotors; i++) {
        if (all_motors[i] == pin) {
            return i;
        }
    }
    return -1;  
}
