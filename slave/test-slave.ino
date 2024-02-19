// TO DO
// COMMUNICATE 2 BLUETOOTH MODULE
// PAIRING 2 BLUETOOTH MODULE
// CONNECT2 BLUETOOTH DEVICE
// CONNECTED - GREEN LED ON
//           - DISPLAY "CONNECTED DEVICE"
// DISCONNECTED BLUETOOTH DEVICE
//           - GREEN LED OFF
//           - RED LED ON
//           - DISPLAY "DISCONNECTED DEVICE"

//CONFIGURATION
// SLAVE
// ADDRESS - +ADDR:98D3:51:F94FC7
// NAME    - +NAME:BLUETOOTH-1
// PSWD    - 1234
// ROLE    - 0

// MASTER
// ADDRESS - +ADDR:0022:12:0232DB
// NAME    - +NAME:MASTER-BLUETOOTH
// PSWD    - 1234
// ROLE    - 1

// SLAVE HC-06 
// PIN CONFIGURATION
// VCC     ->  5V
// GND     ->  GND
// TX      ->  10
// RX      ->  11

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
void setup()
{

  Serial.begin(9600);
  while (!Serial)
  {
  }

  Serial.println("Welcome to RSSI Reciever");

  mySerial.begin(9600);

}

void loop()
{
  if (mySerial.available())
  {
    Serial.write(mySerial.read());
  }
  if (Serial.available())
  {
    mySerial.write(Serial.read());
  }
}