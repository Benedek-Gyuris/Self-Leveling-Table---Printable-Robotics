#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);  // SDA = GPIO 21, SCL = GPIO 22

  Serial.println("Initializing MPU6050...");
  mpu.initialize();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful!");
  } else {
    Serial.println("MPU6050 connection failed!");
    while (1); // Halt if connection fails
  }
}

void loop() {
  int16_t ax, ay, az;

  // Read raw accelerometer data
  mpu.getAcceleration(&ax, &ay, &az);

  // Print raw data to serial
  Serial.print("Accel X: ");
  Serial.print(ax);
  Serial.print(" | Y: ");
  Serial.print(ay);
  Serial.print(" | Z: ");
  Serial.println(az);

  delay(500); // Delay for stability
}
