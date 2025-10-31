// Pin definitions
const int pumpPin = 34;        // Pump control
const int leg1PumpPin = 4;     // Leg 1 pump connection
const int leg1AtmPin = 23;     // Leg 1 vent to atmosphere
const int leg2PumpPin = 19;    // Leg 2 pump connection
const int leg2AtmPin = 18;     // Leg 2 vent to atmosphere
const int leg3PumpPin = 17;    // Leg 3 pump connection
const int leg3AtmPin = 16;     // Leg 3 vent to atmosphere

// Variable to store the current state of each pin
bool pumpState = LOW;
bool leg1PumpState = LOW;
bool leg1AtmState = LOW;
bool leg2PumpState = LOW;
bool leg2AtmState = LOW;
bool leg3PumpState = LOW;
bool leg3AtmState = LOW;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set all pins as OUTPUT
  pinMode(pumpPin, OUTPUT);
  pinMode(leg1PumpPin, OUTPUT);
  pinMode(leg1AtmPin, OUTPUT);
  pinMode(leg2PumpPin, OUTPUT);
  pinMode(leg2AtmPin, OUTPUT);
  pinMode(leg3PumpPin, OUTPUT);
  pinMode(leg3AtmPin, OUTPUT);

  // Initialize all pins to LOW
  digitalWrite(pumpPin, LOW);
  digitalWrite(leg1PumpPin, LOW);
  digitalWrite(leg1AtmPin, LOW);
  digitalWrite(leg2PumpPin, LOW);
  digitalWrite(leg2AtmPin, LOW);
  digitalWrite(leg3PumpPin, LOW);
  digitalWrite(leg3AtmPin, LOW);

  Serial.println("Serial Monitor Pin Control Ready.");
  Serial.println("Type one of the following to toggle a pin:");
  Serial.println("pump, leg1Pump, leg1Atm, leg2Pump, leg2Atm, leg3Pump, leg3Atm");
}

void loop() {
  // Check if there is serial input
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // Read the input
    command.trim(); // Remove any extra spaces or newlines

    // Handle the command
    if (command == "pump") {
      pumpState = !pumpState; // Toggle state
      digitalWrite(pumpPin, pumpState);
      Serial.println("Toggled Pump Pin. Current state: " + String(pumpState ? "HIGH" : "LOW"));
    } else if (command == "leg1Pump") {
      leg1PumpState = !leg1PumpState;
      digitalWrite(leg1PumpPin, leg1PumpState);
      Serial.println("Toggled Leg 1 Pump Pin. Current state: " + String(leg1PumpState ? "HIGH" : "LOW"));
    } else if (command == "leg1Atm") {
      leg1AtmState = !leg1AtmState;
      digitalWrite(leg1AtmPin, leg1AtmState);
      Serial.println("Toggled Leg 1 Atmosphere Pin. Current state: " + String(leg1AtmState ? "HIGH" : "LOW"));
    } else if (command == "leg2Pump") {
      leg2PumpState = !leg2PumpState;
      digitalWrite(leg2PumpPin, leg2PumpState);
      Serial.println("Toggled Leg 2 Pump Pin. Current state: " + String(leg2PumpState ? "HIGH" : "LOW"));
    } else if (command == "leg2Atm") {
      leg2AtmState = !leg2AtmState;
      digitalWrite(leg2AtmPin, leg2AtmState);
      Serial.println("Toggled Leg 2 Atmosphere Pin. Current state: " + String(leg2AtmState ? "HIGH" : "LOW"));
    } else if (command == "leg3Pump") {
      leg3PumpState = !leg3PumpState;
      digitalWrite(leg3PumpPin, leg3PumpState);
      Serial.println("Toggled Leg 3 Pump Pin. Current state: " + String(leg3PumpState ? "HIGH" : "LOW"));
    } else if (command == "leg3Atm") {
      leg3AtmState = !leg3AtmState;
      digitalWrite(leg3AtmPin, leg3AtmState);
      Serial.println("Toggled Leg 3 Atmosphere Pin. Current state: " + String(leg3AtmState ? "HIGH" : "LOW"));
    } else {
      Serial.println("Invalid command. Try again.");
    }
  }
}
