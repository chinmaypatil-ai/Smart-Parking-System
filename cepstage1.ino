#include <ESP32Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// const char* ssid = "project";  // Change to your WiFi Network name
// const char* password = "testingon"; 

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize the LCD display with I2C address
Servo servo1;
// Define the pin for the single IR sensor
#define ir1 12
#define ir2 14
#define ir3 27
#define ir4 26
#define ir5 25
#define ir6 33
#define ir7 18
#define ir8 19

int s1 = 0,s2 = 0 ,s3 = 0,s4 = 0,s5 = 0,s6 = 0,s7 = 0,s8 = 0;

void setup() {
  lcd.init();

  Serial.begin(115200);

  // Setup pin mode for the sensor
  pinMode(ir1, INPUT);
  pinMode(ir2,INPUT);
  pinMode(ir3,INPUT);
  pinMode(ir4,INPUT);
  pinMode(ir5,INPUT);
  pinMode(ir6,INPUT);
  pinMode(ir7,INPUT);
  pinMode(ir8,INPUT);
  pinMode(23, OUTPUT);
  servo1.attach(23);
  servo1.write(0);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Parking Status");
  delay(3000);
  lcd.clear();

}

void loop() {
  // Read sensor value
  int s1 = digitalRead(ir1);
  int s2 = digitalRead(ir2);
  int s3 = digitalRead(ir3);
  int s4 = digitalRead(ir4);
  int s5 = digitalRead(ir5);
  int s6 = digitalRead(ir6);
  int s7 = digitalRead(ir7);
  int s8 = digitalRead(ir8);

  // Print sensor value to Serial Monitor
  Serial.print("IR1: "); Serial.println(s1);
  Serial.print("\tIR2: "); Serial.print(s2);
  Serial.print("\tIR3: "); Serial.print(s3);
  Serial.print("\tIR4: "); Serial.print(s4);
  Serial.print("\tIR5: "); Serial.print(s5);
  Serial.print("\tIR6: "); Serial.println(s6);
  Serial.print("\tIR7: "); Serial.print(s7);
  Serial.print("\tIR8: "); Serial.println(s8);
  // Clear the display before updating
  lcd.clear();

  // Display status for the sensor
  lcd.setCursor(0, 0);
  if (s1 == 0) {
    lcd.print("S1:F");   // Display "Full" when object is detected
  } else {
    lcd.print("S1:E");  // Display "Empty" otherwise
  }
    lcd.setCursor(5, 0);
  if (s2==0){
    lcd.print("S2:F");
  }
  else{
    lcd.print("S2:E");
  }

  lcd.setCursor(10, 0);
  if (s3==0){
    lcd.print("S3:F");
  }
  else{
    lcd.print("S3:E");
  }

  lcd.setCursor(0, 1);
  if (s4==0){
    lcd.print("S4:F");
  }
  else{
    lcd.print("S4:E");
  }
  
  lcd.setCursor(5, 1);
  if (s5==0){
    lcd.print("S5:F");
  }
  else{
    lcd.print("S5:E");
  }
  
  lcd.setCursor(10, 1);
  if (s6==0){
    lcd.print("S6:F");
  }
  else{
    lcd.print("S6:E");
  }
  // if (s7==0 || s8==0){
  //   servo1.write(90);
  //   delay(3000);
  //   servo1.write(0);
  // }
  // else{
  //   servo1.write(0);
  // }
  if (s1 == 0 && s2 == 0 && s3 == 0 && s4 == 0 && s5 == 0 && s6 == 0){
    servo1.write(0);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("All Slots are");
    lcd.setCursor(6, 1);
    lcd.print("Full");
  }
  else{
    if (s7==0 || s8==0){
    servo1.write(90);
    delay(3000);
    servo1.write(0);
  }
  else{
    servo1.write(0);
  }
  }

  delay(1000); // Delay to control update speed
}
