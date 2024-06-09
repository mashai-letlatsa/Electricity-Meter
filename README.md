# Electricity Meter Unit

This project is an electricity meter unit that measures voltage, current, power, and energy consumption. The system also includes balance management and power control features. The electricity meter unit is built using an AVR microcontroller.

## Features

- **Voltage Measurement:** Measures and displays the peak voltage.
- **Current Measurement:** Measures and displays the peak current.
- **Temperature Measurement:** Measures the temperature of the water.
- **Power Calculation:** Calculates and displays the power consumption in kilowatts (kW).
- **Energy Calculation:** Calculates and displays the accumulated energy consumption in kilowatt-hours (kWh).
- **Balance Management:** Manages the user’s balance based on energy consumption.
- **Power Control:** Controls the power relay based on the remaining balance.

## Components

- AVR Microcontroller (ATmega32)
- Voltage Sensor
- Current Sensor
- Load
- Relay Switch
- LCD Display
- Miscellaneous components (resistors, capacitors, etc.)

## Circuit Diagram

![image](https://github.com/mashai-letlatsa/Electricity-Meter/assets/161247807/af46847f-1125-49df-9841-91bf3928aad0)


## Code Explanation

### Initialization Functions

- `lcdInit()`: Initializes the LCD display.
- `ADC_Init()`: Initializes the ADC for voltage and current measurement.
- `initTimer()`: Initializes the timer for time tracking.
- `lcdCommand()`: Sends a command to the LCD.
- `lcdData()`: Sends data to the LCD.

### Main Loop

The main loop continuously updates the electricity meter unit by performing the following tasks:

- Measures the peak voltage and current.
- Calculates and displays the power consumption.
- Calculates and displays the accumulated energy consumption.
- Manages the user’s balance based on energy consumption.
- Controls the power based on the remaining balance.

### Interrupt Service Routines (ISR)

- `ISR(TIMER1_COMPA_vect)`: Handles Timer1 Compare Match A interrupts for time tracking.

## How to Use

1. **Setup the hardware** according to the circuit diagram.
2. **Upload the code** to the AVR microcontroller.
3. **Run the system** and monitor the LCD display for real-time updates on voltage, current, power, energy consumption, and balance status.
4. **Adjust the settings** as necessary for your specific requirements.

## License

This project is licensed under the MIT License.


## Contact

For any questions or support, please contact Kopanang Letlatsa at mashailetlatsa@gmail.com or kopanangletlatsa@gmail.com.

