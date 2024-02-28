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
**SLAVE**
ADDRESS - +ADDR:98D3:51:F94FC7
NAME    - +NAME:BLUETOOTH-1
PSWD    - 1234
ROLE    - 0

**MASTER**
ADDRESS - +ADDR:0022:12:0232DB
NAME    - +NAME:MASTER-BLUETOOTH
PSWD    - 1234
ROLE    - 1

**MASTER HC-05**
PIN CONFIGURATION
HC-05   ->  ARDUINO UNO
EN      ->  3.3V
VCC     ->  5V
GND     ->  GND
TX      ->  10
RX      ->  11

**SLAVE HC-06**
PIN CONFIGURATION
HC-06   ->  ARDUINO UNO
VCC     ->  5V
GND     ->  GND
TX      ->  10
RX      ->  11

## Desired output
COMMUNICATE 2 BLUETOOTH MODULE
PAIRING 2 BLUETOOTH MODULE
CONNECT2 BLUETOOTH DEVICE
CONNECTED - GREEN LED ON
          - DISPLAY "CONNECTED DEVICE"
          - SEND MESSAGE "EMERGENCY"
DISCONNECTED BLUETOOTH DEVICE
          - GREEN LED OFF
          - RED LED ON
          - DISPLAY "DISCONNECTED DEVICE"
