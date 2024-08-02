#include <M5StickC.h>
#include <WiFi.h>
#include <HTTPClient.h>


float accX, accY, accZ;
bool potentialFallDetected = false;
unsigned long fallStartTime = 0;
const unsigned long minFallTime = 400;  // Adjusted minimum time in milliseconds to consider a fall
const unsigned long maxFallTime = 1500; // Adjusted maximum time in milliseconds to confirm the fall
const float impactThreshold = 2.0;      // Increased impact threshold for fall detection
const float fallThreshold = 1.5;        // More sensitive threshold to start fall detection
unsigned long nextPrintTime = 0;
const unsigned long printInterval = 200; // Interval for printing in milliseconds

unsigned int fallCount = 0; //for debugging purposes 

//wifi
const char* ssid       = "wifi-ssid";
const char* password   = "wifi-password";
const char* apiEndpoint = "https://server-endpoint/api/fall-detection";
void connectToWiFi(); // Function declaration
void callFallApiEndpoint(); // Function declaration


void setup() {
  M5.begin();
  M5.IMU.Init();
  M5.IMU.getAres();
  connectToWiFi();
}

void loop() {
  M5.IMU.getAccelData(&accX, &accY, &accZ);

  float accMagnitude = sqrt(accX * accX + accY * accY + accZ * accZ);

  M5.Lcd.setCursor(0, 30);
  M5.Lcd.printf("accMagnitude: %.2f", accMagnitude);

  unsigned long currentTime = millis();

 
  // if (currentTime >= nextPrintTime) {

  //   nextPrintTime = currentTime + printInterval; // Schedule the next print
  // }

  if (accMagnitude < fallThreshold && !potentialFallDetected) {
    potentialFallDetected = true;
    fallStartTime = currentTime;
  } else if (potentialFallDetected) {
    unsigned long elapsedFallTime = currentTime - fallStartTime;

    if (accMagnitude > impactThreshold && elapsedFallTime >= minFallTime && elapsedFallTime <= maxFallTime) {
      //debugging
      // Serial.print("Fall Detected! Number: ");
      // Serial.println(fallCount); // Use println to end the line, making it easier to read
      // fallCount++;

      M5.Lcd.setCursor(0, 0);
      M5.Lcd.printf("Fall detected!");
      M5.Lcd.fillScreen(RED);
      delay(3000);
      M5.Lcd.fillScreen(BLACK);
      callFallApiEndpoint();
      potentialFallDetected = false;
    } else if (elapsedFallTime > maxFallTime || accMagnitude > fallThreshold) {
      // Reset fall detection if conditions are not met within the time window
      potentialFallDetected = false;
    }
  }

  // testing
  // if (accMagnitude > impactThreshold || accMagnitude < fallThreshold) {
  //   Serial.print("Fall Detected! Number: ");
  //   Serial.println(fallCount); // Use println to end the line, making it easier to read
  //   fallCount++;
  // }

  delay(30);
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi..");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());
}

void callFallApiEndpoint() {
  if(WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(apiEndpoint);
      http.addHeader("Content-Type", "application/json");
      String jsonBodyData = "{\"deviceId\": \"9f13890e-d3a8-4f2c-8ca0-d320553b86e8\"}";
      int httpResponseCode = http.POST(jsonBodyData); // Send the POST request

      // If the POST request was successful, httpResponseCode will be positive
      if (httpResponseCode > 0) {
        String response = http.getString(); // Get the response to the request
        Serial.println(httpResponseCode); // Print the response code
        Serial.println(response); // Print the response
      } else {
        Serial.print("Error on sending POST: ");
        Serial.println(httpResponseCode);
      }
    
      http.end(); // Close connection
  } else {
    //here should be the logic for handling falls that happened on an unconnected device
    Serial.println("Error in WiFi connection");
  }
}
