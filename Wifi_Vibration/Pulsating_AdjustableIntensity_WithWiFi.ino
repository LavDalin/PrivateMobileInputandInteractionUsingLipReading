#include <WiFi.h>
#include <ArduinoOTA.h>
#include <WebSocketsServer.h>

const char* ssid = "HCI-Group";  // Replace with your Wi-Fi SSID
const char* password = "HCI95343";  // Replace with your Wi-Fi Password

const int all_motors[5] = {3, 5, 6, 9, 12};

const int up_motorPins[2] = {3, 5};
const int down_motorPins[2] = {12, 12};
const int right_motorPins[1] = {6};
const int left_motorPins[1] = {9};

const int potPin = A0;
const int numberOfMotors = 5;
const int motorsPerGroup_1 = 2;
const int motorsPerGroup_2 = 1;

String inputDirection = "";
String lastDirection = "";
const int maxIntensity = 255;

// Pulse timing variables
unsigned long lastPulseTime = 0;
const unsigned long pulseDuration = 200;
const unsigned long pulseInterval = 400;
bool motorState = false;

// Voltage display timing
unsigned long lastVoltageDisplayTime = 0;
const unsigned long voltageDisplayInterval = 1000;

WebSocketsServer webSocket(81);  // WebSocket server on port 81

void setup() {
    Serial.begin(115200);

    // Wi-Fi Setup
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // OTA Setup
    ArduinoOTA.setHostname("NanoESP32");
    ArduinoOTA.onStart([]() { Serial.println("OTA Update Started..."); });
    ArduinoOTA.onEnd([]() { Serial.println("\nOTA Update Completed!"); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]\n", error);
    });
    ArduinoOTA.begin();

    // WebSocket Setup
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    // Motor & Potentiometer Setup
    for (int motor = 0; motor < numberOfMotors; motor++) {
        pinMode(all_motors[motor], OUTPUT);
    }
    pinMode(potPin, INPUT);

    Serial.println("WebSocket Ready! Send: up, down, left, right");
}

void loop() {
    ArduinoOTA.handle();  // Handle OTA updates
    webSocket.loop();  // Handle WebSocket connections

    int potValue = analogRead(potPin);
    int intensity = map(potValue, 0, 1023, 0, maxIntensity);

    unsigned long currentTime = millis();
    if (currentTime - lastVoltageDisplayTime >= voltageDisplayInterval) {
        lastVoltageDisplayTime = currentTime;
        float voltage = (potValue / 1023.0) * 5.0;
        Serial.print("Potentiometer Voltage: ");
        Serial.print(voltage, 2);
        Serial.println("V");
    }

    if (currentTime - lastPulseTime >= (motorState ? pulseDuration : pulseInterval)) {
        motorState = !motorState;
        lastPulseTime = currentTime;

        if (motorState) {
            processInput(lastDirection, intensity);
        } else {
            stopAllMotors();
        }
    }
}

// Handle WebSocket messages
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
    if (type == WStype_TEXT) {
        String message = String((char*)payload);
        message.trim();
        Serial.println("Received: " + message);

        if (message == "up" || message == "down" || message == "left" || message == "right") {
            if (message != lastDirection) {
                lastDirection = message;
                stopAllMotors();
            }
        }
    }
}

void processInput(String direction, int intensity) {
    if (direction == "up") {
        pulseMotors(up_motorPins, intensity, motorsPerGroup_1);
    } else if (direction == "down") {
        pulseMotors(down_motorPins, intensity, motorsPerGroup_1);
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
        analogWrite(pins[motor], intensity);
        Serial.print("Pulsing motor: ");
        Serial.print(pins[motor]);
        Serial.print(" with intensity: ");
        Serial.println(intensity);
    }
}

void stopAllMotors() {
    for (int motor = 0; motor < numberOfMotors; motor++) {
        analogWrite(all_motors[motor], 0);
        digitalWrite(all_motors[motor], LOW);
    }
}
