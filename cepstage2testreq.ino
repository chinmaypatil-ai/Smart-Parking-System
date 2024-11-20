#include <WiFi.h>
#include <FirebaseESP32.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Wi-Fi credentials
#define WIFI_SSID "project"
#define WIFI_PASSWORD "testingon"

// Firebase credentials
#define FIREBASE_HOST "ledcontrol-1897f-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "EpO6vIdBv5IATc1uJkGuWJAqMZpp1aH9xslZsoRU"

// Firebase objects
FirebaseData fbData;
FirebaseAuth auth;
FirebaseConfig config;

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns, 2 rows

// Servo setup
Servo servo1;

// LED pin
const int ledPin = 4;

// Define the pins for the 8 IR sensors
const int slotPins[8] = {12, 14, 27, 26, 25, 33, 18, 19};
String slotStatus[6]; // To store the status of the first 6 slots

// Variables to store sensor readings
int sensorValues[8];

// Function to initialize Firebase
void setupFirebase() {
  // Firebase configuration
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  // Enable auto-reconnect Wi-Fi
  Firebase.reconnectWiFi(true);

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  if (Firebase.ready()) {
    Serial.println("Firebase connected");
  } else {
    Serial.println("Failed to connect to Firebase");
  }
}

void setup() {
  Serial.begin(115200);

  // Setup Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to Wi-Fi!");

  // Initialize Firebase
  setupFirebase();

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Parking Status");
  delay(3000);
  lcd.clear();

  // Initialize pins and components
  pinMode(ledPin, OUTPUT);
  for (int i = 0; i < 8; i++) {
    pinMode(slotPins[i], INPUT);
  }

  // Initialize the servo motor
  servo1.attach(23);
  servo1.write(0); // Start with servo closed
}

void updateFirebase(int slotIndex, const String &status) {
  String firebasePath = "/slot" + String(slotIndex + 1);
  if (Firebase.RTDB.setString(&fbData, firebasePath, status)) {
    Serial.println("Slot " + String(slotIndex + 1) + " Status updated: " + status);
  } else {
    Serial.println("Failed to update Slot " + String(slotIndex + 1) + " status: " + fbData.errorReason());
  }
}

void loop() {
  // Read sensor values and update statuses
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = digitalRead(slotPins[i]);
    if (i < 6) {
      // Update status for first 6 slots
      slotStatus[i] = (sensorValues[i] == LOW) ? "FULL" : "EMPTY";
      updateFirebase(i, slotStatus[i]);
    }
  }

  // Display status on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("S1:");
  lcd.print(slotStatus[0] == "FULL" ? "F" : "E");
  lcd.setCursor(5, 0);
  lcd.print("S2:");
  lcd.print(slotStatus[1] == "FULL" ? "F" : "E");
  lcd.setCursor(10, 0);
  lcd.print("S3:");
  lcd.print(slotStatus[2] == "FULL" ? "F" : "E");

  lcd.setCursor(0, 1);
  lcd.print("S4:");
  lcd.print(slotStatus[3] == "FULL" ? "F" : "E");
  lcd.setCursor(5, 1);
  lcd.print("S5:");
  lcd.print(slotStatus[4] == "FULL" ? "F" : "E");
  lcd.setCursor(10, 1);
  lcd.print("S6:");
  lcd.print(slotStatus[5] == "FULL" ? "F" : "E");

  // Check if all slots are full
  bool allFull = true;
  for (int i = 0; i < 6; i++) {
    if (slotStatus[i] == "EMPTY") {
      allFull = false;
      break;
    }
  }

  if (allFull) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("All Slots Full");
  }

  // Control servo for slots 7 and 8
  if (sensorValues[6] == LOW || sensorValues[7] == LOW) {
    servo1.write(90); // Open gate
    delay(3000);
    servo1.write(0);  // Close gate
  } else {
    servo1.write(0);  // Keep gate closed
  }

  // Read LED status from Firebase
  if (Firebase.ready() && Firebase.RTDB.getString(&fbData, "/ledStatus")) {
    String ledStatus = fbData.stringData();
    Serial.println("LED Status: " + ledStatus);
    digitalWrite(ledPin, (ledStatus == "ON") ? HIGH : LOW);
  } else {
    Serial.println("Failed to read LED status from Firebase: " + fbData.errorReason());
  }

  delay(2000); // Delay for 2 seconds before the next loop
}
