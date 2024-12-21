#include "ST25DVSensor.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h> // Include Wire library for I2C communication

#if defined(ARDUINO_B_L4S5I_IOT01A)
  #define GPO_PIN PE4
  #define LPD_PIN PE2
  #define SDA_PIN PB11
  #define SCL_PIN PB10
  TwoWire MyWire(SDA_PIN, SCL_PIN); // Define TwoWire instance for I2C
  ST25DV st25dv(12, -1, &MyWire); // Initialize ST25DV sensor with MyWire
#else
  #define DEV_I2C Wire
  ST25DV st25dv(12, -1, &DEV_I2C); // Initialize ST25DV sensor with default Wire
#endif

String last_read;
bool cleared = false;
const int chipSelect = 7;

void beep(int pitch, int duration, int loudness){
  tone(24, pitch, loudness);
  delay(duration);
  noTone(24);
  delay(10);
}

void printStart(){
  Serial.println(F("         ╔═══╦═══╦══╦═══╗"));
  Serial.println(F("         ║╔═╗║╔══╩╣╠╩╗╔╗║"));
  Serial.println(F("         ║╚═╝║╚══╗║║─║║║║"));
  Serial.println(F("         ║╔╗╔╣╔══╝║║─║║║║"));
  Serial.println(F("         ║║║╚╣║──╔╣╠╦╝╚╝║"));
  Serial.println(F("         ╚╝╚═╩╝──╚══╩═══╝"));  
  Serial.println(F("By Ruben Schoonbaert"));
  Serial.println(F("Written 29/12/2023"));
}

void setup() {
  Serial.begin(9600);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
  printStart();
  
  // Initialize ST25DV sensor
  if(st25dv.begin() == 0) {
    Serial.println(F("ST25DV Sensor initialized successfully."));
    beep(2000, 100, 50);
  } else {
    Serial.println(F("ST25DV Sensor initialization failed!"));
    digitalWrite(25, HIGH);
    while(1);
  }
  
  delay(500);
  digitalWrite(25, LOW);

  // Begin I2C communication (only if using MyWire instance)
  #if !defined(ARDUINO_B_L4S5I_IOT01A)
    DEV_I2C.begin();
    // Send message to ESP8266
    String message = "Test!";
    DEV_I2C.beginTransmission(8); // Replace with your ESP8266's I2C address
    DEV_I2C.print(message);
    byte error = DEV_I2C.endTransmission();
    
    if (error == 0) {
      Serial.println("Message sent successfully to ESP8266.");
    } else {
      Serial.print("Error sending message to ESP8266. Error code: ");
      Serial.println(error);
    }
  #endif
}

void loop() {
  // Your main loop code here
  
}
