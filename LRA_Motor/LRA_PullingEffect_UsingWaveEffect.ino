const int motors[4] = {4, 12, 6, 2};  // M1-Up, M2-Right, M3-Down, M4-Left

String command = "";

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 4; i++) {
    pinMode(motors[i], OUTPUT);
    analogWrite(motors[i], 0); // Make sure all motors are off
  }
  Serial.println("Ready. Send: up, down, left, right, stop");
}

void loop() {
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    command.trim();

    Serial.println("Received: " + command);

    if (command == "right") pullRight();
    else if (command == "left") pullLeft();
    else if (command == "up") pullUp();
    else if (command == "down") pullDown();
    else stopAll();
  }
}

void pulse(int pin, int strength, int duration) {
  analogWrite(pin, strength);
  delay(duration);
  analogWrite(pin, 0);
}

void pullRight() {
  pulse(motors[0], 100, 30); delay(20);  // Up
  pulse(motors[1], 180, 30); delay(20);  // Right
  pulse(motors[2], 220, 30); delay(20);  // Down
  pulse(motors[3], 255, 30);             // Left
}

void pullLeft() {
  pulse(motors[3], 100, 30); delay(20);  // Left
  pulse(motors[2], 180, 30); delay(20);  // Down
  pulse(motors[1], 220, 30); delay(20);  // Right
  pulse(motors[0], 255, 30);             // Up
}

void pullUp() {
  pulse(motors[2], 100, 30); delay(20);  // Down
  pulse(motors[1], 180, 30); delay(20);  // Right
  pulse(motors[3], 220, 30); delay(20);  // Left
  pulse(motors[0], 255, 30);             // Up
}

void pullDown() {
  pulse(motors[0], 100, 30); delay(20);  // Up
  pulse(motors[1], 180, 30); delay(20);  // Right
  pulse(motors[3], 220, 30); delay(20);  // Left
  pulse(motors[2], 255, 30);             // Down
}

void stopAll() {
  for (int i = 0; i < 4; i++) {
    analogWrite(motors[i], 0);
  }
  Serial.println("All motors stopped.");
}
