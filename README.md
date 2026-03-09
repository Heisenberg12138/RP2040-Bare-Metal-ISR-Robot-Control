📌 Project Overview
This repository contains a collection of five embedded firmware implementations for an RP2040-based autonomous robot. The project demonstrates a progression from high-level control to low-level hardware optimization, focusing on PWM register manipulation, Interrupt Service Routines (ISR), and real-time sensor fusion.

🚀 Modules & Assignments
1. Variable Speed Control (Hardware PWM)
Implementation: Directly configures the RP2040 hardware PWM slices using the hardware/pwm.h SDK.

Key Feature: Instead of standard analogWrite, it manipulates pwm_hw->slice[slice].cc registers for high-precision duty cycle control.

Logic: Includes a "kick-start" feature to overcome motor static friction and implements smooth acceleration/deceleration curves.

2. Interrupt-Driven Obstacle Detection
Implementation: Uses a non-blocking approach with External Interrupts (ISR) on the Echo pin.

Key Feature: Measures pulse duration via micros() inside an ISR, allowing the main loop to remain responsive.

Safety: Implements an emergency stop if an object is detected within 15cm.

3. Adaptive Line Follower
Implementation: A state-aware navigation system using IR reflective sensors.

Logic: Features a "Same Direction Counter" to adjust steering intensity. The longer the robot stays off-center, the sharper it turns (TURN_SLIGHT vs TURN_SHARP), preventing over-oscillation on straight lines while handling tight curves.

4. Autonomous Obstacle Avoidance
Implementation: A complex state machine that integrates line following with ultrasonic scanning.

Logic: * Phase 1 (Scan): Incremental scanning to find a clear path.

Phase 2 (Maneuver): Executes a timed detour.

Phase 3 (Re-acquisition): Performs a secondary scan and a "Big Right" turn logic to re-locate and lock back onto the black line.

5. Fixed-Distance Odometry
Implementation: Precision movement using a physical speed encoder.

Key Feature: Implements Software Debouncing inside the ISR (if (now - lastEncoderTime > 300)) to filter out mechanical noise from the encoder wheel.

Calibration: Converts raw encoder "ticks" to centimeters using a calibrated constant (0.52 cm/tick).

🛠 Hardware Mapping
MCU: Raspberry Pi Pico (RP2040)

Motor Driver: L9110 H-Bridge (Pins 18, 19, 20, 21)

Ultrasonic: Trigger (Pin 7), Echo (Pin 6)

IR Sensors: Left (A1/GPIO27), Right (A0/GPIO26)

Encoder: Left Wheel (Pin 3)

⚙️ How to Use
Ensure you have the Raspberry Pi Pico SDK and Arduino-Pico core installed.

Each assignment is a standalone .ino / .cpp file.

Upload the desired assignment to the Pico via USB.

Monitor real-time telemetry via the Serial Monitor (115200 baud).
