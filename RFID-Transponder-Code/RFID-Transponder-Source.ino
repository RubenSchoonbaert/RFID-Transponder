#include "ST25DVSensor.h"
#include <SPI.h>
#include <SD.h>

#if defined(ARDUINO_B_L4S5I_IOT01A)
  #define GPO_PIN PE4
  #define LPD_PIN PE2
  #define SDA_PIN PB11
  #define SCL_PIN PB10
  #define WireNFC MyWire
  TwoWire MyWire(SDA_PIN, SCL_PIN);
  ST25DV st25dv(12, -1, &MyWire);
#else
  #define DEV_I2C         Wire
  ST25DV st25dv(12, -1, &DEV_I2C);
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

void clearFile(const char* filename) {
  if (SD.begin(chipSelect)) {
    // Open the file in write mode without appending
    File myFile = SD.open(filename, FILE_WRITE | O_TRUNC);
    
    if (myFile) {
      myFile.close();
      Serial.println("File cleared!");
    }
  }
}

void printLine(){
  for(int i = 0; i < 50; i++){
    Serial.print('-');
  }
  Serial.println();
}

void printStart(){
  Serial.println(F("         ╔═══╦═══╦══╦═══╗"));
  Serial.println(F("         ║╔═╗║╔══╩╣╠╩╗╔╗║"));
  Serial.println(F("         ║╚═╝║╚══╗║║─║║║║"));
  Serial.println(F("         ║╔╗╔╣╔══╝║║─║║║║"));
  Serial.println(F("         ║║║╚╣║──╔╣╠╦╝╚╝║"));
  Serial.println(F("         ╚╝╚═╩╝──╚══╩═══╝"));  
  printLine();
  Serial.println(F("By Ruben Schoonbaert"));
  Serial.println(F("Written 29/12/2023"));
  printLine();
  delay(1000);
  Serial.println(F("COMMANDS:"));
  Serial.println(F("$printFile -> prints all records on the SD card"));
  Serial.println(F("$clearFile -> clears all records on the SD card"));
  Serial.println(F("$clearRFID -> removes last entry from RFID tag"));
  Serial.println(F("$addRecord <record> -> Adds new record to SD & RFID Tag"));
  Serial.println(F("$testBuzzer -> Tests integrated piezo buzzer"));
  Serial.println(F("$testLED -> Tests integrated LED"));
  printLine();
}

void printFile(const char* filename) {
  if (SD.begin(chipSelect)) {
    // Open the file in read mode
    File myFile = SD.open(filename, FILE_READ);
    printLine();
    Serial.println(F("    Printing SD-Card content    "));
    printLine();
    if (myFile) {
      while (myFile.available()) {
        Serial.write(myFile.read());
      }
      
      myFile.close();
      printLine();
    } else {
      Serial.println(F("Error opening file for reading."));
      digitalWrite(25, HIGH);
    }
  }
}

bool isDuplicate(const char* filename, const String& newString) {
  File file = SD.open(filename);
  if (file) {
    while (file.available()) {
      String line = file.readStringUntil('\n');
      line.trim();
      if (line.equals(newString)) {
        file.close();
        return true; // Duplicate found
      }
    }
    file.close();
  }
  return false; // No duplicate found
}

void addRecord(String record){
  const char uri_write_protocol[] = URI_ID_0x01_STRING;
  if(st25dv.writeURI(uri_write_protocol, record, "")) {
    Serial.println(F("Write to RFID Tag failed!"));
    digitalWrite(25, HIGH);
    while(1);
  } else {
    if(record != "!ignore"){
    Serial.print(F("Written : \""));
    Serial.print(record);
    Serial.print(F("\" to RFID tag succesfully"));
    Serial.println();
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
  printStart();
  // The wire instance used can be omitted in case you use default Wire instance
  if(st25dv.begin() == 0) {
    Serial.println(F("System Init done!"));
    beep(2000, 100, 50);
  } else {
    Serial.println("System Init failed!");
    digitalWrite(25, HIGH);
    while(1);
  }
  delay(500);
  digitalWrite(25, LOW);
}

void loop() {  
  String uri_read;
   if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    
    if (command.equals("$clearFile")) {
      clearFile("records.txt");
      cleared = true;
      beep(2000, 100, 50);
      return;
    } else if (command.equals("$printFile")){
       beep(2000, 100, 50);
      printFile("records.txt");
    } else if (command.startsWith("$addRecord ")) {
      // Extract the record from the command
      String recordToAdd = command.substring(11); 
      // Add the record to the file
      addRecord(recordToAdd);
    } else if (command.equals("$clearRFID")){
       addRecord("!ignore");
       Serial.println(F("Cleared RFID"));
         beep(2000, 100, 50);
    } else if (command.equals("$testLED")){
      bool state = false;
      for(int i = 0; i<10; i++){
        digitalWrite(25, state);
        state = !state;
        delay(100);
      }
    }
     else if (command.equals("$testBuzzer")){
     int melody[] = {262, 330, 392, 523, 392, 330, 262, 196, 262};
     for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
      delay(10);
      beep(melody[i], 200, 100);
     }

    } else {
      Serial.println("unrecognised command!");
      beep(500, 500, 500);
    }
  }
  
  if(st25dv.readURI(&uri_read)) {
    Serial.println("Read failed!");
    digitalWrite(25, HIGH);
    while(1);
  } 
  if(uri_read != last_read){
     if (SD.begin(chipSelect)) {
      if(uri_read.startsWith("www.")){
        uri_read = uri_read.substring(4);
      }
      else if(uri_read.startsWith("http://www.")){
        uri_read = uri_read.substring(11);
      }
      else if(uri_read.startsWith("http://")){
        uri_read = uri_read.substring(7);
      }
      else if(uri_read.startsWith("https://")){
        uri_read = uri_read.substring(8);
      }
      if(uri_read != "!ignore"){
        // Check if the last line of the file is the same as the string
        if (!isDuplicate("records.txt", uri_read)) {
          if(!cleared){
            Serial.print(F("New record detected = "));
          } else {
            Serial.print(F("current RFID record = "));
          }
          cleared = false;
          Serial.print(uri_read);
          Serial.println();
          
          File myFile = SD.open("records.txt", FILE_WRITE);
          if (myFile) {
            // Write to the file
            myFile.println(uri_read);
            // Close the file
            myFile.close();
            delay(100);
            beep(800, 100, 50);
            delay(10);
            beep(1000, 100, 50);
           }
          }
        }
       last_read = uri_read;
     }
  }
  delay(500);
  digitalWrite(25, LOW);
} 
