# Guardian-Ring
# The Guardian Ring: Edge-AI Safety Wearable
This repository contains the simulation logic and hardware architecture for the Guardian Ring prototype.

## The Prototype Concept
To validate our Edge-Computing logic, we built a Hardware-in-the-Loop simulation using SimulIDE. We abstracted the physical struggle detection into a tactile sequence trigger (a smart button) to prove our weighted algorithm can differentiate between accidental bumps and genuine emergencies.

## Wiring & Pinout
If you wish to replicate this simulation in SimulIDE, use an Arduino Uno and connect the components as follows:

| Component | Arduino Pin | Purpose |
| :--- | :--- | :--- |
| **Tactile Trigger (Button)** | Digital Pin 2 | Uses internal pull-up. Hardware interrupt for deep sleep. |
| **Button Ground** | GND | Completes the trigger circuit. |
| **RGB LED (Red)** | Digital Pin 11 | Indicates SOS Triggered. |
| **RGB LED (Green)** | Digital Pin 10 | Indicates System Armed / Low Power mode. |
| **RGB LED (Blue)** | Digital Pin 9 | Indicates Active Pattern Analysis. |
| **Haptic Motor** | Digital Pin 13 | Built-in LED simulates haptic vibration confirmation. |

*(Include the wiring_diagram.png image in your repository to see the visual layout).*

## How to Run the Simulation
1. Download `GuardianRing_Circuit.simu` and open it in SimulIDE.
2. Load the compiled `.hex` file onto the virtual Arduino Uno.
3. Open the Serial Monitor (Baud Rate: 9600).
4. **Test False Alarm:** Click the button 1-3 times. The system analyzes (Blue) and resets (Green).
5. **Test Emergency:** Rapidly click the button 5 times within 2 seconds. The SOS triggers (Red).
