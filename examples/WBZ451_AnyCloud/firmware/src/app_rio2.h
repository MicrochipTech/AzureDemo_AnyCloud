/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_rio2.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_RIO2_Initialize" and "APP_RIO2_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_RIO2_STATES" definition).  Both
    are defined here for convenience.
 *******************************************************************************/

#ifndef _APP_RIO2_H
#define _APP_RIO2_H

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

    // *****************************************************************************
    // *****************************************************************************
    // Section: Type Definitions
    // *****************************************************************************
    // *****************************************************************************

    // *****************************************************************************

    /* Application states

      Summary:
        Application states enumeration

      Description:
        This enumeration defines the valid application states.  These states
        determine the behavior of the application at various times.
     */

    typedef enum {
        /* Application's state machine's initial state. */
        APP_RIO2_STATE_INIT = 0,
        APP_RIO2_STATE_WAIT_RST,
        APP_RIO2_STATE_ECHO_OFF,
        APP_RIO2_STATE_GET_VERSION,
        APP_RIO2_STATE_CONFIG_AP,
        APP_RIO2_STATE_CONFIG_AP_OK,
        APP_RIO2_STATE_CONNECT_STA,
        APP_RIO2_STATE_WAIT_IP,
        APP_RIO2_STATE_OPEN_UDP_SOCKET,
        APP_RIO2_STATE_UDP_SOCKET_BIND,
        APP_RIO2_STATE_UDP_SOCKET_BINDED,
        APP_RIO2_STATE_UDP_SOCKET_RCV_DATA,
        APP_RIO2_STATE_UDP_SOCKET_REPLY_DATA,
        APP_RIO2_STATE_RESOLVE_DNS,
        APP_RIO2_STATE_WAIT_RESOLVE_DNS,
        APP_RIO2_STATE_OPEN_TCP_SOCKET,
        APP_RIO2_STATE_TCP_SOCKET_BIND,
        APP_RIO2_STATE_TCP_SOCKET_BINDED_REMOTE,
        APP_RIO2_STATE_TCP_SOCKET_BINDED,
        APP_RIO2_STATE_TCP_SOCKET_CONNECTED,
        APP_MQTT_STATE_INIT_MQTT,
        APP_MQTT_STATE_INIT_MQTT_OK,
        APP_MQTT_STATE_CONNECTED,
        APP_MQTT_STATE_RCV_DATA,
        APP_MQTT_STATE_PUB_RESPONSE,
        APP_MQTT_STATE_SUBSCRIBE,
        APP_MQTT_STATE_SUBSCRIBE_COMPLETE,
                APP_RIO2_STATE_AZURE_PUB_DPS_REGISTRATION_PUT,
                APP_RIO2_STATE_AZURE_PUB_DPS_REGISTRATION_GET,
                APP_RIO2_STATE_AZURE_GET_ASSIGN_HUB_AND_DISCONNECT,
                APP_RIO2_STATE_AZURE_WAIT_DISCONNET,
                APP_RIO2_STATE_AZURE_RECONNECT,
                APP_RIO2_STATE_AZURE_SUBSCRIBE_1,
                APP_RIO2_STATE_AZURE_SUBSCRIBE_2,
                APP_RIO2_STATE_AZURE_SUBSCRIBE_3,
                APP_RIO2_STATE_AZURE_SUBSCRIBE_DONE,
        APP_MQTT_STATE_PUBLISH,
		APP_MQTT_STATE_WAIT_PUB,
        APP_MQTT_STATE_DISCONNECTED,
        APP_RIO2_STATE_TCP_SOCKET_RCV_DATA,
        APP_RIO2_STATE_TCP_SOCKET_READ_DATA,
        APP_RIO2_STATE_TCP_SOCKET_WRITE,
        APP_RIO2_STATE_TCP_SOCKET_CLOSE,
        APP_RIO2_STATE_TCP_SOCKET_WAIT_CLOSE,


        APP_RIO2_STATE_SOCKET_IDLE,


        /* TODO: Define states used by the application state machine. */

    } APP_RIO2_STATES;


    // *****************************************************************************

    /* Application Data

      Summary:
        Holds application data

      Description:
        This structure holds the application's data.

      Remarks:
        Application strings and buffers are be defined outside this structure.
     */

    typedef struct {
        /* The application's current state */
        APP_RIO2_STATES state;

        /* TODO: Define any additional data used by the application. */

    } APP_RIO2_DATA;

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

    /*******************************************************************************
      Function:
        void APP_RIO2_Initialize ( void )

      Summary:
         MPLAB Harmony application initialization routine.

      Description:
        This function initializes the Harmony application.  It places the
        application in its initial state and prepares it to run so that its
        APP_RIO2_Tasks function can be called.

      Precondition:
        All other system initialization routines should be called before calling
        this routine (in "SYS_Initialize").

      Parameters:
        None.

      Returns:
        None.

      Example:
        <code>
        APP_RIO2_Initialize();
        </code>

      Remarks:
        This routine must be called from the SYS_Initialize function.
     */

    void APP_RIO2_Initialize(void);


    /*******************************************************************************
      Function:
        void APP_RIO2_Tasks ( void )

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
        APP_RIO2_Tasks();
        </code>

      Remarks:
        This routine must be called from SYS_Tasks() routine.
     */

    void APP_RIO2_Tasks(void);

    //DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_RIO2_H */

/*******************************************************************************
 End of File
 */

