/********************************** (C) COPYRIGHT *******************************
 * File Name          : usbd_composite_km.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/04/06
 * Description        : USB keyboard and mouse processing.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/


/*******************************************************************************/
/* Header Files */
#include <ch32x035_usbfs_device.h>
#include "usbd_composite_km.h"

/*******************************************************************************/
/* Global Variable Definition */

/* Mouse */
volatile uint8_t  MS_Scan_Done = 0x00;                                          // Mouse Movement Scan Done
volatile uint16_t MS_Scan_Result = 0x00F0;                                      // Mouse Movement Scan Result
uint8_t  MS_Data_Pack[ 4 ] = { 0x00 };                                          // Mouse IN Data Packet

/* Keyboard */
volatile uint8_t  KB_Scan_Done = 0x00;                                          // Keyboard Keys Scan Done
volatile uint16_t KB_Scan_Result = 0x0003;                                      // Keyboard Keys Current Scan Result
volatile uint16_t KB_Scan_Last_Result = 0x0003;                                 // Keyboard Keys Last Scan Result
uint8_t  KB_Data_Pack[ 8 ] = { 0x00 };                                          // Keyboard IN Data Packet
volatile uint8_t  KB_LED_Last_Status = 0x00;                                    // Keyboard LED Last Result
volatile uint8_t  KB_LED_Cur_Status = 0x00;                                     // Keyboard LED Current Result

/* USART */
volatile uint8_t  USART_Recv_Dat = 0x00;
volatile uint8_t  USART_Send_Flag = 0x00;
volatile uint8_t  USART_Send_Cnt = 0x00;

button_t btn_dot;
button_t btn_dash;

morse_t my_morse;

MorseCode morse_dict[] = {
    {".-", KEY_A}, {"-...", KEY_B}, {"-.-.", KEY_C}, {"-..", KEY_D},
    {".", KEY_E}, {"..-.", KEY_F}, {"--.", KEY_G}, {"....", KEY_H},
    {"..", KEY_I}, {".---", KEY_J}, {"-.-", KEY_K}, {".-..", KEY_L},
    {"--", KEY_M}, {"-.", KEY_N}, {"---", KEY_O}, {".--.", KEY_P},
    {"--.-", KEY_Q}, {".-.", KEY_R}, {"...", KEY_S}, {"-", KEY_T},
    {"..-", KEY_U}, {"...-", KEY_V}, {".--", KEY_W}, {"-..-", KEY_X},
    {"-.--", KEY_Y}, {"--..", KEY_Z},
    {"-----", KEY_0}, {".----", KEY_1}, {"..---", KEY_2}, {"...--", KEY_3},
    {"....-", KEY_4}, {".....", KEY_5}, {"-....", KEY_6}, {"--...", KEY_7},
    {"---..", KEY_8}, {"----.", KEY_9},
    {"------", KEY_SPACE} // Space for word separation
};

/*******************************************************************************/
/* Interrupt Function Declaration */
void TIM3_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      TIM3_Init
 *
 * @brief   Initialize timer3 for keyboard and mouse scan.
 *
 * @param   arr - The specific period value
 *          psc - The specifies prescaler value
 *
 * @return  none
 */
void TIM3_Init( uint16_t arr, uint16_t psc )
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = { 0 };
    NVIC_InitTypeDef NVIC_InitStructure = { 0 };

    /* Enable Timer3 Clock */
    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );

    /* Initialize Timer3 */
    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure );

    TIM_ITConfig( TIM3, TIM_IT_Update, ENABLE );

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    /* Enable Timer3 */
    TIM_Cmd( TIM3, ENABLE );
}

/*********************************************************************
 * @fn      TIM3_IRQHandler
 *
 * @brief   This function handles TIM3 global interrupt request.
 *
 * @return  none
 */
static uint32_t tim3_millis = 0;
void TIM3_IRQHandler( void )
{
    
    if( TIM_GetITStatus( TIM3, TIM_IT_Update ) != RESET )
    {
        /* Clear interrupt flag */
        TIM_ClearITPendingBit( TIM3, TIM_IT_Update );
        tim3_millis++;
        if(my_morse.timeout < MORSE_TIMEOUT_MAX) my_morse.timeout ++;

    }
}
void btn_dot_callback(void*  b)
{
    button_t* me_b = (button_t*)b;
    switch (me_b->state) {
	case BUTTON_SW_STATE_PRESSED: {
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);

        if(my_morse.index < MORSE_BUFFER_LEN - 2)
        {
            my_morse.buffer[my_morse.index++] = '.'; 
            my_morse.buffer[my_morse.index] = '\0'; 
        }
        my_morse.is_buffer_empty = 0;
        my_morse.timeout = 0;
	}
		break;

	case BUTTON_SW_STATE_LONG_PRESSED: {
	}
		break;

	case BUTTON_SW_STATE_RELEASED: {
	}
		break;

	default:
		break;
	}
}
void btn_dash_callback(void*  b)
{
    button_t* me_b = (button_t*)b;
	switch (me_b->state) {
	case BUTTON_SW_STATE_PRESSED: {
        GPIO_SetBits(GPIOB, GPIO_Pin_12);

        if(my_morse.index < MORSE_BUFFER_LEN - 2)
        {
            my_morse.buffer[my_morse.index++] = '-'; 
            my_morse.buffer[my_morse.index] = '\0'; 
        }
        my_morse.is_buffer_empty = 0;
        my_morse.timeout = 0;
	}
		break;

	case BUTTON_SW_STATE_LONG_PRESSED: {
	}
		break;

	case BUTTON_SW_STATE_RELEASED: {

	}
		break;

	default:
		break;
	}
}
uint8_t btn_dot_read( void )
{
    return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
}
uint8_t btn_dash_read( void )
{
    return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);
}
/*********************************************************************
 * @fn      KB_Scan_Init
 *
 * @brief   Initialize IO for keyboard scan.
 *
 * @return  none
 */
