/********************************** (C) COPYRIGHT *******************************
 * File Name          : usbd_composite_km.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/04/06
 * Description        : USB keyboard and mouse processing.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#ifndef __USBD_COMPOSITE_KM_H
#define __USBD_COMPOSITE_KM_H

/*******************************************************************************/
/* Header Files */
#include "debug.h"
#include "string.h"
#include "button.h"
#include "usb_desc.h"

/*******************************************************************************/
/* Keyboard Key Value Macro Definition */
#define DEF_KEY_CHAR_W              0x1A                                        /* "W" */
#define DEF_KEY_CHAR_A              0x04                                        /* "A" */
#define DEF_KEY_CHAR_S              0x16                                        /* "S" */
#define DEF_KEY_CHAR_D              0x07                                        /* "D" */

#define KEY_SPACE 0x2c // Keyboard Spacebar
#define KEY_A 0x04 // Keyboard a and A
#define KEY_B 0x05 // Keyboard b and B
#define KEY_C 0x06 // Keyboard c and C
#define KEY_D 0x07 // Keyboard d and D
#define KEY_E 0x08 // Keyboard e and E
#define KEY_F 0x09 // Keyboard f and F
#define KEY_G 0x0a // Keyboard g and G
#define KEY_H 0x0b // Keyboard h and H
#define KEY_I 0x0c // Keyboard i and I
#define KEY_J 0x0d // Keyboard j and J
#define KEY_K 0x0e // Keyboard k and K
#define KEY_L 0x0f // Keyboard l and L
#define KEY_M 0x10 // Keyboard m and M
#define KEY_N 0x11 // Keyboard n and N
#define KEY_O 0x12 // Keyboard o and O
#define KEY_P 0x13 // Keyboard p and P
#define KEY_Q 0x14 // Keyboard q and Q
#define KEY_R 0x15 // Keyboard r and R
#define KEY_S 0x16 // Keyboard s and S
#define KEY_T 0x17 // Keyboard t and T
#define KEY_U 0x18 // Keyboard u and U
#define KEY_V 0x19 // Keyboard v and V
#define KEY_W 0x1a // Keyboard w and W
#define KEY_X 0x1b // Keyboard x and X
#define KEY_Y 0x1c // Keyboard y and Y
#define KEY_Z 0x1d // Keyboard z and Z

#define KEY_1 0x1e // Keyboard 1 and !
#define KEY_2 0x1f // Keyboard 2 and @
#define KEY_3 0x20 // Keyboard 3 and #
#define KEY_4 0x21 // Keyboard 4 and $
#define KEY_5 0x22 // Keyboard 5 and %
#define KEY_6 0x23 // Keyboard 6 and ^
#define KEY_7 0x24 // Keyboard 7 and &
#define KEY_8 0x25 // Keyboard 8 and *
#define KEY_9 0x26 // Keyboard 9 and (
#define KEY_0 0x27 // Keyboard 0 and )
/*******************************************************************************/
/* Global Variable Declaration */
extern volatile uint8_t  KB_LED_Last_Status;
extern volatile uint8_t  KB_LED_Cur_Status;


/*******************************************************************************/
extern button_t btn_dot;
extern button_t btn_dash;

extern void btn_dot_callback(void*  b);
extern void btn_dash_callback(void*  b);

extern uint8_t btn_dot_read( void );
extern uint8_t btn_dash_read( void );


/*******************************************************************************/
// Morse code dictionary
typedef struct {
    const char *morse;
    char letter;
} MorseCode;

#define MORSE_BUFFER_LEN 10
#define MORSE_TIMEOUT_TO_SEND 1800
#define MORSE_TIMEOUT_MAX 5000
typedef struct
{
    char buffer[MORSE_BUFFER_LEN];
    char char_to_send;
    uint8_t index;
    uint8_t is_buffer_empty;
    uint32_t timeout;
}morse_t;

extern morse_t my_morse;



/*******************************************************************************/
/* Function Declaration */
extern void TIM3_Init( uint16_t arr, uint16_t psc );
extern void KB_Scan_Init( void );
extern void KB_Sleep_Wakeup_Cfg( void );
extern void KB_Scan_Handle( void );
extern void USB_Sleep_Wakeup_CFG( void );
extern void MCU_Sleep_Wakeup_Operate( void );


#endif