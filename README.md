# Connecting to Azure IoT Central using Microchip's UART-to-AnyCloud™ Solution

## Introduction

[AnyCloud™](https://github.com/MicrochipTech/PIC32MZW1_AnyCloud) is a Cloud connectivity solution package for Microchip's [WFI32E01PC](https://www.microchip.com/en-us/product/WFI32E01PC) IoT module that runs on the [PIC32 WFI32E Curiosity Board](https://www.microchip.com/en-us/development-tool/EV12F11A) or the [WFI32-IoT Development Board](https://www.microchip.com/en-us/development-tool/ev36w50a). The [AnyCloud™](https://github.com/MicrochipTech/PIC32MZW1_AnyCloud) solution includes a full set of firmware to enable custom modifications and the default binary image that can be used as well. The solution is publicly available on [Microchip Technology's GitHub account](https://github.com/MicrochipTech). To review the software, clone the repository, or simply download a ZIP file, access the [AnyCloud™](https://github.com/MicrochipTech/PIC32MZW1_AnyCloud) repository on [GitHub](https://github.com).

The [WFI32E01PC](https://www.microchip.com/en-us/product/WFI32E01PC) module (which has been provisioned with the [AnyCloud™](https://github.com/MicrochipTech/PIC32MZW1_AnyCloud) firmware) is meant to act as a "UART to Cloud" bridge to enable the Host MCU of an IoT device to easily connect to (and communicate with) a cloud application. In this example, a PC runs various Python scripts to emulate the operations that a Host MCU would need to execute in order to authenticate, connect, and communicate with a Microsoft Azure IoT Central application.

<img src="./media/SolutionBlockDiagram.png" alt="A screenshot of a new Device button" width = 700/>

## Software Prerequisites / Tools Installation

* [Git](https://git-scm.com/)
* [Python 3.0](https://www.python.org/download/releases/3.0/)
* [Python Serial Port Extension](https://pypi.org/project/pyserial/)
* [OpenSSL](https://www.openssl.org)

## Getting Started

### Step 1 - Install the AnyCloud™ Firmware onto the Development Board

Follow all of the existing instructions found in the [AnyCloud™ Getting Started Guide](https://github.com/MicrochipTech/PIC32MZW1_AnyCloud/blob/main/README.md) to get the hardware platform set up and verified. During the setup process, you will discover the Virtual COM port number that is associated with your board's USB-to-UART serial connection with your PC. For example, with the help of the the Windows Device Manager, under the category `Ports (COM & LPT)`, the Virtual COM port may show up as a "USB Serial Device" as illustrated here:

<img src="./media/WindowsDeviceManager.png" alt="A screenshot of a new Device button" width = 300/>

#### 1.1 Using the text editor of your choice, open the `AzureAnyCloud.py` script and locate the following line towards the top of the file:

```bash
COM_PORT = "your_COM_Port"
```
For example, if the USB Serial Device is associated with `COM4`, then the line would need to be changed to look like the following:

```bash
COM_PORT = "COM4"
```

Edit this line to reflect the Virtual COM port associated with your board's USB serial connection and save your changes to the script.

#### 1.2 Open the `WFI32_DeviceCert.py` script and repeat the same process for setting the COM port.

#### 1.3 Open the `WFI32_RootCert.py` script and repeat the same process for setting the COM port.

<br>

**Note** After the MPLAB X IPE has completed the programming of the FW image (HEX file), the IPE holds the WFI32 module in reset, so the board should be disconnected from the USB cable and then reconnected in order for the [AnyCloud™](https://github.com/MicrochipTech/PIC32MZW1_AnyCloud) firmware to run after it has been programmed.

Recommendations:

Clone the repository to your local machine, even if you are not planning to rebuild the project initially.  This provides a local copy of the README file, you can review project source code to self-support, and it includes the pre-built HEX file that can be programmed without rebuilding if that is your wish. 

    git clone https://github.com/MicrochipTech/PIC32MZW1_AnyCloud

**Note** To execute AT commands from a terminal to learn the [AnyCloud™](https://github.com/MicrochipTech/PIC32MZW1_AnyCloud) software, make sure the terminal application has the capability to append `\r\n` (Carriage Return + Line Feed) to the commands you are executing.  The AT commands are not executed without the `\r\n` terminating characters.  

### Step 2 - Read the Root and Device Certificates from the Module

The device certificate file will be needed when we create the device in Azure IoT Central using the individual enrollment method. Another option is to use the group enrollment method which requires uploading the root certificate file to the Azure IoT Central application, so that any device which presents a leaf certificate that was derived from the root certificate will automatically be granted access to registration.

#### 2.1 The **Device** certificate can be read out of the WFI32 module by executing the `WFI32_DeviceCert.py` script. The certificate file will be named based on the device's Common Name (i.e. `<"COMMON_NAME">.PEM`). Execute the following command in a PowerShell or Command Prompt window:

    python3 WFI32_DeviceCert.py

#### 2.2 Use OpenSSL to verify that the Common Name used in the device certificate matches the name of the PEM file which was auto-generated by the script. The following command will list certificate details in an easy to read format:
    
    openssl x509 -in <"COMMON_NAME">.PEM -text

The output of the command will show all fields, but the common name is what is required to register a device into an IoT Central application.  This common name (a.k.a. device ID) is shown in the Subject's *CN* field as illustrated below. In this example, the Subject's CN = sn0123FE0CF960432D01:

    Certificate:
    Data:
        Version: 3 (0x2)
        Serial Number:
            58:e6:60:93:53:e9:7b:45:79:25:15:c6:7e:cf:49:41
        Signature Algorithm: ecdsa-with-SHA256
        Issuer: O = Microchip Technology Inc, CN = Crypto Authentication Signer 2C60
        Validity
            Not Before: Nov 11 07:00:00 2020 GMT
            Not After : Nov 11 07:00:00 2048 GMT
        Subject: O = Microchip Technology Inc, CN = sn0123FE0CF960432D01
        Subject Public Key Info:
            Public Key Algorithm: id-ecPublicKey
                Public-Key: (256 bit)
                pub:
                    04:58:ad:8a:f1:d9:9d:c7:7f:4d:92:3d:0d:e8:49:
                    8b:4c:e8:ea:60:81:ce:e0:0e:a6:a7:68:3f:e0:de:
                    ....

#### 2.3 The **Root** certificate can be read out of the WFI32 module by executing the `WFI32_RootCert.py` script. The certificate file will be named `RootCA.PEM`. Execute the following command in a PowerShell or Command Prompt window:

    python3 WFI32_RootCert.py

### Step 3 - Create an Azure IoT Central Application

If you already have an existing IoT Central Application created, skip to Step 4.

#### 3.1 Create an Azure Account and Subscription

Microsoft has excellent instructions to create an new Azure account and subscription.  Please create an account and subscription before continuing. Review our overview of the [Azure Account/Subscription creation process](./CreateAzureAccountAndSubscription.md) as required.

#### 3.2 Create an Azure IoT Central Application

Refer to the linked instructions to [create an Azure IoT Central Application](CreateAnAzureIoTCentralApplication.md).

### Step 4 - Enroll Your Device in the Azure IoT Central Application

Choose either the [Group](./IoT_Central_Group_Enrollment.md) or [Individual](./IoT_Central_Individual_Enrollment.md) enrollment method to register your device with the IoT Central application. [Group enrollment](https://learn.microsoft.com/en-us/azure/iot-dps/concepts-service#enrollment-group) allows you to create a group of allowable devices which each have a leaf certificate derived from a common root certificate so that devices do not need to be pre-enrolled on an individual basis. Enrollment groups are used to enroll multiple related devices; [Individual enrollment](https://learn.microsoft.com/en-us/azure/iot-dps/concepts-service#individual-enrollment) is used to enroll a single device. Feel free to review both methods and be sure to complete your preferred method before proceeding with the next step.

### Step 5 - Configuring the AzureAnyCloud Script

1. Open the `AzureAnyCloud.py` script in a text editor of your choice

    <img src="./media/ScriptConfiguration.png" alt="Script Configuration" width = 400/>

2. Enter your WiFi network's SSID and passphrase as the *WiFi Credentials*
3. Enter your ID scope and Device ID (Common Name) into the *Azure Application/Device Information* settings.
4. Enter the model ID of the device template you wish to interact with in IoT Central.  Example, we can emulate the SAM-IoT Demonstration board from the script using *`dtmi:com:Microchip:SAM_IoT_WM;2`* as the model ID. 

The model ID will be declared during the DPS registration process.  If the model is published in the [Azure Device Model Repository](https://devicemodels.azure.com), IoT Central will automatically download the device model and use it to interact with your device based on the model's characteristics.  You can also create a custom device template in your IoT Central application, which will generate a new model ID that can declared and used with the [AnyCloud™](https://github.com/MicrochipTech/PIC32MZW1_AnyCloud) repository on [GitHub](https://github.com) as well.

### Step 6 - Run the AzureAnyCloud Script

To run the Azure IoT Central script type the following command line:

    python3 AzureAnyCloud.py

The script will check if you are connected to a WiFi network. If you are not connected, it will issue commands to connect with the SSID and passphrase provided.

    --------------------------------------------------------------------------------
    Starting the AnyCloud Azure IoT Central Demonstration
    --------------------------------------------------------------------------------
    Once device provisioning is complete, enter AT commands to continue.
    Press ESC to Exit
    Start Initialization...
    .............................
    ATE1
    OK
    >
    AT+WSTA
    +WSTA:0
    OK
    >
    Event: WiFi not connected, initialializing
    AT+WSTAC=1,"your_SSID"
    OK
    >
    AT+WSTAC=2,3
    OK
    >
    AT+WSTAC=3,"your_PASSPHRASE"
    OK
    >
    AT+WSTAC=4,255
    OK
    >
    AT+WSTAC=12,"pool.ntp.org"
    OK
    >
    AT+WSTAC=13,1
    OK
    >
    AT+WSTA=1
    OK
    >
    +WSTALU:"5C:76:95:1B:D9:2F",6
    >
    +WSTAAIP:"10.0.0.156"
    >
    Event: WiFi connected

It will then check if you are already connected to an MQTT broker.  If not, it will issue the commands to connect to the Azure DPS server. The DPS server uses a common host address to handle all requests. The unique device ID entered into the top of the script is used for the MQTT Client ID (MQTTC=3), and it is also part of the user name parameter (MQTTC=4). The ID Scope identifies your application which is also part of the username field.

    AT+MQTTCONN
    +MQTTCONN:0
    OK
    >
    Broker connection not found, connecting...
    AT+MQTTC=1,"global.azure-devices-provisioning.net
    OK
    >
    AT+MQTTC=2,8883
    OK
    >
    AT+MQTTC=3,"your_DEVICE_ID"
    OK
    >
    AT+MQTTC=4,"your_ID_SCOPE/registrations/your_DEVICE_ID/api-version=2019-03-31"
    OK
    >
    AT+MQTTC=7,1
    OK
    >
    AT+MQTTCONN=1
    OK
    >
    +MQTTCONNACK:0,0
    >
    +MQTTCONN:1
    >
    Event: MQTT broker connected

Finally, the script subscribes to the DPS MQTT notification topic, and publishes to a topic that registers the device.  The initial publish to the registration topic includes the model ID as the payload.  The result of this publication will be a a JSON message with an operationID field, and the status "assigning".  The code then delays, and issues a polling request to a second topic to determine if the registration is complete.

    Start DPS registration...
    
    subscribe to topics
    AT+MQTTSUB="$dps/registrations/res/#",0
    OK
    >
    +MQTTSUB:0
    >
    
    Event: Subscribed to DPS topics, publish registration request....
    
    AT+MQTTPUB=0,0,0,"$dps/registrations/PUT/iotdps-register/?rid=1","{\"payload\" : {\"modelId\" : \"dtmi:com:Microchip:SAM_IoT_WM;2\"}}"
    OK
    >
    +MQTTPUB:47,"$dps/registrations/res/202/?$rid=&retry-after=3",94,"{"operationId":"4.65f62b2644c85bb1.7e3cc6bc-896f-4c01-9007-75e32b6cebe8","status":"assigning"}"
    >
    Event: DPS subscription recieved notification
    topic: "$dps/registrations/res/202/?$rid=&retry-after=3"
    {
        "operationId": "4.65f62b2644c85bb1.7e3cc6bc-896f-4c01-9007-75e32b6cebe8",
        "status": "assigning"
    }
    AT+MQTTPUB=0,0,0,"$dps/registrations/GET/iotdps-get-operationstatus/?rid=2&operationId=4.65f62b2644c85bb1.7e3cc6bc-896f-4c01-9007-75e32b6cebe8",""
    OK
    >

**Note:** [AnyCloud™](https://github.com/MicrochipTech/PIC32MZW1_AnyCloud) currently has issues receiving large MQTT responses (greater than 512 bytes) from the last publication shown above.  Project to be continued....




