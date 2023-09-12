# README BinarX Emulator  

## Reason for the project 

Binar (BIN-ah) is the Noongar word for “fireball”. We are a space program, building the next generation of Australian small spacecraft, at Curtin University’s Space Science and Technology Centre. Binar will advance our understanding of the solar system and lower the barrier for operating in space.  

BinarX is a two-year program that will see students design and prototype science payloads for the Binar CubeSat in 2022, and then build those projects for launch in 2023. Student payloads from three pilot schools will be part of Launch 3. Payload concepts will be directed by students and teachers, but might include sensors to study earth and space environment, samples for testing in microgravity, or samples for testing in a vacuum. They may also include a software-only payload running on Binar’s flight computer. 

Furthermore, Curtin University offers a spacecraft design and manufacturing units. 

The BinarX Emulator has been created to support the student's payload developments. The Emulator provides an interface just as they will encounter from the Binar CubeSat. Therefore, the payload can be integrated with an emulated Binar CubeSat for every group. The Emulator maximizes the learning outcomes by providing quick feedback to their payload development. Furthermore, once the payloads arrive at Binar for integration with the spacecraft, we can be confident that the payload will behave as expected.   

## Technical description 

The Emulator uses a Nucleo-H743ZIQ for electrical interfaces and computer resources. Through the SPI serial communication protocol, the students can transmit their data. In flight, Binar would transmit this data to the ground using radio. Instead, the emulator forwards the data received to the student's computer using UART communication protocol and USB cable.  

Nucleo-H743ZIQ was chosen to allow for the project to increase in complexity with minimal rework.  

SPI was selected as all communication control is managed by the Emulator, and by the spacecraft during flight.  

Communicating the student's data straight to the computer through serial was selected due to its simplicity. This implementation allows for minimum development time and high portability between devices.  

For development and testing, the emulator library was built using Bazel and the Google Mock library. The library was then included in a STM32CubeIDE project for integration testing with the Nucleo microcontroller. A payload project was also created to test the Emulator. Extra effort has gone into making this payload project a starting point for the BinarX students. This payload example project has also been provided in this repo.  

## How to download and run the STM32CubeIDE projects

### Common steps for both both Emulator and Payload projects
1. Download the STM32CubeIDE program to your computer from the st.com website.
    - Link: https://www.st.com/en/development-tools/stm32cubeide.html
2. Download the BinarX Emulator project from the STM32CubeIDE project folder. The project can be found on the Binar GitHub page.
    - GitHub Link: https://github.com/BinarX-Curtin/spacecraft-software-emulator 
    - Download Tutorial: https://docs.github.com/en/repositories/working-with-files/using-files/downloading-source-code-archives
3. Import the BinarX Emulator project to STM32CubeIDE. (Needs testing as it might only work for Linux)
    - Go to the Information Centre page within STM32CubeIDE. The first time you open STM32CubeIDE it should open up with the information center. Otherwise, click on the blue button with an "i" in it. 
    - From the information center, click on "import project"
    - Click "Directory" and navigate and then open the STM32CubeIDE projects folder
    - Select all and press finish. 
4. Build the project and check for errors 
    - In STM32CubeIDE press the hammer button on the top left corner
    - Check for errors within the "Console" Window in STM32CubeIDE

### Flash the emulator 

This section is intended for users that want to develop their payload and need to get the emulator working in a NUCLEO-H743ZIQ microcontroller.  
1. Complete the common steps above to flash the binarx_emulato-h743zi project
2. If there are no errors, connect the NUCLEO-H743ZIQ to the computer using the USB cable. 
    - Make sure you connect the micro-USB connector to the correct port as there are two.   
    - The correct one is on the ST-Link side and has a "USB PWR" label. The incorrect one has the "User USB" label and it is next to the user blue button and the ethernet port.
3. Flash the code to the microcontroller by pressing the green play button. If you get a message with no ST-Link detected, then the cable is probably not connected correctly.  
4. Once flashed, if all worked as intended, you should see the yellow LED turn on.  
    - The yellow LED is pretty much in the centre of the board but slightly ofset towards the ST-Link section of the board. 
    - There are more LEDs near the edges of the board that are independent of the emulator
5. Go to the "How to use section" to continue from here

