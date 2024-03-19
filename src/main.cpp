#include <M5StickC.h>
#include <Arduino.h>

float gyroX, gyroY, gyroZ;
float accX, accY, accZ;
bool isFalling = false;
unsigned long fallStartTime = 0;

void setup() {
  // Initialize M5 and IMU
  M5.begin();
  M5.IMU.Init();
  // Get resolution for acc and gyro
  M5.IMU.getGres();
  M5.IMU.getAres();
}

void loop() {
  // Gyroscope data
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);

  // Accelerometer data
  M5.IMU.getAccelData(&accX, &accY, &accZ);

  // Calculate the magnitude of the acceleration vector
  float accMagnitude = sqrt(accX * accX + accY * accY + accZ * accZ); //1g≈9.81m/s(s al cuadrado). accMagnitude es en g 

  M5.Lcd.setCursor(0, 30);
  M5.Lcd.printf("accMagnitude: %.2f", accMagnitude);

  // Check for fall
  if (accMagnitude < 0.5) { // Threshold for detecting free fall (in g's, can be adjusted)
    if (!isFalling) {
      isFalling = true;
      fallStartTime = millis(); // Record the time when the fall starts
    }
  } else if (accMagnitude > 2 && isFalling) { // Threshold for detecting impact (in g's, can be adjusted)
    unsigned long fallTime = millis() - fallStartTime;
    if (fallTime < 2000) { // Check if the fall and impact are within a reasonable time window (here 2 seconds)
      // Fall detected
      M5.Lcd.setCursor(0, 0);
      M5.Lcd.printf("Fall detected!");
      digitalWrite(M5_LED, HIGH);
      delay(1000);
      digitalWrite(M5_LED,LOW);
      delay(4000);
      // Clear the screen
      M5.Lcd.fillScreen(BLACK);

    }
    isFalling = false;
  } else if (accMagnitude >= 0.5 && isFalling) {
    // If acceleration is back to normal range without an impact, reset fall detection
    isFalling = false;
  }

  // Display the acceleration data
  // M5.Lcd.setCursor(1, 60);
  // M5.Lcd.printf("X acc:%5.3f\nY acc:%5.3f\nZ acc:%5.3f g",
  //               accX, accY, accZ);

  delay(30);
}
