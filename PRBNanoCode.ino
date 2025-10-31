#include <Wire.h>
#include <MPU6050.h>


// Actuator Solenoid Pins
const int S1PathToAtmPin = 11;  // Solenoid 1 - Path to atmosphere
const int S1SupplyPin = 10;     // Solenoid 1 - Supply pressure
const int S2PathToAtmPin = 9;   // Solenoid 2 - Path to atmosphere
const int S2SupplyPin = 6;      // Solenoid 2 - Supply pressure
const int S3PathToAtmPin = 5;   // Solenoid 3 - Path to atmosphere
const int S3SupplyPin = 3;      // Solenoid 3 - Supply pressure

// IMU setup
MPU6050 mpu;

// Variables to store raw data
int16_t ax, ay, az, gx, gy, gz;

// Control parameters
float targetPitch = 0.0; // Desired pitch angle
float targetRoll = 0.0;  // Desired roll angle
float threshold = 5.0;   // Allowable deviation from target angles

void setup() {
 
  // Solenoid setup
  pinMode(S1PathToAtmPin, OUTPUT);
  pinMode(S1SupplyPin, OUTPUT);
  pinMode(S2PathToAtmPin, OUTPUT);
  pinMode(S2SupplyPin, OUTPUT);
  pinMode(S3PathToAtmPin, OUTPUT);
  pinMode(S3SupplyPin, OUTPUT);

  // Initialize solenoids (all closed initially)
  digitalWrite(S1PathToAtmPin, LOW);
  digitalWrite(S1SupplyPin, LOW);
  digitalWrite(S2PathToAtmPin, LOW);
  digitalWrite(S2SupplyPin, LOW);
  digitalWrite(S3PathToAtmPin, LOW);
  digitalWrite(S3SupplyPin, LOW);

  // IMU setup
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Initializing MPU6050...");
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  Serial.println("System ready. Adjusting actuators based on IMU feedback.");
}

void controlActuator(int pathToAtmPin, int supplyPin, float error) {
  if (error > threshold) {
    // Extend actuator (open supply, close atmosphere)
    digitalWrite(pathToAtmPin, LOW);
    digitalWrite(supplyPin, HIGH);
  } else if (error < -threshold) {
    // Retract actuator (close supply, open atmosphere)
    digitalWrite(pathToAtmPin, HIGH);
    digitalWrite(supplyPin, LOW);
  } else {
    // Maintain position (close both solenoids)
    digitalWrite(pathToAtmPin, LOW);
    digitalWrite(supplyPin, LOW);
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

  // Output IMU data to Serial Monitor
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" | Roll: ");
  Serial.print(roll);
  Serial.print(" | Pitch Error: ");
  Serial.print(pitchError);
  Serial.print(" | Roll Error: ");
  Serial.println(rollError);

  // Control actuators based on pitch and roll errors
  controlActuator(S1PathToAtmPin, S1SupplyPin, pitchError); // Actuator 1
  controlActuator(S2PathToAtmPin, S2SupplyPin, rollError);  // Actuator 2
  controlActuator(S3PathToAtmPin, S2SupplyPin, rollError)

  // Add control for additional actuators as needed
  delay(100); // Small delay for stability
}
