#define SLAVE_ADDRESS "0022,12,0232DB"
#define GREENLED 4
#define REDLED 6

void setup() {
  pinMode(GREENLED, OUTPUT);
  pinMode(REDLED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Check if AT command is working
  Serial.println("AT");
  delay(500); // Wait for response
  
  if (Serial.find("OK")) {
    // AT command is working
    Serial.println("AT command is working");

    // Get version
    Serial.println("AT+VERSION?");
    delay(500);
    if (Serial.find("+VERSION:")) {
      String version = Serial.readStringUntil('\n');
      Serial.println("Version: " + version);
    }

    // Get role
    Serial.println("AT+ROLE?");
    delay(500);
    if (Serial.find("+ROLE:")) {
      int role = Serial.parseInt();
      if (role == 1) {
        Serial.println("Role: Slave");

        // Set mode
        Serial.println("AT+CMODE=1");
        delay(500);

        // Inquiry
        Serial.println("AT+INQ");
        delay(500);

        // Check for slave address
        if (SLAVE_ADDRESS != "EXISTS") {
          // Display error message
          Serial.println("Error: Slave address does not exist");
        } else {
          // Bind to slave address
          Serial.println("AT+BIND=" + String(SLAVE_ADDRESS));
          delay(500);

          // Check state
          Serial.println("AT+STATE?");
          delay(500);
          if (Serial.find("READY")) {
            // Pair
            Serial.println("AT+PAIR=" + String(SLAVE_ADDRESS) + ",20");
            delay(500);

            // Check state
            Serial.println("AT+STATE?");
            delay(500);
            if (Serial.find("PAIRED")) {
              // Connect
              Serial.println("AT+LINK=" + String(SLAVE_ADDRESS));
              delay(500);

              // Check state
              Serial.println("AT+STATE?");
              delay(500);
              if (Serial.find("CONNECTED")) {
                // Green LED on, Red LED off
                digitalWrite(GREENLED, HIGH);
                digitalWrite(REDLED, LOW);
                Serial.println("ALERT: Emergency");
                // Sound buzzer
              } else {
                Serial.println("Error: Not connected");
              }
            } else {
              Serial.println("Error: Not paired");
            }
          } else {
            Serial.println("Error: Not ready");
          }
        }
      } else {
        Serial.println("Error: Invalid role");
      }
    }
  } else {
    // AT command failed
    Serial.println("Error: AT command failed");
  }
}
