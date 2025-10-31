#include <Wire.h>
#include <MPU6050.h>

// Pump control
const int pumpPin = 11;        // PWM output pin for pump control
const int pwmValue = 255;      // Fixed PWM value for pump control

// Solenoid pins (2 per actuator, 3 actuators total)
const int leg1PumpPin = 9;     // Leg 1 pump connection (affects only pitch)
const int leg1AtmPin = 10;     // Leg 1 vent to atmosphere
const int leg2PumpPin = 8;     // Leg 2 pump connection (affects pitch and roll)
const int leg2AtmPin = 7;      // Leg 2 vent to atmosphere
const int leg3PumpPin = 6;     // Leg 3 pump connection (affects pitch and roll)
const int leg3AtmPin = 5;      // Leg 3 vent to atmosphere

// MPU6050 setup
MPU6050 mpu;

// Variables to store raw data
int16_t ax, ay, az, gx, gy, gz;

// Control thresholds
const float targetPitch = 0.0;  // Desired pitch angle
const float targetRoll = 0.0;   // Desired roll angle
const float tolerance = 3;    // Allowable deviation from target angles

void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);

  // Initialize Pump and Solenoid Pins
  pinMode(pumpPin, OUTPUT);
  pinMode(leg1PumpPin, OUTPUT);
  pinMode(leg1AtmPin, OUTPUT);
  pinMode(leg2PumpPin, OUTPUT);
  pinMode(leg2AtmPin, OUTPUT);
  pinMode(leg3PumpPin, OUTPUT);
  pinMode(leg3AtmPin, OUTPUT);

  // Turn off all solenoids initially
  digitalWrite(leg1PumpPin, LOW);
  digitalWrite(leg1AtmPin, LOW);
  digitalWrite(leg2PumpPin, LOW);
  digitalWrite(leg2AtmPin, LOW);
  digitalWrite(leg3PumpPin, LOW);
  digitalWrite(leg3AtmPin, LOW);

  // Initialize MPU6050
  Wire.begin(); // A4 (SDA) and A5 (SCL) are default on Arduino Nano
  Serial.println("Initializing MPU6050...");
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
    while (1); // Stop execution if connection fails
  }
}

void controlActuator(int pumpPin, int atmPin, float adjustment) {
  static const float increment = 0.5;       // Minimum increment threshold
  static const float hysteresis = 1.0;      // Hysteresis value to avoid oscillations

  if (adjustment > (tolerance + hysteresis)) {
    // Small incremental inflation
    digitalWrite(pumpPin, LOW);             // Open pump solenoid
    digitalWrite(atmPin, HIGH);             // Close atmosphere solenoid
    delay(750);                              // Short inflation burst
    digitalWrite(pumpPin, HIGH);            // Close solenoids after burst
    digitalWrite(atmPin, HIGH);
    Serial.println("Extending actuator incrementally...");
  } else if (adjustment < -(tolerance + hysteresis)) {
    // Small incremental deflation
    digitalWrite(pumpPin, HIGH);            // Close pump solenoid
    digitalWrite(atmPin, LOW);              // Open atmosphere solenoid
    delay(100);                              // Short deflation burst
    digitalWrite(pumpPin, HIGH);            // Close solenoids after burst
    digitalWrite(atmPin, HIGH);
    Serial.println("Retracting actuator incrementally...");
  } else {
    // Maintain position within the hysteresis range
    digitalWrite(pumpPin, HIGH);            // Close pump solenoid
    digitalWrite(atmPin, HIGH);             // Close atmosphere solenoid
    Serial.println("Maintaining position...");
  }
}



void loop() {
  // Read raw accelerometer and gyroscope data
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Convert raw accelerometer values to "g"
  float AccX = ax / 16384.0; // Scale for ±2g
  float AccY = ay / 16384.0;
  float AccZ = az / 16384.0;

  // Calculate pitch and roll angles
  float pitch = atan2(-AccX, sqrt(AccY * AccY + AccZ * AccZ)) * 180 / PI; // Forward/backward tilt
  float roll = atan2(AccY, AccZ) * 180 / PI; // Side-to-side tilt

  // Calculate errors
  float pitchError = targetPitch - pitch;
  float rollError = targetRoll - roll;

  const float pitchGain = 1.5;  // Adjust this value to increase pitch correction

  
  // Map pitch and roll errors to actuator adjustments
  float leg1Adjustment = -rollError;
  float leg2Adjustment = -pitchGain * pitchError;                               // Amplify pitch correction
  float leg3Adjustment = rollError;

  // Debugging output
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" | Roll: ");
  Serial.print(roll);
  Serial.print(" | Leg 1 Adj: ");
  Serial.print(leg1Adjustment);
  Serial.print(" | Leg 2 Adj: ");
  Serial.print(leg2Adjustment);
  Serial.print(" | Leg 3 Adj: ");
  Serial.print(leg3Adjustment);

 
  // Check if the platform is balanced
  if (abs(leg1Adjustment) <= tolerance &&
      abs(leg2Adjustment) <= tolerance &&
      abs(leg3Adjustment) <= tolerance) {
    Serial.println("    The platform is balanced!");

    // Explicitly close all solenoids
    digitalWrite(leg1PumpPin, HIGH);
    digitalWrite(leg1AtmPin, HIGH);
    digitalWrite(leg2PumpPin, HIGH);
    digitalWrite(leg2AtmPin, HIGH);
    digitalWrite(leg3PumpPin, HIGH);
    digitalWrite(leg3AtmPin, HIGH);

    digitalWrite(pumpPin,LOW);
      
  } else {
    // Apply control logic for each actuator
    // Turn the pump on
    digitalWrite(pumpPin, HIGH);

    controlActuator(leg2PumpPin, leg2AtmPin, leg2Adjustment);
    controlActuator(leg1PumpPin, leg1AtmPin, leg1Adjustment);
    controlActuator(leg3PumpPin, leg3AtmPin, leg3Adjustment);
  }

}

Function controlActuator(pumpPin, atmPin, adjustment):
    If adjustment is significantly positive:
        Open pump to inflate actuator
        Close after a short burst

    Else if adjustment is significantly negative:
        Open atmosphere vent to deflate actuator
        Close after a short burst

    Else:
        Keep both pump and vent closed