void KB_Scan_Init( void )
{
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };

    /* Enable GPIOB clock */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );

    /* Initialize GPIOB (Pin0-Pin1) for the keyboard scan */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    my_morse.buffer[0] = '\0';
    my_morse.index = 0;
    my_morse.is_buffer_empty = 1;
    my_morse.timeout = MORSE_TIMEOUT_MAX;
}

/*********************************************************************
 * @fn      KB_Sleep_Wakeup_Cfg
 *
 * @brief   Configure keyboard wake up mode.
 *
 * @return  none
 */
void KB_Sleep_Wakeup_Cfg( void )
{
    EXTI_InitTypeDef EXTI_InitStructure = { 0 };

    /* Enable GPIOB clock */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE );

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOB, GPIO_PinSource0 );
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    GPIO_EXTILineConfig( GPIO_PortSourceGPIOB, GPIO_PinSource1 );
    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );

    EXTI->INTENR |= EXTI_INTENR_MR0 | EXTI_INTENR_MR1;
}

/*********************************************************************
 * @fn      KB_Scan
 *
 * @brief   Perform keyboard scan.
 *
 * @return  none
 */
void KB_Scan( void )
{

}

char morse_to_char(const char *morse) {
    for (int i = 0; i < sizeof(morse_dict) / sizeof(MorseCode); i++) {
        if (strcmp(morse, morse_dict[i].morse) == 0) {
            return morse_dict[i].letter;
        }
    }
    return 0x00; // Return '?' for unrecognized Morse code
}


/*********************************************************************
 * @fn      KB_Scan_Handle
 *
 * @brief   Handle keyboard scan data.
 *
 * @return  none
 */
void KB_Scan_Handle( void )
{
    if(tim3_millis % 10 == 0)
    {
        button_timer_polling(&btn_dot);
        button_timer_polling(&btn_dash);            
    }
    if(my_morse.is_buffer_empty == 0 && my_morse.timeout > MORSE_TIMEOUT_TO_SEND)
    {
        KB_Data_Pack[2] = morse_to_char(my_morse.buffer);
        USBFS_Endp_DataUp( DEF_UEP1, KB_Data_Pack, sizeof( KB_Data_Pack ), DEF_UEP_CPY_LOAD );

        my_morse.buffer[0] = '\0';
        my_morse.index = 0;
        my_morse.is_buffer_empty = 1;
        my_morse.timeout = MORSE_TIMEOUT_MAX;

        Delay_Ms(10);

        KB_Data_Pack[2] = 0; 
        USBFS_Endp_DataUp( DEF_UEP1, KB_Data_Pack, sizeof( KB_Data_Pack ), DEF_UEP_CPY_LOAD );


    }

}

/*********************************************************************
 * @fn      USB_Sleep_Wakeup_CFG
 *
 * @brief   Configure USB wake up mode
 *
 * @return  none
 */
void USB_Sleep_Wakeup_CFG( void )
{
    EXTI_InitTypeDef EXTI_InitStructure = { 0 };

    EXTI_InitStructure.EXTI_Line = EXTI_Line28;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init( &EXTI_InitStructure );
}

/*********************************************************************
 * @fn      MCU_Sleep_Wakeup_Operate
 *
 * @brief   Perform sleep operation
 *
 * @return  none
 */
void MCU_Sleep_Wakeup_Operate( void )
{
    printf( "Sleep\r\n" );
    __disable_irq();
    EXTI_ClearFlag( EXTI_Line0 | EXTI_Line1 );

    PWR_EnterSTOPMode(PWR_STOPEntry_WFE);
    SystemInit();
    SystemCoreClockUpdate();
    USBFS_RCC_Init();

    if( EXTI_GetFlagStatus( EXTI_Line0 | EXTI_Line1 ) != RESET  )
    {
        EXTI_ClearFlag( EXTI_Line0 | EXTI_Line1 );
        USBFS_Send_Resume( );
    }
    __enable_irq();
    printf( "Wake\r\n" );
}