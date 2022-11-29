# Creating a Device using Individual Enrollment for an IoT Central Application

1. Select the Devices menu, and click the "+ New" button
<img src="./media/CreateNewDeviceButton.png" alt="A screenshot of a new Device button" />

2. Use the device certificate common name for the Device ID
*OpenSSL* was used previously to read the certificate common name from the PEM file.  Use the common name for the *Device ID* field. This pairing is how the certificate is matched to the device during registration. The *Device name* can be changed to a friendly name, or left the same as the *Device ID*.

    <img src="./media/CreateNewDevice.png" alt="A screenshot of a new Device Dialog" width = 550/>

3. Select the device, and then click Connect to configure the device authentication method

    <img src="./media/SelectDevice.png" alt="A screenshot device selection" width = 700/>
    <img src="./media/ConnectButton.png" alt="A screenshot Connect Button" width = 600/>

4. Configure an X.509 Individual Enrollment
The Connect button will open the device connection dialog.  Several items are accomplished here.

    <img src="./media/DeviceConnectionDialog.png" alt="A screenshot Connect Button" width = 600/>

5. Select the *Authentication type* as "Individual enrollment"

6. Select the *Authentication method* as "Certificates (X.509)"

7. Select the file folder icon for the Primary certificate, upload your device PEM file (e.g. *`<"COMMON_NAME">.PEM`*).

8. Select the file folder icon for the Secondary certificate, upload your device PEM file (e.g. *`<"COMMON_NAME">.PEM`*). Both the primary and secondary certificate have to be selected to save the setting, which is why the same PEM file is uploaded twice.

9. Note the *`ID scope`* at the top of the dialog.  This is used when we configure the device to connect to Azure.

10. Click the blue *`Save`* button, then click the *`Close`* button