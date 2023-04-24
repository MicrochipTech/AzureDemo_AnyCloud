/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F57Q84
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/
    
#include "mcc_generated_files/mcc.h"

//#define SetAcquisitionChannel(X) do { ADPCH = X; } while (0)


extern uint8_t UART_flag ;
int16_t gain;
int16_t offset;
uint16_t ADC_MEAS = 0;
int24_t temp_c = 0;  
float float_TempC;

#define DegF(float_TempC)           (((float_TempC) * 1.8) + 32)
//#define DebugMode

void ADC_Temp_routine(void){
        
#ifdef DebugMode
    __delay_ms(1000);
        printf("Start ADC convert \r\n");
        printf("Value of gain: %d \r\n", gain);
        printf("offset: %d \r\n", offset);
        
        ADC_DischargeSampleCapacitor();
        ADC_MEAS = ADC_GetSingleConversion(channel_Temp);
        printf("ADC_MEAS: %d \r\n", ADC_MEAS);
        temp_c = (int24_t) (ADC_MEAS) * gain;
        printf("ADC x gain: %d \r\n", temp_c);
        temp_c = temp_c / 256;
        printf("data/256: %d \r\n", temp_c);
        temp_c = temp_c + offset;
        printf("add offset: %d \r\n", temp_c);
        float_TempC = temp_c;
        float_TempC = float_TempC/10;
        printf("Device Temperature in DegC: %.2fC \r\n", float_TempC);
        printf("Device Temperature in DegF: %.2fF \r\n", DegF(float_TempC));
        printf("----- \r\n");
#else
        __delay_ms(1000);
        printf("Start ADC convert \r\n");
        //printf("Value of gain: %d \r\n", gain);
        //printf("offset: %d \r\n", offset);
        
        ADC_DischargeSampleCapacitor();
        ADC_MEAS = ADC_GetSingleConversion(channel_Temp);
        //printf("ADC_MEAS: %d \r\n", ADC_MEAS);
        temp_c = (int24_t) (ADC_MEAS) * gain;
        //printf("ADC x gain: %d \r\n", temp_c);
        temp_c = temp_c / 256;
        //printf("data/256: %d \r\n", temp_c);
        temp_c = temp_c + offset;
        //printf("add offset: %d \r\n", temp_c);
        float_TempC = temp_c;
        float_TempC = float_TempC/10;
        printf("Device Temperature in DegC: %.2fC \r\n", float_TempC);
        printf("Device Temperature in DegF: %.2fF \r\n", DegF(float_TempC));
        printf("----- \r\n");
#endif
    
        
        
}




/*
                         Main application
 */
void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    
    
    gain = FLASH_ReadWord(DIA_TSHR1);  
    offset = FLASH_ReadWord(DIA_TSHR3);
    
    INTERRUPT_GlobalInterruptEnable();
    
    UART_flag = 0;
    
    while (1)
    {
        // Add your application code
        //ADC_Temp_routine();
        if(UART_flag == 1){
            IO_RF3_SetLow();
            ADC_Temp_routine();
        }else{
            IO_RF3_SetHigh();
        }
        
        
        //unsigned char Temp1 = UART1_Read();
        
        //printf("Timer start to conv/ert: %c \r\n", Temp1);
        
        
    }
}
/**
 End of File
*/