// Motor Pin (Change this to the correct pin connected to your motor)
const int motorPin = 2;  // Example: Motor connected to pin 3

void setup() {
  // Set the motor pin as output
  pinMode(motorPin, OUTPUT);
}

void loop() {
  // Turn the motor on at half speed
  analogWrite(motorPin, 128);  // Range: 0-255 (0 = off, 255 = full speed)

  delay(1000);  // Run the motor for 1 second

  // Turn the motor off
  analogWrite(motorPin, 0);  // Stop the motor

  delay(1000);  // Wait for 1 second before repeating
}
