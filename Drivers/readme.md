# README BinarX Drivers

## Current List of Drivers
1. FGD-02F (Dosimeter) (SPI)
2. ICM42670P (6-AXIS IMU // Gyro + Accelerometer) (I2C)
3. MMC5983MA (3-AXIS Magnetometer) (I2C)

## How to install External Driver Code Into Your STM32CubeIDE Project

![Alt text](figures/download_drivers.png)

1. Unzip the downloaded Driver code

![Alt text](figures/extract_driver.png)

1. Drag the unzipped folded into the project directory

![Alt text](figures/copy_extracted_file.png)

![Alt text](figures/copy_extracted_file_menu.png)

2. Right click on your project file

3. Select properties from the dropdown menu

![Alt text](figures/project_properties.png)

4. Open the dropdown for "C/C++ General" and select "Paths and Symbols"

![Alt text](figures/properties_paths_n_symbols.png)

5. Under the "Includes" tab

![Alt text](figures/properties_paths_n_symbols_includes.png)

6. select the "Add..." button on the right

![Alt text](figures/properties_paths_n_symbols_add.png)

7. From the popup select "Workspace..." then find the added folder "example_driver_code"

![Alt text](figures/add_directory_path_workspace.png)

![Alt text](figures/folder_selection.png)

8.  Select the driver folder in the list and move it up to under "peripheral_library"

![Alt text](figures/properties_paths_n_symbols_move_up.png)

9.  Move to the "Source Location" tab and repeat step 7

10. Select the drive code folder from the workspace and click "OK"

![Alt text](figures/folder_selection_source.png)

11. Select "Apply and close"

![Alt text](figures/properties_paths_n_symbols_source_apply_and_close.png)

12. Head to your main.c file and add a "#include" statement like the one below
    
    ``` #include "example_drive_code.h" ```

![Alt text](figures/include_main.png)

13. Build the project

![Alt text](figures/build.png)

You have now successfully installed the driver code.

Follow the readme.md under the specific driver folder for further information on use.
