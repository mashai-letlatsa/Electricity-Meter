Electricity Meter
This project is designed to measure and display electrical parameters such as voltage, current, power, and accumulated energy using an AVR microcontroller. It also includes functionality to control a relay based on energy consumption.

Table of Contents
Description
Features
Installation
Usage
Contributing
License
Contact
Description
Electricity Meter is a firmware for an AVR microcontroller that measures voltage, current, power, and energy consumption. It uses an LCD to display these parameters and includes a relay control feature based on the energy balance.

Features
Voltage Measurement: Measures the peak voltage.
Current Measurement: Measures the peak current.
Power Calculation: Calculates power in kW.
Energy Calculation: Accumulates energy consumption in kWh.
Relay Control: Controls a relay based on energy balance.
LCD Display: Displays voltage, current, power, energy, and balance on an LCD.

Installation
Hardware Requirements
AVR Microcontroller (e.g., ATmega series)
LCD Display
Relay
Voltage and Current Sensors
Additional components: resistors, capacitors, etc.

Software Requirements
AVR-GCC (AVR C Compiler) or Microchip Studio
AVRDUDE (for uploading the code to the microcontroller)
Proteus 8.13 for simulation

Steps
Clone the repository
git clone https://github.com/mashai-letlatsa/Electricity-Meter.git
Navigate to the project directory
cd Electricity-Meter
Build the project
make
Upload the firmware to your microcontroller
make upload


Usage
Connect the hardware: Ensure all components (sensors, LCD, relay) are connected correctly to the microcontroller as per the code configuration.
Power up the system: Supply power to the microcontroller and connected components.
Monitor the LCD: The LCD will display the measured voltage, current, calculated power, accumulated energy, and the remaining balance.
Relay control: The relay will be controlled automatically based on the energy balance.
Contributing
Contributions are welcome! Please follow these steps to contribute:

Fork the repository.
Create a new branch (git checkout -b feature-branch).
Make your changes.
Commit your changes (git commit -m 'Add new feature').
Push to the branch (git push origin feature-branch).
Open a pull request.

License
This project is licensed under the MIT License. See the LICENSE file for more details.

Contact
For any questions or support, please contact mashailetlatsa@gmail.com or kopanangletlatsa@gmail.com.
