<p align="center">
  <img width="759" height="493" alt="image" src="https://github.com/user-attachments/assets/eab27fe3-115a-420f-881d-3214ef43f4bd" />
</p>

# Tank-and-Controller
Controlled tank(two motors) via RF24 controller with one joystick.
Link for 3D parts:
  https://www.thingiverse.com/thing:5975570


# Crawler RC Tank Project
## About Project

Crawler, or Easier Tank, is a small remote-controlled vehicle (approximately 30x30x10 mm) steered by a homemade, simple controller using wireless communication. It's a hobbyistic project that I plan to develop further. However, in this document, I will only describe the foundational aspects of my project. I have also attached all the links to the GitHub code and 3D parts below, along with a description of how to print them.

## Required components

### For receiver - tank
*   Arduino Uno
*   Cytron 2A Motor Driver Shield
*   2.4GHz RF Transceiver Module + adapter
*   2x Yellow DC Motor 3-9V
*   Accumulator LiPo 7.4V
*   3D-printed parts

### For transmitter - controller
*   Arduino Nano
*   Joystick
*   Powerbank

---

### Cytron 2A Motor Driver Shield
<p align="center">
  <img width="300" alt="image" src="https://github.com/user-attachments/assets/d640c732-72c8-470d-82a1-c0abced7d91a" />
</p>

SHIELD-2A MOTOR is an Arduino shield for controlling dual DC motors up to 2A (peak) for each channel. It is compatible with Arduino Uno.
*   Bi-directional control for 2 brushed DC motors.
*   Support motor voltage ranges from 5V to 26V.
*   Maximum current up to 2A per channel.
*   3.3V and 5V logic level input.
*   Selectable pins for Signed Magnitude and Locked Anti-Phase.
*   External voltage polarity protector.

### 2.4GHz RF Transceiver

<p align="center">
  <img width="300" alt="image" src="https://github.com/user-attachments/assets/77e18edc-ae8f-41bb-942e-fedf454e1552" />
</p>

A cutting-edge wireless communication module designed for seamless connectivity within the globally accessible 2.4GHz ISM band.
*   Worldwide license-free 2.4GHz ISM band.
*   Max Output Power: ~20dBm.
*   Receiver Sensitivity: ~-95dBm.
*   1.9 to 3.6V power supply range.
*   5V tolerant inputs.

### 2.4GHz RF Transceiver Adapter

<p align="center">
  <img width="300" alt="image" src="https://github.com/user-attachments/assets/a2cc7a0e-7af7-455a-b3bb-b7d274c590b5" />
</p>

The AMS1117 integrated DC-DC voltage regulator regulates the voltages in the range from 4.8V to 12V, down to a stable 3.3V, which is required for the operation of the NRF24L01+ module.

### DC Motor

<p align="center">
  <img width="300" alt="image" src="https://github.com/user-attachments/assets/da496d63-4b3e-4eee-ab1b-49da18d6424d" />
</p>

> Meet the Double Shaft Geared Motor, a powerhouse with robust magnetism and exceptional anti-interference capabilities. Designed to thrive in a versatile voltage range of 3V to 12V DC, its optimal performance zone hovers around 6 to 9V. At 6V, experience efficient operation with a No-Load Current of ≤160mA and a No-Load Speed of 190±10% rpm, ensuring smooth and reliable functionality. The motor delivers a torque of 0.8kgf.cm at 6V, complemented by a turning speed ratio of 1:48, making it an ideal choice for applications demanding precision and power.

### Joystick

<p align="center">
  <img width="300" alt="image" src="https://github.com/user-attachments/assets/93015094-a25b-4e18-8a36-637d4e0b0543" />
</p>

Incorporating two potent potentiometers and a responsive button. This ingenious creation boasts three outputs: VRX and VRY, which provide position data through output voltage, and SW, a button with a stable state at 0. With convenient inputs like GND for ground and "+5V" for the voltage source, this device is geared to seamlessly integrate into various systems.