### Flash the Payload example 

This section quickly explains how to flash the payload example. The payload example was created for a Binar X Developement Kit.  
1. Complete the common steps above to flash the XXXXXXX project
2. If there are no errors, connect the ST-Link to the Payload board and to the computer.
3. Flash the code to the microcontroller by pressing the green play button. If you get a message with no ST-Link detected, then the cable is probably not connected correctly.  
4. Go to the "How to use section" to continue from here

## How to use the project 
This sections assumes that you have downloaded and flashed (in other words uploaded) the code to the microcontrollers as explained above. The simple steps to connect and start using the emulator and payload example are;
1. Plug in the Emulator microcontroller to the computer using the USB cable. Warning, there are two micro usb ports on the microcontroller and only one will work correctly. The correct one has a label that says "USB PWR". The incorrect one has the "User USB" label and it is next to the user blue button and the ethernet port.
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
    - The probable cause is that the payload_ready pin is not connected correctly. 
2. "ERROR: The number of packets is too large and there will not be enough space to store the information"
    - The payload data is too large for the emulator limits
    - or you have incorrectly requested to transmit more data that you had intended
3. "ERROR: Sorry the message was not received correctly by the Binar Emulator"
    - The size of the payload data was received correctly by the emulator but an error occurred during the transmission of the data. If it continues to happen the payload or emulator are not working as intended. Restart both devices and try again.
4. "ERROR: Sorry an error occurred with the Binar Emulator"
    - This error is unlikely. If it continues to happen the payload or emulator are not working as intended. Restart both devices and try again.
5. If the Emulator has timeout
    - Then the payload ready pin did not trigger an interrupt on the emulator

## Emulator for developers 

### Emulator Libraries
The Emulator libraries has been developed in C++ using the Bazel tool and unit tested with the Google Mock library. There are four main folders within the emulator_libraries folder. 
1. "emulator" is the library intended to mimic the comunicating with the binar flight computer. It also transmits any data received from the payload back to the computer. 
2. "emulator_liason" is the library used by the student's payload to comunicate apropiatelly with the emulator.
3. "abstraction_layer" the emulator and emulator_liason require hardware connections to operate correctly. The "abtraction_layer" library abstracts this connections to allow for the librtaries to be tested. 
4. "external_libraries" the students are intended to use JSON as the data formating structure. A JSON library has already been chosen to test the emulator and it has been provided to support the students. 

#### Running the Tests
From the emulator_libraries folder you can run "bazel test <target_test>"
- For emulator_liason: 
    "bazel test --cxxopt=-std=c++17 --test_output=all //emulator_liason/test:emulator_liason_test"
- For emulator: 
    "bazel test --cxxopt=-std=c++17 --test_output=all //emulator/test:emulator_test"

### Stm32CubeIDE projects
The emulator_libraries folder has been copied to the Stm32CubeIDE project for the IDE to find and build the software.  The STM32CubeIDE projects have been setup to look for this library as a relative path whithin the STM32 project.  Therefore, if the code withing the emulator_libraries is updated, one should only have to replace the folder within the project directories. 

Whithin the STM32CubeIDE projects three things require attention.
1. The IOC file that configures the Nucleo board to have the appropiate SPI and GPIO connections. 
2. The main.c file was renamed to main.cpp as required to creat a C++ project. Annoingly, if you change the IOC file a new main.c is created. It is usually very similar to the previous main.cpp you were working on, but changes are almost certain.
3. Then the main.cpp code was edited to add the required functionality.

#### Creating a new STM32CubeIDE project
Follow this brief steps to create a STM32CubeIDE project for a new microcontroller instead of the one listed above. 
1. Download the STM32CubeIDE program to your computer from the st.com website.
    - Link: https://www.st.com/en/development-tools/stm32cubeide.html
2. Create a new STM32 project with your microcontroller. 
    - Warning, when given the option, make sure you choose C++ as the target language. 
    - Follow the steps in this link: https://wiki.st.com/stm32mpu/wiki/How_to_get_started_with_STM32CubeIDE_from_scratch#:~:text=To%20start%20a%20new%20project,%5D%3E%5BSTM32%20Project%5D.&text=Select%20the%20desired%20STM32%20device,Click%20on%20%5BNext%20%3E%5D.
