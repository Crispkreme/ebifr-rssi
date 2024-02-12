//  Pins Configuration HC-05
//  BT VCC to Arduino 5V out. 
//  BT GND to GND
//  BT RX to Arduino pin 3 (through a voltage divider)
//  BT TX to Arduino pin 2 (no need voltage divider)
 
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial bluetoothSerial(10, 11); // RX, TX
LiquidCrystal_I2C lcd(0x27, 20, 4); // I2C address 0x27, 20 column and 4 rows

const int buzzer = 9; //buzzer to arduino pin 9

void setup() {
  Serial.begin(9600);
  bluetoothSerial.begin(38400);

  lcd.init(); // initialize the lcd
  lcd.backlight();

  pinMode(buzzer, OUTPUT);
  
  Serial.println("Initiating Bluetooth inquiry...");
  bluetoothSerial.println("AT+INQ");
}

void loop() {
  while (bluetoothSerial.available()) {

    String response = bluetoothSerial.readStringUntil('\r\n');

    if (response.startsWith("+INQ:")) {

      // Extract device address and name from response
      int startPos = response.indexOf(',') + 1;
      int endPos = response.lastIndexOf(',');
      String deviceAddress = response.substring(startPos, endPos);
      
      // Pair with the device
      if (pairWithDevice(deviceAddress)) {

        // Connect to the device
        Serial.print("PAIRED DEVICE: ");
        Serial.println(deviceAddress);

        if (connectToDevice(deviceAddress)) {

          // Connect from the device
          Serial.print("CONNECTED DEVICE: ");
          Serial.println(deviceAddress);

          // DISPLAY THE DATA HERE
          lcd.setCursor(0, 0);            
          lcd.print("EBIFR-RSSI");          
          lcd.setCursor(0, 1);            
          lcd.print("Message:"); 
          lcd.setCursor(0, 2);            
          lcd.print("Device is in range");          
          lcd.setCursor(0, 3);            
          lcd.print("Status: Data Send"); 

          disconnectFromDevice(deviceAddress);
        }
      }
    }
  }
}

bool pairWithDevice(String deviceAddress) {

  Serial.print("Pairing with device: ");
  Serial.println(deviceAddress);
  bluetoothSerial.print("AT+PAIR=" + deviceAddress);
  delay(1000); // Wait for pairing response
  return bluetoothSerial.find("OK+PAIR");

}

bool connectToDevice(String deviceAddress) {

  Serial.print("Connecting to device: ");
  Serial.println(deviceAddress);
  bluetoothSerial.print("AT+LINK=" + deviceAddress);
  delay(1000); // Wait for connection response
  return bluetoothSerial.find("OK+CONN");

}

void disconnectFromDevice(String deviceAddress) {

  Serial.print("Disconnecting from device: ");
  Serial.println(deviceAddress);
  bluetoothSerial.print("AT+DISC=" + deviceAddress);
  delay(1000); // Wait for disconnection response

}