### 3D-printed parts

<p align="center">
  <img width="400" alt="image" src="https://github.com/user-attachments/assets/278778e9-2cae-4f06-8a50-068d1d0070c7" />
</p>

*   **Body:** Add a support on the bottom to get a clean print. The support has to be removed after printing. Infill: 15%, use PLA.
*   **Tracks:** For the project you need 64 tracks. Infill: 15%, use PLA.
*   **Small cogs:** You need 8 small cogs. Infill: 15%, use PLA.
*   **Side:** You need 2. Infill: 15%, use PLA.
*   **Side Bumper:** You need 2 of them. Infill: ~20-30%, use PLA.
*   **Main cog:** You need 4 main cogs. Infill: 90%, use PLA.
*   **Pins:** As a pin, you can use square paper clips.

## 2. Hardware description

### Tank:
To control two motors simultaneously, save on cables, and simplify the setup, I equipped myself with an Arduino shield: `2A Motor Driver Shield`. Before we proceed to connect everything, let's begin by setting up our shield. The first thing is to change the power source selection; for our project, it should be on **PWRIN** because we will use an external power supply for our motors.

<p align="center">
  <img width="600" alt="image" src="https://github.com/user-attachments/assets/eb9477b5-be20-421a-86f9-539954493972" />
</p>

The second thing we need to take care of is motor control selection by adjusting the four mini jumpers on the shield to **“Signed Magnitude”**. This allows us to control the phase and rotate motors in two directions.

<p align="center">
  <img width="600" alt="image" src="https://github.com/user-attachments/assets/00a19796-195d-46b2-8de2-030480bd014b" />
</p>

When everything is mounted correctly on the shield, we can move on to connecting it to the Arduino Uno. The next phase is to connect the radio transmitter via an adapter to the board. In the adapter, you have a total of 8 pins, and we will use only 7 of them:

