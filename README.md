# Development of a Radar System using Ultrasonic Sensor and Servo Motor Control with STM32 Microcontroller

## Project Overview
This project demonstrates the implementation of a radar system using an STM32 microcontroller. It utilizes an ultrasonic sensor mounted on a servo motor to detect nearby objects within a 180-degree range. When an object is detected, the system provides visual and auditory feedback using LEDs and a buzzer.

## Features
- Object detection within 25 cm using an ultrasonic sensor.
- Servo motor control for 180-degree rotation.
- Real-time feedback:
  - Red LED blinking upon detection.
  - Buzzer sound for auditory indication.

## Components Used
- STM32 Microcontroller
- HC-SR04 Ultrasonic Sensor
- Futaba S3003 Servo Motor
- LEDs (Green and Red)
- Buzzer
- 7-Segment Display
- Resistors (220 Ω)
- Breadboard and Jumper Wires

## How It Works
1. **Initialization**: A green LED lights up when the system is powered on.
2. **Scanning Mode**:
   - The servo motor rotates the ultrasonic sensor from 0 to 180 degrees.
   - The ultrasonic sensor emits ultrasonic pulses and measures distances by detecting echoes.
3. **Object Detection**:
   - If an object is detected within 25 cm:
     - The red LED blinks.
     - The buzzer beeps in sync with the LED.
4. **Continuous Operation**: The servo motor continues scanning while monitoring the range for objects.

## Circuit Diagram
Refer to the **Appendix** section in the [Project Report](PROJECT_REPORT_doc.pdf) for the detailed circuit connections.

### Pin Configuration
| Device             | STM32 Board (PIN)  |
|--------------------|--------------------|
| **Servo Motor**    | PB5 (PWM), 5V, GND |
| **Ultrasonic Sensor** | PB7 (Trigger), PB3 (Echo), 5V, GND |
| **Green LED**      | PD0                |
| **Red LED & Buzzer** | PA0               |

## Results
- **Scanning Mode**: The servo motor smoothly rotates the ultrasonic sensor, scanning for objects.
- **Detection Mode**: Red LED blinks and the buzzer emits sound when an object is detected.

## Getting Started
### Prerequisites
- STM32 IDE (e.g., Keil, STM32CubeIDE)
- STM32F4 board
- Basic knowledge of embedded programming and circuit design

### Steps to Run
1. Clone this repository:
   ```bash
   git clone <repository_url>
2. Open the provided code in your STM32 IDE.
3. Connect the components as per the circuit diagram.
4. Compile and upload the code to the STM32 microcontroller.
5. Power on the system and observe the functionality.
