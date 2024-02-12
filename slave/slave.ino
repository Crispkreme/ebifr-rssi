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
  
}

void loop() {
  // Receive data from master
  while (bluetoothSerial.available()) {
    char c = bluetoothSerial.read();
    Serial.print(c);
  }

  // Send data to master
  bluetoothSerial.println("Hello from slave!");
  
  // DISPLAY THE DATA HERE
  lcd.setCursor(0, 0);            
  lcd.print("EBIFR-RSSI");          
  lcd.setCursor(0, 1);            
  lcd.print("Message:"); 
  lcd.setCursor(0, 2);            
  lcd.print("Device is in range");          
  lcd.setCursor(0, 3);            
  lcd.print("Status: Data Receive"); 

  // ACTIVATE THE ALERT
  tone(buzzer, 1000); 
  delay(1000);       
  noTone(buzzer);
  delay(1000);

}
