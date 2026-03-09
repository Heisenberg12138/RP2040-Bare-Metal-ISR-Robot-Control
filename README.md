# 🏎️ RP2040 Autonomous Robot: Bare-Metal & ISR Control

> **Autonomous Robotics Project** > A high-performance firmware implementation for an RP2040-powered robot, transitioning from high-level abstractions to direct hardware register manipulation and interrupt-driven logic.

---

## 📌 Project Overview
This repository contains five distinct firmware modules developed for an autonomous car robot. The project demonstrates advanced embedded techniques including **Hardware PWM configuration**, **External Interrupts (ISR)**, and **Real-time Sensor Fusion** for navigation and obstacle avoidance.



## 🛠️ Hardware Architecture
* **Controller:** Raspberry Pi Pico H (Dual-core ARM Cortex-M0+)
* **Sensing Suite:**
    * `HC-SR04`: Ultrasonic sensor for precision distance measurement.
    * `TCRT5000`: Infrared reflective sensors for surface detection.
    * `B83609`: Photoelectric encoders for odometry.
* **Actuators:** L9110 H-Bridge driver with dual DC gear motors.

---

## 🚀 Modules & Assignments

### 1️⃣ Variable Speed Control (Register-Level PWM)
* **Implementation:** Directly manipulates the `pwm_hw->slice` registers via the Pico SDK to manage motor duty cycles.
* **Technical Highlight:** Bypassed standard abstractions to configure hardware PWM slices and clock dividers manually. 
* **Logic:** Includes a **Kick-start** feature (initial high-power pulse) to overcome static friction of the gear motors.

### 2️⃣ Interrupt-Driven Obstacle Detection
* **Logic:** Utilizes `attachInterrupt` to capture signal changes on the Echo pin via an **Interrupt Service Routine (ISR)**.
* **Safety:** Implements an immediate emergency stop protocol when an object is detected within a **15cm** threshold.



### 3️⃣ Adaptive Line Follower
* **Algorithm:** Employs a "Keep-in" logic to maintain the black line between two IR sensors.
* **Innovation:** Features a **Same Direction Counter**. This dynamically scales steering intensity (from `SLIGHT` to `SHARP`) based on the duration of deviation, preventing oscillations on straightaways and "derailing" on tight curves.

### 4️⃣ Autonomous Obstacle Avoidance
* **Workflow:** 1. **Scanning Phase:** Incremental rotation to find a clear path.
    2. **Maneuver Phase:** Executes a timed detour detour.
    3. **Recovery Phase:** Uses side-sensor feedback and a "Big Right" turn logic to re-locate the line and resume navigation.

### 5️⃣ Fixed-Distance Traveling
* **Accuracy:** Achieves precision movement with a calibrated error margin of less than **10cm**.
* **Feature:** Integrated **Software Debouncing** within the encoder ISR (`micros()` checking) to filter out mechanical noise, ensuring reliable and accurate odometry data.
