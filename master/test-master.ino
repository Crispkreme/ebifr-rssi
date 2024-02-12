//  Pins Configuration HC-05
//  BT VCC to Arduino 5V out. 
//  BT GND to GND
//  BT RX to Arduino pin 3 (through a voltage divider)
//  BT TX to Arduino pin 2 (no need voltage divider)

// AT COMMANDS
// mySerial.print("AT\r\n");
// mySerial.print("AT+RMAAD\r\n");
// mySerial.print("AT+ROLE=1\r\n");  
// mySerial.print("AT+NAME="+String(BT_NAME)+"\r\n");
// mySerial.print("AT+PSWD=\"1234\"\r\n");
// mySerial.print("AT+BIND="+String(SLAVE_ADDRESS)+"\r\n");
// mySerial.print("AT+UART=38400,0,0\r\n");
// mySerial.print("AT+UART?\r\n");

#include <SoftwareSerial.h>

SoftwareSerial bluetoothSerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);
  bluetoothSerial.begin(9600); // Set the baud rate to match your HC-05 module
}

void loop() {
  // Send data to slave
  bluetoothSerial.println("Hello from master!");

  // Receive data from slave
  while (bluetoothSerial.available()) {
    char c = bluetoothSerial.read();
    Serial.print(c);
  }

  delay(1000);
}