*   `VCC` -> `5V` - Power Supply pin 5V
*   `GND` -> `GND` - Ground
*   `CE` -> `D9` - Chip Enable pin
*   `CSN` -> `~D10` - SPI Chip Select pin
*   `SCK` -> `D13` - SPI Clock pin
*   `MO` -> `~D11` - Slave Data Input pin (MOSI)
*   `MI` -> `D12` - Slave Data Output pin (MISO)
*   (*`IRQ` - Active Low Interrupt pin, which we won't use.*)

I have attached the complete tank circuit below, inclusive of the two motors.

<p align="center">
  <img width="600" alt="image" src="https://github.com/user-attachments/assets/2877d37f-bd78-498a-bdc6-a6b6dbde7b7b" />
</p>

> **Note:** It's not crucial how you connect the motors (left and right side); later, you can simply shift the cable from A to B or from B to A (change the phase) to alter the rotating direction.

To connect an external power source, use the `PWRIN` input on the shield. The optimal choice would be a rechargeable battery pack: **7.4V, >1800mAh**, connected via suitable connectors, and maybe even equipped with a switch. You can also use this power source to supply power to the Arduino by soldering a Barrel Plug to the switch I/O. The Arduino can handle a 7.4V input (recommended range is 7 to 12 volts).

### Controller:
In the controller, we will use an Arduino Nano. The radio module will be connected in the same way as in the tank:
*   `VCC` -> `5V` - Power Supply pin 5V
*   `GND` -> `GND` - Ground
*   `CE` -> `D9` - Chip Enable pin
*   `CSN` -> `D10` - SPI Chip Select pin
*   `SCK` -> `D13` - SPI Clock pin
*   `MO` -> `D11` - Slave Data Input pin (MOSI)
*   `MI` -> `D12` - Slave Data Output pin (MISO)

Another part that we have to connect is the joystick. This module produces an output of around 2.5V from X and Y when it is in the resting position (reading ~512). Moving the joystick will cause the output to vary from 0V to 5V (reading 0 to 1023).

*   `GND` -> `GND` - ground
*   `+5V` -> `5V` - 5V DC
*   `VRx` -> `A4` - voltage proportional to x position
*   `VRy` -> `A3` - voltage proportional to y position
*   `SW` -> `A2` - switch pushbutton

For the power source, you can simply use a power bank and a USB cable.

<p align="center">
  <img width="700" alt="image" src="https://github.com/user-attachments/assets/7fca63c3-3e6e-447d-9563-776cc856be07" />
</p>

## 3. Construction

1.  To correctly mount the motor, place it in one of the designated holes in the 'Side' part with the projected shaft on the other side, then attach the 'Main cog' part to it.

<p align="center">
  <img width="470" height="183" alt="image" src="https://github.com/user-attachments/assets/30956a24-9301-456a-bfff-0cf86c328389" />
</p>

2.  Once done, you can mount the remaining cogs and the side bumper. If everything was printed correctly and cleaned, they should press fit.
3.  After connecting all the tracks (32 for each side), you can place them on the cogs. Don't hesitate to use a bit more force; it shouldn't break.

<p align="center">
  <img width="390" height="272" alt="image" src="https://github.com/user-attachments/assets/5a29e235-d636-4f27-960c-692c51a73ef9" />
</p>

4.  I recommend securing the 'Side' with the 'Side bumper' using rope or paper clips in the middle hole designed for this purpose.
5.  The final step is to guide the cables from the motors between the holes in the body and press-fit everything together. If the clips don't hold it securely enough, you can either try reprinting it or simply use some tape on the bottom.

<p align="center">
  <img width="392" height="197" alt="image" src="https://github.com/user-attachments/assets/494bb5ff-77e3-4c2a-aa54-50fcbcbc833b" />
</p>

## 4. Software description

The entire code is quite simple and is divided into two parts: "tank - receiver" for the vehicle and "controller - transmitter" for the remote.

### Tank receiver
*   **Libraries:**
    *   `SPI.h`, `RF24.h`, `nRF24L01.h`: Libraries for NRF24L01 wireless communication.
    *   `CytronMotorDriver.h`: Library for controlling CytronMD motors.
*   **Pins:**
    *   `CE_PIN`, `CSN_PIN`: Pins used to control the NRF24L01 module.
*   **Constants:**
    *   `INTERVAL_..._LOST`: Time interval to consider signal lost.
    *   `INTERVAL_..._RETRY`: Time interval for retrying the signal transmission.

### Controller transmitter
*   **Libraries:**
    *   `SPI.h`, `RF24.h`, `nRF24L01.h`: Libraries for NRF24L01 wireless communication.
*   **Pins:**
    *   `CE_PIN`, `CSN_PIN`: Pins used to control the NRF24L01 module.

## 5. Conclusions

In conclusion, while there are numerous possibilities for upgrading or extending the project, it currently stands as a perfect and straightforward starting point, serving as a robust foundation for future enhancements. Rest assured, I plan to incorporate these improvements in the near future. Stay tuned for updates!

<p align="center">
  <img width="500" alt="image" src="https://github.com/user-attachments/assets/eab27fe3-115a-420f-881d-3214ef43f4bd" />
</p>

---

## Bibliography
*   **NRF24L01+PA Datasheet:** [https://www.handsontec.com/dataspecs/module/NRF24L01+PA.pdf](https://www.handsontec.com/dataspecs/module/NRF24L01+PA.pdf)
*   **Cytron 2A Motor Driver Shield User Manual:** [https://www.technobotsonline.com/Datasheets3/1519-002-User.pdf](https://www.technobotsonline.com/Datasheets3/1519-002-User.pdf)
*   **Project Code Repository:** [https://github.com/Marcel3245/Tank-and-Controller](https://github.com/Marcel3245/Tank-and-Controller)
