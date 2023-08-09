# README BinarX Emulator  

## Reason for the project 

Binar (BIN-ah) is the Noongar word for “fireball”. We are a space program, building the next generation of Australian small spacecraft, at Curtin University’s Space Science and Technology Centre. Binar will advance our understanding of the solar system and lower the barrier for operating in space.  

BinarX is a two-year program that will see students design and prototype science payloads for the Binar cubesat in 2022, and then build those projects for launch in 2023. Student payloads from three pilot schools will be part of Launch 3. Payload concepts will be directed by students and teachers, but might include sensors to study earth and space environment, samples for testing in microgravity, or samples for testing in a vacuum. They may also include a software-only payload running on Binar’s flight computer. 

Furthermore, Curtin University offers a spacecraft design and manufacturing units. 

The BinarX Emulator has been created to support the student's payload developments. The Emulator provides an interface just as they will encounter from the Binar CubeSat. Therefore, the payload can be integrated with an emulated Binar CubeSat for every group. The Emulator maximizes the learning outcomes by providing quick feedback to their payload development. Furthermore, once the payloads arrive at Binar for integration with the spacecraft, we can be confident that the payload will behave as expected.   

## Technical description 

The Emulator uses a Nucleo-H743ZIQ for electrical interfaces and computer resources. Through the SPI serial communication protocol, the students can transmit their data. In flight, Binar would transmit this data to the ground using radio. Instead, the emulator forwards the data received to the student's computer using UART communication protocol and USB cable.  

Nucleo-H743ZIQ was chosen to allow for the project to increase in complexity with minimal work rework.  

SPI was selected as all communication control is managed by the Emulator, and by the spacecraft during flight.  

Communicating the student's data straight to the computer was selected due its simplicity. This implementation allows for minimum development time and high portability between devices.  

For development and testing, the emulator library was built using Bazel and the Google Mock library. The library was then included in a STM32CubeIDE project for integration testing. A payload project was created to test the Emulator. This payload example project has also been privided in this repo.  

## How to download and run 

### Use the emulator 

This section is intended for users that want to develop their payload and need to get the emulator working in a NUCLEO-H743ZIQ microcontroller.  

1. Download the STM32CubeIDE program to your computer 
2. Download the BinarX Emulator project from the STM32CubeIDE project folder 
3. Import the BinarX Emulator project to STM32CubeIDE 
4. Build the project and check for errors 
5. If there are no errors, connect the NUCLEO-H743ZIQ to the computer using the USB cable. Make sure you connect the micro-USB connector to the correct port.  
6. Flash the code to the microcontroller by pressing  . If you get a message with no ST-Link detected, then the cable is probably not connected correctly.  
7. Once flashed, if all worked as intended, you should see this yellow LED turn on.  
8. Go to the How to use section 

### Use the Payload example 

This section quickly explains how to flash the payload example. The payload example was created for a NUCLEO-F303.  

1. If you have not done so already, download the STM32CubeIDE program to your computer 
2. Download the BinarX payload example project from the STM32CubeIDE project folder 
3. Import the BinarX payload example project to STM32CubeIDE 
4. Build the project and check for errors 
5. If there are no errors, connect a NUCLEO-F303 to the computer using the USB cable.  
6. Flash the code to the microcontroller by pressing  . If you get a message with no ST-Link detected, then the cable is probably not connected correctly.  
7. Once flashed, if all worked as intended, you should see the Yellow LED turn on. 
8. Go to the How to use section 

### Emulator for developers 

The Emulator libraries has been developed in C++ using the Bazel tool and unit tested with the Google Mock library. There are four main folders within the emulator_libraries folder. 
1. "emulator" is the library intended to mimic the comunicating with the binar flight computer. It also transmits any data received from the payload back to the computer. 
2. "emulator_liason" is the library used by the student's payload to comunicate apropiatelly with the emulator.
3. "abstraction_layer" the emulator and emulator_liason require hardware connections to operate correctly. The "abtraction_layer" library abstracts this connections to allow for the librtaries to be tested. 
4. "external_libraries" the students are intended to use JSON as the data formating structure. A JSON library has already been chosen to test the emulator and it has been provided to support the students. 

The emulator_libraries folder has been copied to the Stm32CubeIDE project for the IDE to find and build the software.  The STM32CubeIDE projects have been setup to have look for this library as a relative path whithin the STM32 project.  Therefore, if the code withing the emulator_libraries is updated, one should only have to replace the folder within the project directories. 

Whithin the STM32CubeIDE projects three things require attention.
1. The IOC file that configures the Nucleo board to have the appropiate SPI and GPIO connections. 
2. The main.c file was renamed to main.cpp as required to creat C++ project.
3. Then the main.cpp code was edited to add the required functionality. 

## How to use the project 
This sections assumes that you have downloaded and flashed, or uploaded, the code to the microcontrollers as explained above. The simple steps to connect and start using the emulator and payload example are;
1. Plug in the Emulator microcontroller to the computer using the USB cable. Warning, there are two micro usb ports on the microcontroller and only one will work correctly. The correct one has a label that sais "USB PWR". The incorrect one has the "User USB" label and it is next to the user blue button and the ethernet port.
    - The Emulator is running correctly if the Yellow LED turns on. 
2. Open the serial terminal of choice. To use the STM32CubeIDE serial terminal you can follow this link.  
    - https://community.st.com/t5/stm32-mcus/how-to-use-the-stm32cubeide-terminal-to-send-and-receive-data/ta-p/49434
    - At the "New serial Port Connection" window,
        - Chose any name you prefer. 
        - The Serial Port can change, most likely it will be /dev/ttyACM0. However, if the Emulator board is connected to the USB when you open the "New serial Port Connection" window, then the Serial Port should be configured to the correct Port automatically.
        - Baud rate should be 115200
        - Data size = 8, Parity = None, Stop bit = 1.
3. Power on the Payload microcontroller board using the power supply or USB cable
4. Wait a second or so for the microntroller to fully switch on
5. Press the Blue Button on the Emulator microcontroller board
6. If this steps have succeeded you should see information being printed to the serial terminal. 
    - The first message says "INFO: Button pressed and waiting for SPI transmission"
    - The Data sent from the payload microcontroller should be printed next
    - An error messages might be printed instead of the data. This error messages will be described bellow.
    - Finally the "INFO: Turning payload off" will be printed last. If this is printed without an error message then the Emulator timed out. 

### Emulator Error messages and possible causes
1. "ERROR: The number of packets that must be sent by the payload was not received correctly" 
    - 

As Binar X documentation 

Include credits 

Add license 

Include tests 

 

 