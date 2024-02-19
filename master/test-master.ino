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

// MASTER HC-05
// PIN CONFIGURATION
// HC-05   ->  ARDUINO UNO
// EN      ->  3.3V
// VCC     ->  5V
// GND     ->  GND
// TX      ->  10
// RX      ->  11

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
void setup()
{

  Serial.begin(19200);
  while (!Serial)
  {
  }

  Serial.println("Welcome to RSSI Transmitter");

  mySerial.begin(38400);

}

void loop()
{

  // #DEFINE SLAVE_ADDRESS 0022,12,0232DB
  // #DEFINE GREENLED 4
  // #DEFINE REDLED 6
  
  // CHECK IF AT COMMAND IS WORKING
  // AT
  // IF AT === OK
    // AT+VERSION?

    // DISPLAY THE VERSION IN MY SERIAL MONITOR

    // AT+ ROLE?
    // IF ROLE === 1

      // AT+CMODE=1
      // AT+INQ

      // IF SLAVE_ADDRESS = EXISTS
        // AT+BIND=SLAVE_ADDRESS
        // AT+STATE?

        // IF STATE === "READY" 
            // AT+PAIR=SLAVE_ADDRESS,20

            // AT+STATE?
            // IF STATE === "PAIRED" 

              // AT+LINK=SLAVE_ADDRESS

              // AT+STATE?
              // IF STATE === "CONNECTED" 

                // SEND THIS INTO MY SLAVE
                // GREEN LED IS HIGH
                // RED LED IS LOW
                // DISPLAY MESSAGE "ALERT HAS A EMERGENCY"
                // PIZZO BUZZER WILL SOUND ON

                // AT+DISC 
                
                // AT+STATE?
                // IF STATE === "DISCONNECTED" 
                  // GREEN LED IS LOW
                  // RED LED IS HIGH
                  // DISPLAY MESSAGE "ALERT SEND"
                  // PIZZO BUZZER WILL SOUND OFF

                // IF STATE !== "DISCONNECTED" 
                // DISPLAY AT "ERROR MESSAGE"

              // IF STATE !== "CONNECTED" 
              // DISPLAY AT "ERROR MESSAGE"

            // IF STATE !== "PAIRED" 
            // DISPLAY AT "ERROR MESSAGE"

        // IF STATE !== "READY" 
        // DISPLAY AT "ERROR MESSAGE"

      // IF SLAVE_ADDRESS != EXISTS
      // DISPLAY AT "ERROR MESSAGE"

    // IF ROLE !== 1
    // DISPLAY AT "ERROR MESSAGE"

  // IF AT !== OK
  // DISPLAY AT "ERROR MESSAGE"
  

  if (mySerial.available())
  {
    Serial.write(mySerial.read());
  }

  if (Serial.available())
  {
    mySerial.write(Serial.read());
  }
}