3. Configure the microcontroller peripherals by modifiying the IOC file.
    - Whithin the project you have just created, there should be a file with the same name as your project that ends with ".ioc". Click on this "Project_name.ioc" file. The IOC file is were we configure the microcontroller.
    -  After clicking the IOC file a new window appears, on the left hand side we have a list. 
        1. Click on "Connectivity" then "SPI1" and choose the following configuration.
            - Mode: Full-Duplex Slave
            - Hardware NSS Signal: Disable
            - Frame format: Motorola
            - Date Size: 8 bits
            - Firts bit: MSB First
            - Clock polarity: Low
            - Clock Phase: 1 Edge
            - CRC calculation: Disabled
            - NSS Signal Type: Software
        2. The SPI pins should now appear on the pinout view on the diagram to the right. 
        3. From the Pinout view diagram on the right, we will modify two pins to configure the "Payload_Chip_Select" and "Data_Ready" pin. This pins correlate to a physical location of the board. Therefore, you may need to check your hardware datasheet to connect the payload to the emulator correctly. 
            1. Click one of GPIO pins that is not being used (i.e. one of the gpio pins that is grey). A list should appear.
            2. Choose "GPIO_Output" for this pin
            3. Click on another GPIO pin that is not being used (i.e. one of the gpio pins that is grey). A list should appear.
            4. Click on "GPIO_EXTI#". The # character will be a number and this number depends on the pin you have clicked on. 
        4. Now click on "System Core" then "GPIO" on the list to the left.
            1. You should see a list with two options that match the two pins you have just selected
            2. For the GPIO_Output pin we will name it "Data_Ready" within the "User Label" box. The GPIO_Output pin is the one that reads "Output Push Pull" whitin the "GPIO Mode" box. 
            3. For the GPIO_EXTI pin we will name it "Payload_Chip_Select" within the "User Label" box. The GPIO_EXTI pin is the one that reads "External interrupt Mode with Rising edge trigger selection" whitin the "GPIO Mode" box. Waring, if this GPIO_EXTI reads "External interrupt Mode with" but the last bit is diferent, change this to match by clicking the arrow to the right of the box.
        5. To finalise the GPIO configuration, click on "System Core" then "NVIC" on the list to the left. We need to enable the EXTI line for the pin you have selected. 
            1. Find "EXTI line # to # interrupts" where # are numbers from low to high.
            2. If your GPIO_EXTI# selected previously is within this two numbers, enable this EXTI and choose priority 2. 
        6. While we are in this NVIC list, there should be an "SPI1 global interrupt" option.
            - also enable it and set the priority to 2 for the SPI. 
4. Download the BinarX Emulator project from the STM32CubeIDE project folder. The project can be found on the Binar GitHub page.
    - GitHub Link: https://github.com/BinarX-Curtin/spacecraft-software-emulator 
    - Download Tutorial: https://docs.github.com/en/repositories/working-with-files/using-files/downloading-source-code-archives
5. Copy the "emulator_libraries" folder into the STM32CubeIDE project.
    - It should be in the same folder as the "Core" and "Drivers" folders and as the ".ioc" file. 
6. Setup the include path and source on your STM32CubeIDE project
    1. Click on the "Project" tab at the top of the STM32CubeIDE window
    2. Then "C/C++ Build", "Settings"
    3. Under "MCU G++ Compiler" click on "Include paths"
    4. For the "Include paths" subsection you will need to include the "emulator_libraries" folder
        - Click on the logo with the green plus sign, then workspace, whithin your project, add emulator libraries. 
    5. For the "Include files" subsection you will need to include all of the source files.
        - "emulator_libraries/emulator_liason/src/emulator_liason.cc"
        - "emulator_libraries/abstraction_layer/payload_impl/serial_impl.cc"
        - "emulator_libraries/abstraction_layer/gpio/src/emulator_liason/emulator_liason_gpo.cc"
7. Build the project and check for errors 
    - In STM32CubeIDE press the hammer button on the top left corner
    - Check for errors within the "Console" Window in STM32CubeIDE
8. If there are no error, you can now follow the "Flash the Payload example" section but for your new project and microcontroller.





 

 