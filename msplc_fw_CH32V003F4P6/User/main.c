/*
    PLC Open source GPL v3 including
    - Embedded C Firmware GPL v3
    - GUI as Web browser application GPL v3
        - Web application/Website has to
        * Do people generally like/want to use PLC Style Programming ?
        Function block : FBD
        Ladder : LD
        Structured Text : SFC
        Or should we do a C/C++ Native PLC controller

        Applications should include
            Home Gateways
            Home controls/automations
*/

/*
    - Define the intermediary language
    - Manufacture Sample Firmware & IDE
*/

/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : JF Simon
 * Version            : V1.0.0
 * Date               : 2026-11-05
 * Description        : MSPLC Firmware for CH32V003 MCU
 *********************************************************************************
 * Copyright (c) 2026 GPL v3 Chrysalide Engineering
 *******************************************************************************/

/*
 *@Note
 *Multiprocessor communication mode routine:
 *Master:USART1_Tx(PD5)\USART1_Rx(PD6).
 *This routine demonstrates that USART1 receives the data sent by CH341 and inverts
 *it and sends it (baud rate 115200).
 *
 *Hardware connection:PD5 -- Rx
 *                     PD6 -- Tx
 *
 */

#include "debug.h"


/* Global define */


/* Global Variable */
vu8 val;

/*********************************************************************
 * @fn      USARTx_CFG
 *
 * @brief   Initializes the USART2 & USART3 peripheral.
 *
 * @return  none
 */
void USARTx_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

    /* USART1 TX-->D.5   RX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_30MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

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
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    USARTx_CFG();

    while(1)
    {

        // Quick test:

        const char * anim [] = {
                ".......*\0", /* 0 */
                "......*.\0", /* 1 */
                ".....*..\0", /* 2 */
                "....*...\0", /* 3 */
                "...*....\0", /* 4 */
                "..*.....\0", /* 5 */
                ".*......\0", /* 6 */
                "*.......\0"}; /* 7 */

        static int run = 1;

        static int j = 0;
        static int k = 1;

        if (run ) {
            if ((j < 7) && (k == 1)) {
                j += k;
            } else if ((j > 0) && (k == -1)) {
                j += k;
            } else {
                k = -k;
                j += k;
            }
            printf("%s %d %d\r\n", anim[j], j, k);
        }

        Delay_Ms(50);

        // while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) { /* waiting rx */ }
        // val = (USART_ReceiveData(USART1));
        // USART_SendData(USART1, val);
        // while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) { /* waiting tx */ }
    }
}
