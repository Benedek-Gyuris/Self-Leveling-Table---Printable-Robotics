// Define pins
const int pwmPin = 5;        // PWM output pin for motor speed control
const int dirPin = 6;        // Direction control pin
const int pwmValue = 255;    // PWM value for full speed (adjust if needed)

// Timing variables
const int runTime = 2000;    // Time (ms) to run the motor in one direction

void setup() {
  // Set the pins as outputs
  pinMode(pwmPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  // Initialize Serial for debugging
  Serial.begin(9600);
  Serial.println("Motor control with MOSFET driver initialized.");
}

void loop() {
  // Move the motor in one direction
  digitalWrite(dirPin, HIGH); // Set direction
  analogWrite(pwmPin, pwmValue); // Run motor at full speed
  Serial.println("Motor running forward...");
  delay(runTime); // Run for specified time

  // Stop the motor briefly
  analogWrite(pwmPin, 0); // Stop motor
  Serial.println("Motor stopped.");
  delay(1000); // Pause for 1 second

  // Move the motor in the opposite direction
  digitalWrite(dirPin, LOW); // Change direction
  analogWrite(pwmPin, pwmValue); // Run motor at full speed
  Serial.println("Motor running backward...");
  delay(runTime); // Run for specified time

  // Stop the motor briefly
  analogWrite(pwmPin, 0); // Stop motor
  Serial.println("Motor stopped.");
  delay(1000); // Pause for 1 second
}
