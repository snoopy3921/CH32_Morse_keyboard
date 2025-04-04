/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/26
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 * @Note
 * Composite Keyboard and Mouse Example:
 * This example uses PB12-PB15 and PA4-PA7 to simulate keyboard key pressing and mouse
 * movement respectively, active low.
 * At the same time, it also uses USART2(PA3) to receive the specified data sent from
 * the host to simulate the pressing and releasing of the following specific keyboard
 * keys. Data is sent in hexadecimal format and 1 byte at a time.
 * 'W' -> 0x1A
 * 'A' -> 0x04
 * 'S' -> 0x16
 * 'D' -> 0x07
 */

#include <ch32x035_usbfs_device.h>
#include "debug.h"
#include "usbd_composite_km.h"


/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    printf("ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    /* Initialize GPIO for keyboard scan */
    KB_Scan_Init( );
    KB_Sleep_Wakeup_Cfg( );
    printf( "KB Scan Init OK!\r\n" );

    button_init(&btn_dot,	10,	BUTTON_HW_PRESSED_LOW,	btn_dot_read,	btn_dot_callback);
    button_init(&btn_dash,	10,	BUTTON_HW_PRESSED_LOW,	btn_dash_read,	btn_dash_callback);

	button_enable(&btn_dot);
	button_enable(&btn_dash);
    
    /* Initialize timer for Keyboard and mouse scan timing */
    TIM3_Init( 1, SystemCoreClock / 10000 - 1 );
    printf( "TIM3 Init OK!\r\n" );



    /* Usb Init */
    USBFS_RCC_Init( );
    USBFS_Device_Init( ENABLE , PWR_VDD_SupplyVoltage());
    USB_Sleep_Wakeup_CFG( );
    while(1)
    {
        /* Determine if enumeration is complete, perform data transfer if completed */
        if(USBFS_DevEnumStatus)
        {
            /* Handle keyboard scan data */
            KB_Scan_Handle(  );

        }
    }
}