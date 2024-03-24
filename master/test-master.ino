// LEGEND
// 1 --------> DEVICE CONNECTED
// 2 --------> DEVICE CONNECTED

#include <SPI.h>              
#include <Wire.h>
#include "HD44780_LCD_PCF8574.h"
#include <SoftwareSerial.h>

#define DISPLAY_DELAY_INIT 50
#define BUTTON_PIN A1

HD44780LCD myLCD(4, 16, 0x27, &Wire);

SoftwareSerial mySerial(10, 11); // RX, TX

char character;
int state = 0;

void setup() {
  pinMode(4, OUTPUT); // GREEN
  pinMode(6, OUTPUT);

  Serial.begin(19200);
  while (!Serial) {}

  delay(DISPLAY_DELAY_INIT);
  myLCD.PCF8574_LCDInit(myLCD.LCDCursorTypeOn);
  myLCD.PCF8574_LCDClearScreen();
  myLCD.PCF8574_LCDBackLightSet(true);
  myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberOne, 0);

  String gateOpenString = "Welcome to RSSI Reciever";
  myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 1);
  myLCD.PCF8574_LCDSendString(gateOpenString.c_str());
  myLCD.PCF8574_LCDSendChar(' ');

  Serial.println("Welcome to RSSI Reciever");
  mySerial.begin(38400);

  initializeBluetooth();
}

void initializeBluetooth() {
  initializeCommand("AT");
  initializeCommand("AT+RMAAD");
  initializeCommand("AT+NAME?");
  initializeCommand("AT+DISC=1");
  initializeCommand("AT+ADDR?");
  initializeCommand("AT+ROLE=1");
  initializeCommand("AT+CMODE=0");
  initializeCommand("AT+BIND=0021,13,155F0");
  initializeCommand("AT+UART=9600,0,0");
}

void initializeCommand(const char* command) {
  mySerial.print(command);
  mySerial.print("\r\n");
  delay(200);

  // Check for response
  while (mySerial.available()) {
    char c = mySerial.read();
    Serial.write(c);
  }
}

void sendCommand(const char* command) {
  mySerial.println(command);
  delay(200); // Wait for response
}

bool waitForResponse(const char* response) {
  unsigned long startTime = millis();
  size_t responseLength = strlen(response);
  size_t index = 0;
  
  while (millis() - startTime < 3000) { // Wait for maximum 3 seconds
    if (mySerial.available() > 0) {
      char c = mySerial.read();
      if (c == response[index]) {
        index++;
        if (index == responseLength) {
          return true;
        }
      } else {
        index = 0;
      }
    }
  }
  return false;
}

void processState(const char* state) {
  String response = mySerial.readStringUntil('\r\n');
  response.trim();
  if (response.indexOf(state) != -1) {
    Serial.println("STATE: " + String(state));
  }
}

void successIndicator(String statusIndicator)
{
  String indicatorString = "STATUS: " + statusIndicator;
  myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 1);
  myLCD.PCF8574_LCDSendString(indicatorString.c_str());
  myLCD.PCF8574_LCDSendChar(' ');

  digitalWrite(4, HIGH);
  digitalWrite(6, LOW);
}

void failedIndicator(String statusIndicator)
{
  String indicatorString = "STATUS: " + statusIndicator;
  myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 1);
  myLCD.PCF8574_LCDSendString(indicatorString.c_str());
  myLCD.PCF8574_LCDSendChar(' ');

  digitalWrite(4, LOW);
  digitalWrite(6, HIGH);
}

void loop() {

  // Check if AT command is working
  sendCommand("AT");
  delay(200); 

  if (waitForResponse("OK")) {

    Serial.println("AT command is working");

    // Get role
    sendCommand("AT+ROLE?");
    delay(200); 

    if (waitForResponse("+ROLE:")) {
      int role = mySerial.parseInt();
      if (role == 1) {
        Serial.println("Role: MASTER");

        // Check state
        sendCommand("AT+STATE?");
        delay(200); 

        if (waitForResponse("+STATE:")) {
          processState("READY");

          sendCommand("AT+STATE?");
          delay(200); 

          if (waitForResponse("+STATE:")) {

            processState("CONNECTED");

            String statusString = "DRIVE SAFELY";
            successIndicator(statusString);

            if (Serial.available() > 0) {
              mySerial.write(1);
            }  

            if(mySerial.available() > 0)
            { 

              String data = mySerial.readString();
              Serial.println("Received data: " + data);
              mySerial.println(data);

              // if (state == '1') {

              //   Serial.println("CONNECTED BLUETOOTH DEVICE");
              //   Serial.print(state);

              //   String statusString = "MAKE A WAY !!";
              //   successIndicator(statusString);

              // } else if (state == '0') {

              //   Serial.println("DISCONNECTED BLUETOOTH DEVICE");
              //   Serial.print(state);

              //   String statusString = "ERROR";
              //   failedIndicator(statusString);
              // }
            } 

            // Disconnect
            sendCommand("AT+DISC");
            delay(200); 

            sendCommand("AT+STATE?");
            delay(200); 

            if (waitForResponse("+STATE:")) {
              processState("DISCONNECTED");

              Serial.println("Disconnected successfully");
              
              if (Serial.available() > 0) {
                mySerial.write("2");
              }             

              String statusString = "DRIVE SAFELY";
              successIndicator(statusString);

            } else {
              
              Serial.println("Failed to disconnect");

              if (Serial.available() > 0) {
                mySerial.write("0");
              }    

              String statusString = "DISCONNECT FAILED";
              failedIndicator(statusString); 
            }
          }
        }
      } else {

        Serial.println("Error: Invalid role");

        String statusString = "INVALID ROLE";
        failedIndicator(statusString); 
      }
    }

  } else {

    Serial.println("Error: AT command failed");

    String statusString = "AT COMMAND FAILED";
    failedIndicator(statusString); 
  }

}
