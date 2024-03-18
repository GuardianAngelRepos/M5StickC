#include <M5StickC.h>

float gyroX, gyroY, gyroZ;
float accX, accY, accZ;

void setup() {
  //initialize M5 and IMU
  M5.begin();
  M5.IMU.Init();
  //get resolution for acc and gyro
  M5.IMU.getGres();
  M5.IMU.getAres();
}

void loop() {
  // Gyroscope data 
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
  M5.Lcd.setCursor(1, 20);
  M5.Lcd.printf("X gyro:%5.2f\nY gyro:%5.2f\nZ gyro:%5.2f mg",
                gyroX * M5.IMU.gRes,
                gyroY * M5.IMU.gRes,
                gyroZ * M5.IMU.gRes);
  M5.Lcd.setCursor(1, 60);

  // Accelerometer data 
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  M5.Lcd.setCursor(0, 100);
  // M5.Lcd.printf("X acc:%5.10f",
  //               M5.IMU.aRes);
  M5.Lcd.printf("X acc:%5.3f\nY acc:%5.3f\nZ acc:%5.3f o/s",
                accX * 50,
                accY * 50,
                accZ * 50);
  // M5.Lcd.printf("X acc:%5.3f\nY acc:%5.3f\nZ acc:%5.3f o/s",
  //               accX * M5.IMU.aRes,
  //               accY * M5.IMU.aRes,
  //               accZ * M5.IMU.aRes);
  delay(30);
}