# Step10V
This project uses a simple microcontroller to implement a PID control loop between a step/dir (or quadrature) CNC controller and an analogic servo amplifier.
The board uses the quadrature encoder signals from the servo amplifier or a glass scale (or servomotor if applicable) to get position feedback.

This is the key to modernize old CNC machines with a new CNC controller package (Machinekit, MACH3 etc) without having to invest in costly new motors, drives and power supply.

# Concept
Use the nucleo32L432 as the base module.
The STM can also be put on the PCB, then the nucleo is only the programmer/debugger.

If the STM is put directly on the PCB, USB DFU bootloader could be used to program it.

The output uses the 2 DAC outputs, which feeds a differential amplifier with a gain of 3.
Both DAC outputs can also output with a gain of 3 to support single +12V power input and drives that take two seperate inputs.

The external UART can be connected to FTDI cable through an isolator board, so that the configuration can be done safely.


# Pinout

**QEI from amplifier**  
* PA0 - TIM2_CH1 = QEI2 A
* PA1 - TIM2_CH2 = QEI2 B
* TODO:(PA3 - TIM2_CH3 = QEI2 I)

**QEI / step-dir from cnc**  
* PA8 - TIM1_CH1 = QEI1 A / STEP
* PA9 - TIM1_CH2 = QEI1 B / DIR

**Analog output**  
* PA4 - DAC1_OUT1
* PA5 - DAC1_OUT2

**Internal UART (debugger)**  
* PA2 - RX
* PA15 - TX

TODO:**External UART**  
* PB6 - TX
* PB7 - RX

**LEDs**  
* PB3 - Green LED

TODO:**USB**  
* PA11 - USB_DM
* PA12 - USB_DP

**OLED Screen**  
* PB4 - SDA SSD1306
* PA7 - SCL SSD1306

# Innerworking
## Main (startup)
The Main function only performs the init
* TODO:Read config
* Performs peripherals init
* Starts the Shell Thread
* Starts the control loop
* TODO:Starts the Watchdog

## Control loop
The PID loop is fired from a timer interrupt at regular interval. The interrupt has the highest priority so the PID loop has very low jitter.
This loop performs the current operations:
* Reading position setpoint
* Read feedback position
* Performs error calculation
* Performs limits / error checks
* Checks for state (enabled, disabled, error) of the regulator
* Performs PID calculation (floating point, using the intenal FPU)
* Updates the DAC outputs
* Updates status variables
* Resets Watchdog

## TODO:Watchdog
If the Watchdog is not triggered during the right interval, the output is put in a safe mode (output = 0V, FAULT=1).

## Shell
The shell is the main configuration interface. It allows to
* Read status
* Read/Write configuration
* Perform a step response test

# Project Status
This project is in early stage, it's actually closer to a Proof Of Concept than a finished product.

There is still a lot to do before I would consider this being ready for any serious work. Experiment at your own risks; but make sure you know what you are doing!
