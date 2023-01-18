/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_multimeter.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

#include "config/default/peripheral/sercom/spi_master/plib_sercom1_spi_master.h"
#include "config/default/peripheral/gpio/plib_gpio.h"   
#include "string.h"
#include "definitions.h"
#include "config/default/peripheral/gpio/plib_gpio.h" 
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app_multimeter.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_MULTIMETER_Initialize function.

    Application strings and buffers are be defined outside this structure.
 */

APP_MULTIMETER_DATA app_multimeterData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
 */

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
 */


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_MULTIMETER_Initialize ( void )

  Remarks:
    See prototype in app_multimeter.h.
 */

void APP_MULTIMETER_Initialize(void) {
    /* Place the App state machine in its initial state. */
    app_multimeterData.state = APP_MULTIMETER_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP_MULTIMETER_Tasks ( void )

  Remarks:
    See prototype in app_multimeter.h.
 */
uint64_t getTick(void);
uint64_t time;
uint8_t txByte,rxByte,numberOfTime;
float fVoltage,voltage_cal;
float multimeterVolage;

float getVoltage( void) 
{
    return multimeterVolage;
}

#define MULTIMETER_I_CHANNEL   0x00
#define MULTIMETER_U_CHANNEL   0x01
#define MULTIMETER_R_CHANNEL   0x02
#define MULTIMETER_C_CHANNEL   0x03
void APP_MULTIMETER_Tasks(void) {
    static uint64_t delay;

    /* Check the application's current state. */
    switch (app_multimeterData.state) {
            /* Application's initial state. */
        case APP_MULTIMETER_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized) {

                numberOfTime = 5;
                app_multimeterData.state = APP_MULTIMETER_CONVERT_DUMMY;
            }

            break;
        }
        case APP_MULTIMETER_CONVERT_DUMMY:
        {
            uint16_t value;
            
                app_multimeterData.state = APP_MULTIMETER_CALIB_VOLTAGE;
            SPI_CS_Clear();
            txByte = 0x6;
            SERCOM1_SPI_Write(&txByte, 1);
            while (SERCOM1_SPI_IsBusy());
            
            txByte = ( MULTIMETER_U_CHANNEL & 0x03 ) << 6;
            SERCOM1_SPI_WriteRead(&txByte, 1, &rxByte, 1);
            while (SERCOM1_SPI_IsBusy());
            value = (uint16_t)(rxByte & 0x0f)<<8;
            
            SERCOM1_SPI_Read(&rxByte, 1);
            while (SERCOM1_SPI_IsBusy());
            value = value | rxByte;
            
            
            SPI_CS_Set();
            
            
            break;
            
        }
        case APP_MULTIMETER_CALIB_VOLTAGE:
        {
            uint16_t value;
            
            if ((getTick() < delay))
                break;
            SPI_CS_Clear();
            txByte = 0x6;
            SERCOM1_SPI_Write(&txByte, 1);
            while (SERCOM1_SPI_IsBusy());
            
            txByte = ( MULTIMETER_U_CHANNEL & 0x03 ) << 6;
            SERCOM1_SPI_WriteRead(&txByte, 1, &rxByte, 1);
            while (SERCOM1_SPI_IsBusy());
            value = (uint16_t)(rxByte & 0x0f)<<8;
            
            SERCOM1_SPI_Read(&rxByte, 1);
            while (SERCOM1_SPI_IsBusy());
            value = value | rxByte;
            voltage_cal = (float)value/2;
            
            SPI_CS_Set();
            printf("voltage_cal in hex= %x\r",value);
            printf("voltage_cal in float = %f\r",voltage_cal);
            app_multimeterData.state = APP_MULTIMETER_STATE_SERVICE_TASKS;
            
            
        }
        case APP_MULTIMETER_STATE_SERVICE_TASKS:
        {
            uint16_t value;

            time = getTick();
            if ((getTick() < delay))
                break;
            delay = getTick() + 1000;
//Blocking code during 3 bytes transfer !!!! Just for TEST :p            
            SPI_CS_Clear();
            txByte = 0x6;
            SERCOM1_SPI_Write(&txByte, 1);
            while (SERCOM1_SPI_IsBusy());
            
            txByte = ( MULTIMETER_U_CHANNEL & 0x03 ) << 6;
            SERCOM1_SPI_WriteRead(&txByte, 1, &rxByte, 1);
            while (SERCOM1_SPI_IsBusy());
            
            value = (uint16_t)(rxByte & 0x0f)<<8;
            SERCOM1_SPI_Read(&rxByte, 1);
            while (SERCOM1_SPI_IsBusy());
            
            value = value | rxByte;
            fVoltage = (float)value/2;
            SPI_CS_Set();

//            printf("value = %x\r",value);
//            printf("fVoltage = %f\r",fVoltage);
//            printf ("Voltage = %.1f mV\r", ( fVoltage - voltage_cal ) * 33 / 2);
                               
//            sprintf (multimeterVolage,"Multimeter Voltage is  = %.1f mV", ( fVoltage - voltage_cal ) * 33 / 2);
            multimeterVolage = ( fVoltage - voltage_cal ) * 33 / 2 ;
            break;
        }

            /* TODO: implement your application state machine.*/


            /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
