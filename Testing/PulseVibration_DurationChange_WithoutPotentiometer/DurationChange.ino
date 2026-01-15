
const int motorPin_1 = 3;
const int motorPin_2 = 2;

int vibrationDuration = 150; 
int pauseDuration = 1000;

void setup() {
  pinMode(motorPin_1, OUTPUT);
  pinMode(motorPin_2, OUTPUT);
  
  Serial.begin(9600);
  
  Serial.println("Continuous Vibration Pulser");
  Serial.println("Enter a number (ms) to change the pulse duration.");
  Serial.println("---------------------------------------------------");
}

void loop() {
  if (Serial.available() > 0) {
    int newDuration = Serial.parseInt();

    while(Serial.available() > 0) {
      Serial.read();
    }

    if (newDuration > 0) {
      vibrationDuration = newDuration;
      Serial.print("New pulse duration set to: ");
      Serial.print(vibrationDuration);
      Serial.println(" ms");
    }
  }
  
  
  digitalWrite(motorPin_1, HIGH);
  digitalWrite(motorPin_2, HIGH);
  delay(vibrationDuration);
  
  digitalWrite(motorPin_1, LOW);
  digitalWrite(motorPin_2, LOW);
  delay(pauseDuration);
}