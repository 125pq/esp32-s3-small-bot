#include <WiFi.h>
#include <esp_wifi.h>
#include "WiFi_Module.h"

// Define the campus network name and password 
// (leave password empty for web portal authentication)
const char* wifiName = "CUP-WLAN";
const char* wifiPassword = "";

// Predefined MAC address of your computer to bypass the campus network web authentication
uint8_t targetMacAddress[] = {0X6e, 0Xfa, 0X21, 0X0e, 0X5c, 0Xed};

void connectToWiFi() {
  // 1. Disconnect previous connections and clear configurations to prevent hanging
  WiFi.disconnect(true);
  delay(100);
  
  // 2. Set to Station mode to allow ESP32 to act as a client connecting to the router
  WiFi.mode(WIFI_STA);
  
  // 3. Forcibly change the MAC address of the ESP32 to the computer's MAC address
  esp_wifi_set_mac(WIFI_IF_STA, targetMacAddress);
  
  // 4. Initiate the network connection request
  WiFi.begin(wifiName, wifiPassword);
  
  // 5. Set a timeout mechanism, waiting up to 20 times (approx. 10 seconds)
  int retryCount = 0;
  while (WiFi.status() != WL_CONNECTED && retryCount < 20) {
    delay(500);
    Serial.print(".");
    retryCount++;
  }
  
  // 6. Verify the final connection result and output feedback to the serial monitor
  if(WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Connected Successfully!");
    Serial.print("Assigned IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi Connection Failed. Please check the signal or for a MAC address conflict.");
  }
}