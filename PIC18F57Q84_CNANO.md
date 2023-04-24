# Using the PIC18F57Q84 Curiosity Nano Evaluation Kit as the Host MCU Board

## Setting Up the Actual Embedded Firmware Example

Now that we've successfully run Python scripts on a PC to emulate all of the necessary transactions of an IoT device, we can now run a "real" embedded firmware example that's programmed onto the [PIC18F57Q84 Curiosity Nano Evaluation Kit](https://www.microchip.com/en-us/development-tool/DM182030) as the Host MCU board.

1. Power cycle the AnyCloud™ serial bridge board and then hit the board's RESET (or MCLR) button
    - WFI32-IoT Development Board: `RESET`
    - PIC32 WFI32E Curiosity Board: `MCLR`
<img src=".//media/WFI32_RESET.png" width=400 />

2. Replace the USB-to-UART converter connection with the `PIC18F57Q84 Curiosity Nano Evaluation Kit` based on whether you're using the `WFI32-IoT` or `PIC32 WFI32E Curiosity` development board as the AnyCloud™ serial bridge (make sure to follow the correct diagram for your board)

    <img src=".//media/PIC18F57Q84CNANO_WFI32-IoT.png"/>

    <img src=".//media/PIC18F57Q84CNANO_WFI32E-Curiosity.png"/>

3. Connect the `PIC18F57Q84 Curiosity Nano Evaluation Kit` to the PC using the supplied micro-USB cable. Launch a [Terminal Emulator](https://en.wikipedia.org/wiki/List_of_terminal_emulators) program of your choice and connect to the PIC18F57Q84 Curiosity Nano Evaluation Kit's Virtual COM Port at 115200 baud.

    - Windows: The correct one to select will most likely be the one that shows up as a generic "USB Serial Port"

        <img src=".//media/image17a.png" width=400 />

    - MacOS: The correct one to select will most likely be the one that is shorter in length and does not contain the `RYN` characters

        <img src=".//media/image17b.png" width=200 />

4. Launch the `MPLAB X` IDE (this tool should have been previously installed and most likely resides in the \Program Files\Microchip\ folder. Once the MPLAB X IDE has finished its initialization routines, you should notice the "Kit Window" that acknowledges an active connection to the PIC18F57Q84 Curiosity Nano Evaluation Kit

    <img src=".//media/image18c.png"  />

5. Navigate to the main toolbar's `File` > `Open Project` operation to load the demo project folder (\*.X) located at `\AzureDemo_AnyCloud\examples\PIC18F57Q84_AnyCloud\firmware\PIC18F57Q84_AnyCloud.X`

    <img src=".//media/image19a.png" width=200 />
    <img src=".//media/image19c.png" width=400 />

   If the `Configuration load error` message in red appears in the `Output` window, click on the `Resolve DFP for configuration: default` link

    <img src=".//media/image21a.png" width=500 />

6. Set the `PIC18F57Q84_AnyCloud` project as the main (currently focused/active) project by right-clicking on it and selecting `Set as Main Project`

    <img src=".//media/image40.png" width=300 />

7. In the `Projects` window, open the `app_rio2_config.h` header file by double-clicking directly on the file name

    <img src=".//media/image23.png" width=200 />

8. In the `app_rio2_config.h` header file, set the necessary parameters corresponding to your IoT device

    - `WIFI_SSID` (name of your Wi-Fi Access Point)
    - `WIFI_PSWD` (password for your Wi-Fi Access Point)
    - `MY_THING_ID` (confirm this matches the Common Name in the client certificate)
    - `ID_SCOPE` (confirm it is mapped to the correct IoT Central application)
    - `MODEL_ID` (confirm Device Twin Model Identifier (DTMI) is correct)

9. Verify the project properties are set correctly before building the project by executing the following steps:

    - right-click on the `PIC18F57Q84_AnyCloud` project
    - select `Properties`
    - under `Connected Hardware Tool`, select `PIC18F57Q84 Curiosity Nano-SN`
    - select the latest version for `CMSIS`
    - select the latest version for `PIC18F-Q_DFP`
    - select the latest XC8 version for `Compiler Toolchain`

        <img src=".//media/image43.png" width=550 />

        **Note** If any changes were made in the project properties window, the `Apply` button should become enabled.  Make sure to hit the `Apply` button before hitting `OK`

10. Right-click on the active project and select `Clean`. Right-click the project again and select `Make and Program Device`. This operation will automatically build the project before attempting to program the target device.

11. After the `BUILD SUCCESSFUL` message appears in the Output window, the application HEX file will be programmed onto the development Board. Once programming has finished, the board will automatically reset and start running its application code.

12. Observe the debug messages in the terminal window and verify that telemetry and properties are being updated in the IoT Central application.

13. To restart the entire connection sequence, peform the following steps in order:

    - Power cycle the AnyCloud™ serial bridge board and then press the RESET (or MCLR) button
        - WFI32-IoT Development Board: `RESET`
        - PIC32 WFI32E Curiosity Board: `MCLR`

            <img src=".//media/WFI32_RESET.png" width=400 />
    - Power cycle the PIC18F57Q84 Curiosity Nano Evaluation Kit (or type `CTRL-C` in the terminal window)

14. Access your IoT Central application by signing into the [IoT Central Portal](https://apps.azureiotcentral.com), clicking on `My Apps` in the left-hand side navigation pane, and then clicking on the tile that is labeled with the name of your application.

15. Confirm that telemetry messages are being received and that all other IoT Central functions are working like before when the Python script was running on the PC. The PIC18F57Q84 Curiosity Nano Evaluation Kit should be interacting with IoT Central just like the main Python script was doing earlier.

16. Using the left-hand navigation pane, click on `Devices` under **Connect**, and then click on your device name

    <img src=".//media/image90a.png" width=800 />

17. Click [here](./DeviceTemplate_CreatingViews.md) to create an additional "Properties" view that allows you to change any of the Cloud-writable properties of the device in the IoT Central application.

18. Click on the **Properties** view. Select "Off" for the property "User LED" and click on the **Save** icon. Observe that `LED0` on the PIC18F57Q84 Curiosity Nano Evaluation Kit is actually off.

    <img src=".//media/image91a.png" width=350 />

19. Try changing the telemetry interval to see if you can speed up (or slow down) the rate of the telemetry messages being sent from the device. Don't forget to hit the `Save` icon each time after changing the value.

20. Click on the **Commands** view. Type any text message in the "String to send" box and click on the **Run** button. To see the echo'ed message response from the device, click on the **command history** link (located just underneath the **Run** button). You should see that the response was received "now" (i.e. within the last minute) and that the correct message was echoed from the device.

    <img src=".//media/image92a.png" width=299 />
    <img src=".//media/image93a.png" width=250 />

NOTE: You can access any of your IoT Central applications in the future by accessing the [IoT Central Portal](https://apps.azureiotcentral.com).

At this point, you have a working embedded firmware project to use as a starting point for a new proof-of-concept and/or IoT device design!