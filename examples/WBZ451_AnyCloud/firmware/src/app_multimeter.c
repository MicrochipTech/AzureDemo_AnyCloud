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

float MULTIMETER_getVoltage(void) 
{
    return (app_multimeterData.voltage);
}

float MULTIMETER_getCurrent(void) 
{
    return (app_multimeterData.current);
}

float MULTIMETER_getCapacitance(void) 
{
    return (app_multimeterData.capacitance);
}

uint16_t MULTIMETER_readChannel(uint8_t channel)
{
    uint8_t rxByte, txByte = 0x06;
    uint16_t value;
    
    SPI_CS_Clear();
    SERCOM1_SPI_Write(&txByte, 1);
    while (SERCOM1_SPI_IsBusy());
    txByte = ((channel & 0x03) << 6);
    SERCOM1_SPI_WriteRead(&txByte, 1, &rxByte, 1);
    while (SERCOM1_SPI_IsBusy());
    value = ((uint16_t)(rxByte & 0x0F) << 8);
    SERCOM1_SPI_Read(&rxByte, 1);
    while (SERCOM1_SPI_IsBusy());
    value |= rxByte;
    SPI_CS_Set();
    return (value);
}

float MULTIMETER_readCapacitance (void)
{
    float value;

    value = ( float ) MULTIMETER_readChannel(MULTIMETER_C_CHANNEL) * 2;
    if (value == 0)
    {
        return 0;
    }
    value = (  64285 / value - app_multimeterData.capacitance_cal ) * 2;

    if ( value < 1 )
    {
        return 0;
    }

    return value;
}

float MULTIMETER_readVoltage (void)
{
    float value;

    value = ( float ) MULTIMETER_readChannel(MULTIMETER_U_CHANNEL) / 2;
    value = ( value - app_multimeterData.voltage_cal ) * 33 / 2;

    return value;
}

float MULTIMETER_readCurrent (void)
{
    float value;

    value = ( float ) MULTIMETER_readChannel(MULTIMETER_I_CHANNEL) / 2;
    value -= app_multimeterData.current_cal;

    return value;
}

void APP_MULTIMETER_Tasks(void)
{
    static uint64_t delay;

    /* Check the application's current state. */
    switch (app_multimeterData.state) {
            /* Application's initial state. */
        case APP_MULTIMETER_STATE_INIT:
        {
            bool appInitialized = true;

            if (appInitialized) {

                app_multimeterData.state = APP_MULTIMETER_CONVERT_DUMMY;
            }
            break;
        }
        case APP_MULTIMETER_CONVERT_DUMMY:
        {
            uint16_t data;
            
            data = MULTIMETER_readChannel(MULTIMETER_U_CHANNEL);
            app_multimeterData.state = APP_MULTIMETER_CALIBRATE_VOLTAGE;
            break;
        }
        case APP_MULTIMETER_CALIBRATE_VOLTAGE:
        {
            uint16_t data;
            
            if ((getTick() < delay))
                break;

            data = MULTIMETER_readChannel(MULTIMETER_U_CHANNEL);
            app_multimeterData.voltage_cal = (float)data/2;
            
            printf("*** Make sure the +5V jumper is set on the mikroBUS Xplained Pro board ***\r\n");
            printf("*** (With no voltage across the Multimeter click's voltage measurement terminals, the voltage calibration value should be around 0x7ff or 1023.00) ***\r\n\r\n");
            printf("Voltage calibration value (hex)   = 0x%x\r\n", data);
            printf("Voltage calibration value (float) = %2.2f\r\n", app_multimeterData.voltage_cal);
            
            app_multimeterData.state = APP_MULTIMETER_CALIBRATE_CURRENT;
        }
        case APP_MULTIMETER_CALIBRATE_CURRENT:
        {
            uint16_t data;
            
            if ((getTick() < delay))
                break;

            data = MULTIMETER_readChannel(MULTIMETER_I_CHANNEL);
            app_multimeterData.current_cal = (float)data/2;
            
            printf("Current calibration value (hex)   = 0x%x\r\n", data);
            printf("Current calibration value (float) = %2.2f\r\n", app_multimeterData.current_cal);
            
            app_multimeterData.state = APP_MULTIMETER_CALIBRATE_CAPACITANCE;
        }
        case APP_MULTIMETER_CALIBRATE_CAPACITANCE:
        {
            uint16_t data;
            
            if ((getTick() < delay))
                break;

            data = MULTIMETER_readChannel(MULTIMETER_C_CHANNEL);
            if (data == 0)
            {
                app_multimeterData.capacitance_cal = 0;
            }
            else
            {
                app_multimeterData.capacitance_cal = (64285.0 /((float)data * 2));
                if (app_multimeterData.capacitance_cal < 1)
                { 
                    app_multimeterData.capacitance_cal = 0;
                }
            }

            printf("Capacitance calibration value (hex)   = 0x%x\r\n", data);
            printf("Capacitance calibration value (float) = %2.2f\r\n", app_multimeterData.capacitance_cal);
            
            app_multimeterData.state = APP_MULTIMETER_STATE_SERVICE_TASKS;
        }
        case APP_MULTIMETER_STATE_SERVICE_TASKS:
        {
            time = getTick();
            if ((getTick() < delay))
                break;
            delay = (getTick() + 1000);         

            app_multimeterData.voltage = MULTIMETER_readVoltage();
            app_multimeterData.current = MULTIMETER_readCurrent();
            app_multimeterData.capacitance = MULTIMETER_readCapacitance();            
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
