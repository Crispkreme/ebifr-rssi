// LEGEND
// 1 --------> DEVICE CONNECTED
// 2 --------> DEVICE DISCONNECTED
#include <SPI.h>              
#include <Wire.h>
#include "HD44780_LCD_PCF8574.h"
#include <SoftwareSerial.h>

#define DISPLAY_DELAY_INIT 50
#define BUTTON_PIN A1
#define BUZZER 5

HD44780LCD myLCD(4, 16, 0x27, &Wire);
SoftwareSerial mySerial(10, 11); // RX, TX

char character;
int state = 0;
bool masterConnected = false;
unsigned long lastPingTime = 0;
const unsigned long pingInterval = 2000;

void setup() {
  pinMode(4, OUTPUT); // GREEN
  pinMode(6, OUTPUT);
  pinMode(BUZZER, OUTPUT);

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
  initializeCommand("AT+ADDR?");
  initializeCommand("AT+ROLE=0");
  initializeCommand("AT+UART=9600,0,0");
  initializeCommand("AT+UART?");
  initializeCommand("AT+CMODE=0");
}

void initializeCommand(const char* command) {
  mySerial.print(command);
  mySerial.print("\r\n");
  delay(100); // Adjust this delay if necessary

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

void connectDevice(const char* deviceName, const char* deviceAddress) {
  char command[50];
  sprintf(command, "AT+LINK=%s", deviceAddress);
  mySerial.println(command);
  delay(200); 
}

void successIndicator(String statusIndicator)
{
  String indicatorString = statusIndicator;
  myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 1);
  myLCD.PCF8574_LCDSendString(indicatorString.c_str());
  myLCD.PCF8574_LCDSendChar(' ');

  digitalWrite(4, HIGH);
  digitalWrite(6, LOW);
}

void failedIndicator(String statusIndicator)
{
  String indicatorString = statusIndicator;
  myLCD.PCF8574_LCDGOTO(myLCD.LCDLineNumberTwo, 1);
  myLCD.PCF8574_LCDSendString(indicatorString.c_str());
  myLCD.PCF8574_LCDSendChar(' ');

  digitalWrite(4, LOW);
  digitalWrite(6, HIGH);
}

void sound()
{
  int dotDuration = 200;
  int dashDuration = dotDuration * 3;
  int interCharPause = dotDuration;
  int frequency = 1000;

  for (int i = 0; i < 3; i++) {

    tone(BUZZER, frequency, dotDuration);
    delay(dotDuration * 2); 
    tone(BUZZER, frequency, dashDuration);
    delay(dotDuration * 2); 
  }
  delay(100); 
}

void loop() {

  sendCommand("AT");
  if (waitForResponse("OK")) {

    Serial.println("AT command is working");

    sendCommand("AT+VERSION?");
    if (waitForResponse("+VERSION:")) {
      String version = mySerial.readStringUntil('\r\n');
      Serial.println("Version: " + version);
    }

    sendCommand("AT+ROLE?");
    if (waitForResponse("+ROLE:")) {

      int role = mySerial.parseInt();
      if (role == 0) { // Check if slave role
        Serial.println("Role: SLAVE");

        sendCommand("AT+STATE?");
        delay(100);
        
        if (waitForResponse("+STATE:")) {
          processState("CONNECTED");

          String statusString = "DRIVE SAFELY";
          successIndicator(statusString);

          if (Serial.available() > 0) {
            mySerial.write('1');
          }

          if (mySerial.available() > 0) {
            String data = mySerial.readString();
            Serial.println("Received data: " + data);

            // Clear the buffer by reading and discarding remaining characters
            while (mySerial.available() > 0) {
              mySerial.read();
            }

            Serial.println("CLEAR PATH");
            String statusString = "CLEAR PATH";
            successIndicator(statusString);
            delay(1000);
          } 

          delay(100);
        }

        sendCommand("AT+STATE?");
        delay(100);

        if (waitForResponse("+STATE:")) {

          processState("DISCONNECTED");

          String statusString = "DRIVE SAFELY";
          successIndicator(statusString);
          delay(1000);

          if (Serial.available() > 0) {
            mySerial.write('2');
          }

        } else {

          Serial.println("Error: Invalid role");

          String statusString = "DISCONNECTED FAILED";
          failedIndicator(statusString); 

        }

      } else {

        Serial.println("Error: no role");

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
