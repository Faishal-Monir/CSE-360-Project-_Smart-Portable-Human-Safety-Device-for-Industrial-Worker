# Smart Portable Human Safety Device for Industrial Worker

## CSE:360 [ SUMMER 2025 ]

![Arduino](https://img.shields.io/badge/Arduino-00979D?logo=arduino&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?logo=cplusplus&logoColor=white)
![IoT](https://img.shields.io/badge/IoT-Safety%20Monitoring-blue)
![RFID](https://img.shields.io/badge/RFID-Access%20Control-orange)
![Bluetooth](https://img.shields.io/badge/Bluetooth-HC--05-0082FC?logo=bluetooth&logoColor=white)
![LCD](https://img.shields.io/badge/LCD-I2C%2016x2-green)

A CSE360 Computer Interfacing project based on an Arduino-powered smart industrial safety vest.  
The system monitors environmental and worker safety conditions using multiple sensors and provides real-time alerts through an LCD display, buzzer, LED indicator, RFID authentication, and Bluetooth serial communication.

<p align="center">
  <img src="./final_vest.png" alt="Smart Safety Vest Prototype" width="38%">
  <img src="./raw_circuit.jpg" alt="Arduino Hardware Prototype" width="56%">
</p>

<p align="center">
  <img src="./block_diagram.png" alt="System Flowchart / Block Diagram" width="48%">
  <img src="./circuit_diagram.png" alt="Circuit Diagram" width="48%">
</p>

---

## Table of Contents

- [Repository Highlights](#repository-highlights)
- [Project: Smart Portable Human Safety Device](#project-smart-portable-human-safety-device)
- [Course Information](#course-information)
- [Project Overview](#project-overview)
- [Features](#features)
- [Hardware Components](#hardware-components)
- [Communication Protocols Used](#communication-protocols-used)
- [System Workflow](#system-workflow)
- [Pin Configuration](#pin-configuration)
- [Required Arduino Libraries](#required-arduino-libraries)
- [Project Group Members](#project-group-members)
- [License](#license)

---

## Repository Highlights

- Arduino UNO-based wearable safety vest prototype
- Real-time industrial worker safety monitoring
- RFID authentication system for authorized access
- Gas and air quality monitoring using MQ sensors
- Temperature and pulse monitoring
- Basic fall or sudden movement detection using MPU6050
- Emergency SOS button with buzzer/LED alert
- Bluetooth serial alert system using HC-05
- LCD-based live system status display
- Includes project images, block diagram, circuit diagram, and source code


---

## Project: Smart Portable Human Safety Device

This project is a smart industrial safety vest designed for workers in risky environments. It combines sensors, RFID authentication, Bluetooth communication, and real-time alerts to detect possible danger and improve worker safety.

The prototype focuses on computer interfacing concepts by connecting hardware sensors and output devices with an Arduino UNO microcontroller.

---

## Course Information

**Course:** CSE360: Computer Interfacing  
**Offered:** Summer 2025  
**Pre-requisite:** CSE341  

This course gives an overview of computer interface components and their characteristics. It covers programmable interfacing ICs, sensors, hardware and software calibration for sensor interfacing, processing devices that connect computers with the real world, and output devices. The course also includes high-power interface devices, LED, LCD, seven-segment display, transducers, stepper motors, and peripheral devices. In addition, it introduces software simulation tools to develop interfacing systems. Advanced topics like Human Computer Interface (HCI) and Brain Computer Interface (BCI) are also discussed with case studies.

---

## Project Overview

Industrial workers often face safety risks such as gas leakage, poor air quality, sudden falls, high temperature, and emergency health conditions. This project presents a wearable safety vest that can monitor these risks in real time.

The system uses an **Arduino UNO** as the main microcontroller. Different sensors collect environmental and physiological data, while the LCD displays live information. If danger is detected, the system activates the buzzer/LED and sends a Bluetooth serial alert to a paired device.

---

## Features

- RFID-based access control for authorized users
- MQ-2 gas sensor for smoke/gas leakage detection
- MQ-135 sensor for air quality monitoring
- DHT11 sensor for temperature monitoring
- Pulse sensor for BPM/heart-rate display
- MPU6050 accelerometer for basic fall or sudden movement detection
- Emergency SOS button for manual distress alert
- Buzzer and LED-based local warning system
- HC-05 Bluetooth module for serial alert transmission
- 16x2 I2C LCD for real-time sensor data display
- Arduino UNO-based embedded hardware implementation

---

## Hardware Components

| Component | Purpose |
|---|---|
| Arduino UNO | Main microcontroller |
| MQ-2 Gas Sensor | Detects smoke, LPG, methane, and gas leakage |
| MQ-135 Air Quality Sensor | Detects air quality changes |
| DHT11 Sensor | Reads temperature |
| Pulse Sensor | Measures heart rate/BPM |
| MPU6050 | Detects sudden movement or fall-like impact |
| RC522 RFID Module | Provides authentication access |
| HC-05 Bluetooth Module | Sends serial alerts to a paired device |
| 16x2 I2C LCD | Displays sensor data and system status |
| Push Button | Emergency SOS trigger |
| Buzzer + LED | Local alarm and warning indicator |
| Jumper Wires and Breadboard | Circuit connection and prototyping |
| Safety Vest | Wearable prototype body |

---

## Communication Protocols Used

| Protocol | Used For |
|---|---|
| I2C | LCD display and MPU6050 |
| SPI | RC522 RFID module |
| UART / SoftwareSerial | HC-05 Bluetooth module |
| Analog Input | MQ-2, MQ-135, and pulse sensor |
| Digital Input/Output | Button, buzzer, DHT11 |

---

## System Workflow

1. The system starts and shows a welcome message.
2. The user scans an RFID card.
3. If the RFID UID is correct, access is granted.
4. After authentication, the LCD shows live sensor data.
5. The system monitors gas, air quality, temperature, pulse, and movement.
6. If danger or sudden movement is detected, the buzzer/LED turns on.
7. If the SOS button is pressed, the system sends an emergency alert.
8. Bluetooth sends alert messages such as `DANGER` or `ALARM`.
9. Scanning the authorized RFID card again locks the system.

---

## Pin Configuration

| Module | Arduino Pin |
|---|---|
| MQ-2 Sensor | A0 |
| MQ-135 Sensor | A1 |
| Pulse Sensor | A2 |
| DHT11 | D8 |
| Buzzer / LED | D6 |
| Emergency Button | D7 |
| HC-05 Bluetooth RX/TX | D2, D3 |
| RC522 RFID SDA/SS | D10 |
| RC522 RFID RST | D9 |
| RC522 RFID MOSI | D11 |
| RC522 RFID MISO | D12 |
| RC522 RFID SCK | D13 |
| LCD I2C SDA/SCL | A4, A5 |
| MPU6050 SDA/SCL | A4, A5 |

---

## Required Arduino Libraries

Install the following libraries in Arduino IDE:

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <MFRC522.h>
```

---

### Project Group Members:

[Rifat Mahmud Tamim](https://github.com/RIFAT-MAHMUD-TAMIM-00) |[Rajin Ibna Rajuanur Rahman](https://github.com/rajin50) | [Umma Salma Mim](https://github.com/ummasalmamim) | [Faishal Monir](https://github.com/Faishal-Monir)

---

## License

This project is developed for academic purposes as part of **CSE360: Computer Interfacing** at BRAC University.