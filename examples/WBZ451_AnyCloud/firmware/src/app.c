/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "stdio.h"
#include "config/default/peripheral/gpio/plib_gpio.h"
#include "config/default/peripheral/tc/plib_tc3.h"

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
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

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
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

uint64_t tick;
#define SECOND 1000


void tickrate(uintptr_t context) {
    tick++;

}
uint64_t getTick(void){
    return tick;
}



void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    TC3_TimerStart();
    TC3_TimerCallbackRegister((TC_TIMER_CALLBACK)tickrate, (uintptr_t )NULL);



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}



/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

uint64_t getTick(void);
void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
            
            printf("\r\nWelcome to the AnyCloud Host MCU Example for the WBZ451 Curiosity Board!!!\r\n\r\n");


            if (appInitialized)
            {

                appData.state = APP_STATE_SERVICE_TASKS;
                appData.LED_user = LED_STATE_ON;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            static uint64_t myTick;
            //static bool state;
            
            if (myTick < getTick())
            {
                switch (appData.LED_user)
                {
                    case LED_STATE_ON: LED_Clear(); break;
                    case LED_STATE_OFF: LED_Set(); break;
                    case LED_STATE_BLINKING: LED_Toggle(); break;
                    default: break;
                }
                myTick = getTick() + (SECOND * 1);

                
            }

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
