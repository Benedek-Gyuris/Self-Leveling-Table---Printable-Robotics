// Pump control
const int pumpPin = 11;        // PWM output pin for pump control
const int pwmValue = 255;      // Fixed PWM value for pump control

// Solenoid pins (specific actuator for testing)
const int testPumpPin = 6;    // Solenoid for pump connection to actuator
const int testAtmPin = 5;     // Solenoid to vent actuator to atmosphere

// Other solenoid pins (to be explicitly closed during the test)
const int leg1PumpPin = 8;    // Leg 1 pump connection
const int leg1AtmPin = 7;    // Leg 1 vent to atmosphere
const int leg2PumpPin = 9;    // Leg 2 pump connection
const int leg2AtmPin = 10;     // Leg 2 vent to atmosphere


void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);

 // Initialize Pump and Solenoid Pins
  pinMode(pumpPin, OUTPUT);
  pinMode(testPumpPin, OUTPUT);
  pinMode(testAtmPin, OUTPUT);
  pinMode(leg1PumpPin, OUTPUT);
  pinMode(leg1AtmPin, OUTPUT);
  pinMode(leg2PumpPin, OUTPUT);
  pinMode(leg2AtmPin, OUTPUT);

   // Set all solenoids to their default state (open)
  digitalWrite(pumpPin, LOW);      // Pump off initially
  digitalWrite(testPumpPin, LOW);  // Default open for pump solenoid
  digitalWrite(testAtmPin, LOW);   // Default open for atmosphere solenoid
  digitalWrite(leg1PumpPin, HIGH); // Close unused pump solenoid
  digitalWrite(leg1AtmPin, HIGH);  // Close unused atmosphere solenoid
  digitalWrite(leg2PumpPin, HIGH); // Close unused pump solenoid
  digitalWrite(leg2AtmPin, HIGH);  // Close unused atmosphere solenoid

  Serial.println("Actuator inflation test ready.");
  Serial.println("Type 'start' in the Serial Monitor to begin the test.");
}

void inflateActuator() {
  // Turn on the pump
  digitalWrite(pumpPin, HIGH);
  Serial.println("Pump is ON...");

  // Open the solenoid to inflate the actuator
  digitalWrite(testPumpPin, LOW);
  digitalWrite(testAtmPin, HIGH); // Close atmosphere solenoid
  Serial.println("Inflating actuator...");
  delay(5000); // Inflate for 5 seconds

  // Close the solenoid to maintain pressure
  digitalWrite(testPumpPin, HIGH);
  Serial.println("Maintaining actuator pressure...");
  delay(5000); // Hold pressure for 5 seconds

  // Vent the actuator to atmosphere
  digitalWrite(testPumpPin, HIGH);
  digitalWrite(testAtmPin, LOW);
  Serial.println("Venting actuator...");
  delay(5000); // Vent for 5 seconds

  // Stop the pump and reset
  digitalWrite(pumpPin, LOW);
  digitalWrite(testAtmPin, HIGH);
  Serial.println("Pump is OFF. Test complete.");
}

void loop() {
  // Wait for user input from the Serial Monitor
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove extra spaces or newline characters

    if (command.equalsIgnoreCase("start")) {
      Serial.println("Starting actuator inflation test...");
      inflateActuator();
      Serial.println("Test complete. Type 'start' to repeat.");
    } else {
      Serial.println("Invalid command. Type 'start' to begin the test.");
    }
  }
}
