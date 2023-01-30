/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_multimeter.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_MULTIMETER_Initialize" and "APP_MULTIMETER_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_MULTIMETER_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_MULTIMETER_H
#define _APP_MULTIMETER_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

uint64_t getTick(void);

// *****************************************************************************
// *****************************************************************************
// Section: Port Pin Definitions
// *****************************************************************************
// *****************************************************************************
/*** Macros for Multimeter 'A' pin ***/
#define MULTIMETER_A_Set()               (GPIOB_REGS->GPIO_LATSET = (1<<1))
#define MULTIMETER_A_Clear()             (GPIOB_REGS->GPIO_LATCLR = (1<<1))
#define MULTIMETER_A_Toggle()            (GPIOB_REGS->GPIO_LATINV= (1<<1))
#define MULTIMETER_A_Get()               ((GPIOB_REGS->GPIO_PORT >> 1) & 0x1)
#define MULTIMETER_A_OutputEnable()      (GPIOB_REGS->GPIO_TRISCLR = (1<<1))
#define MULTIMETER_A_InputEnable()       (GPIOB_REGS->GPIO_TRISSET = (1<<1))
#define MULTIMETER_A_PIN                  GPIO_PIN_RB1
/*** Macros for Multimeter 'B' pin ***/
#define MULTIMETER_B_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<2))
#define MULTIMETER_B_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<2))
#define MULTIMETER_B_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<2))
#define MULTIMETER_B_Get()               ((GPIOA_REGS->GPIO_PORT >> 2) & 0x1)
#define MULTIMETER_B_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<2))
#define MULTIMETER_B_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<2))
#define MULTIMETER_B_PIN                  GPIO_PIN_RA2
/*** Macros for Multimeter 'C' pin ***/
#define MULTIMETER_C_Set()               (GPIOA_REGS->GPIO_LATSET = (1<<2))
#define MULTIMETER_C_Clear()             (GPIOA_REGS->GPIO_LATCLR = (1<<2))
#define MULTIMETER_C_Toggle()            (GPIOA_REGS->GPIO_LATINV= (1<<2))
#define MULTIMETER_C_Get()               ((GPIOA_REGS->GPIO_PORT >> 2) & 0x1)
#define MULTIMETER_C_OutputEnable()      (GPIOA_REGS->GPIO_TRISCLR = (1<<2))
#define MULTIMETER_C_InputEnable()       (GPIOA_REGS->GPIO_TRISSET = (1<<2))
#define MULTIMETER_C_PIN                  GPIO_PIN_RA2

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
#define MULTIMETER_I_CHANNEL   0x00
#define MULTIMETER_U_CHANNEL   0x01
#define MULTIMETER_R_CHANNEL   0x02
#define MULTIMETER_C_CHANNEL   0x03

#define MULTIMETER_MAX_VOLTAGE 4095.0

#define MULTIMETER_MAX_RESRANGE   5
#define MULTIMETER_RESRANGE_DLY 100

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
    /* Application's state machine's initial state. */
    APP_MULTIMETER_STATE_INIT=0,
            APP_MULTIMETER_CONVERT_DUMMY,
            APP_MULTIMETER_CALIBRATE_VOLTAGE,
            APP_MULTIMETER_CALIBRATE_CURRENT,
            APP_MULTIMETER_CALIBRATE_CAPACITANCE,
    APP_MULTIMETER_STATE_SERVICE_TASKS,
    /* TODO: Define states used by the application state machine. */

} APP_MULTIMETER_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    APP_MULTIMETER_STATES state;

    /* TODO: Define any additional data used by the application. */
    float capacitance_cal;
    float voltage_cal;
    float current_cal;
    uint8_t scan_range;
            
    float capacitance;
    float voltage;
    float current;
    float resistance;

    uint64_t time;

} APP_MULTIMETER_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

float MULTIMETER_getVoltage(void);

float MULTIMETER_getCurrent(void);

float MULTIMETER_getCapacitance(void);

float MULTIMETER_getResistance(void);

uint16_t MULTIMETER_getResistanceRange (uint8_t range);

void MULTIMETER_setResistanceRange (uint8_t range);

/*******************************************************************************
  Function:
    void APP_MULTIMETER_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_MULTIMETER_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_MULTIMETER_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_MULTIMETER_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_MULTIMETER_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_MULTIMETER_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_MULTIMETER_Tasks( void );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_MULTIMETER_H */

/*******************************************************************************
 End of File
 */

