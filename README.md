## Bluetooth-Based Emergency Response System with RSSI and MAODV

## Introduction
In times of crisis, efficient emergency response is paramount for ensuring the safety of individuals and communities. However, traditional emergency response mechanisms often face challenges in timely and coordinated actions. This project introduces a Bluetooth-based Emergency Response System, leveraging Received Signal Strength Indicator (RSSI) and Modified Ad hoc On-Demand Vector (MAODV) routing protocol, to enhance emergency response coordination and communication.

## Problem Statement
- **Detection System Development:** Develop a detection system capable of identifying target nodes (e.g., vehicles) within a Bluetooth network using RSSI.
- **Emergency Broadcast System Creation:** Create an emergency broadcast system using a modified MAODV protocol to disseminate emergency alerts to identified nodes based on RSSI readings.
- **System Evaluation:** Evaluate the performance of the developed system in terms of detection accuracy and response time under simulated emergency scenarios.

## Significance
This project addresses critical aspects of emergency response:

- **Improved Response Efficiency:** By leveraging Bluetooth technology and advanced routing protocols, the system enables swift and coordinated emergency response actions.
- **Enhanced Safety:** Rapid dissemination of emergency alerts enhances situational awareness and improves overall safety.
- **Scalability and Adaptability:** The system's design allows for scalability and adaptability to various emergency scenarios and infrastructure environments.
- **Future Research Opportunities:** This project lays the groundwork for further research in integrating emerging technologies for emergency management.
- **Practical Application of Technologies:** Demonstrates practical applications of Arduino Uno, Bluetooth modules, LEDs, and LCD displays in emergency response systems.

## Materials
The project utilizes the following materials:

- Arduino Uno microcontroller board
- Bluetooth modules HC-05/HC-06
- LED indicators
- I2C LCD display
- Arduino IDE for programming and development.

## Pin Configuration
**Slave**  
Address - +ADDR:98D3:51:F94FC7
Name - +NAME:BLUETOOTH-1
Pswd - 1234
Role - 0 

**Master**  
Address - +ADDR:0022:12:0232DB
Name - +NAME:MASTER-BLUETOOTH
Pswd - 1234
Role - 1

**Master HC-05**  
Pin configuration
HC-05 -> Arduino Uno
EN -> 3.3V
VCC -> 5V
GND -> GND
TX -> 10
RX -> 11 

**Slave HC-06**  
Pin configuration
HC-06 -> Arduino Uno
VCC -> 5V
GND -> GND
TX -> 10
RX -> 11  

## Desired output 
**Communicate 2 Bluetooth module**
Pairing 2 Bluetooth module
Connect2 Bluetooth device
Connected - Green LED on
- Display "Connected device"
- Send message "Emergency"
Disconnected Bluetooth device
- Green LED off
- Red LED on
- Display "Disconnected device"
