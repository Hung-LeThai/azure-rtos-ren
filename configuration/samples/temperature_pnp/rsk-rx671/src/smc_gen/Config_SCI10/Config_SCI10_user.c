/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : Config_SCI10_user.c
* Version      : 1.10.0
* Device(s)    : R5F5671EHxFB
* Description  : This file implements device driver for Config_SCI10.
* Creation Date: 2021-07-20
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "Config_SCI10.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_sci10_tx_address;                /* SCI10 transmit buffer address */
extern volatile uint16_t  g_sci10_tx_count;                   /* SCI10 transmit data number */
extern volatile uint8_t * gp_sci10_rx_address;                /* SCI10 receive buffer address */
extern volatile uint16_t  g_sci10_rx_count;                   /* SCI10 receive data number */
extern volatile uint16_t  g_sci10_rx_length;                  /* SCI10 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI10_Create_UserInit
* Description  : This function adds user code after initializing the SCI10 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI10_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI10_transmit_interrupt
* Description  : This function is TXI10 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_SCI10_TXI10
#pragma interrupt r_Config_SCI10_transmit_interrupt(vect=VECT(SCI10,TXI10),fint)
#else
#pragma interrupt r_Config_SCI10_transmit_interrupt(vect=VECT(SCI10,TXI10))
#endif
static void r_Config_SCI10_transmit_interrupt(void)
{
    if (0U < g_sci10_tx_count)
    {
        SCI10.TDR = *gp_sci10_tx_address;
        gp_sci10_tx_address++;
        g_sci10_tx_count--;
    }
    else
    {
        SCI10.SCR.BIT.TIE = 0U;
        SCI10.SCR.BIT.TEIE = 1U;
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI10_transmitend_interrupt
* Description  : This function is TEI10 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI10_transmitend_interrupt(void)
{
    /* Set TXD10 pin */
    PORT8.PMR.BYTE &= 0x7FU;

    SCI10.SCR.BIT.TIE = 0U;
    SCI10.SCR.BIT.TE = 0U;
    SCI10.SCR.BIT.TEIE = 0U;
    
    r_Config_SCI10_callback_transmitend();
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI10_receive_interrupt
* Description  : This function is RXI10 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_SCI10_RXI10
#pragma interrupt r_Config_SCI10_receive_interrupt(vect=VECT(SCI10,RXI10),fint)
#else
#pragma interrupt r_Config_SCI10_receive_interrupt(vect=VECT(SCI10,RXI10))
#endif
static void r_Config_SCI10_receive_interrupt(void)
{
    if (g_sci10_rx_length > g_sci10_rx_count)
    {
        *gp_sci10_rx_address = SCI10.RDR;
        gp_sci10_rx_address++;
        g_sci10_rx_count++;
    }
    
    if (g_sci10_rx_length <= g_sci10_rx_count)
    {
        /* All data received */
        SCI10.SCR.BIT.RIE = 0U;
        SCI10.SCR.BIT.RE = 0U;
        r_Config_SCI10_callback_receiveend();
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI10_receiveerror_interrupt
* Description  : This function is ERI10 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void r_Config_SCI10_receiveerror_interrupt(void)
{
    uint8_t err_type;
    
    r_Config_SCI10_callback_receiveerror();
    
    /* Clear overrun, framing and parity error flags */
    err_type = SCI10.SSR.BYTE;
    err_type &= 0xC7U;
    err_type |= 0xC0U;
    SCI10.SSR.BYTE = err_type;
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI10_callback_transmitend
* Description  : This function is a callback function when SCI10 finishes transmission
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI10_callback_transmitend(void)
{
    /* Start user code for r_Config_SCI10_callback_transmitend. Do not edit comment generated here */
    void demo_printf_transmit_end(void);
    demo_printf_transmit_end();
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI10_callback_receiveend
* Description  : This function is a callback function when SCI10 finishes reception
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI10_callback_receiveend(void)
{
    /* Start user code for r_Config_SCI10_callback_receiveend. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI10_callback_receiveerror
* Description  : This function is a callback function when SCI10 reception encounters error
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI10_callback_receiveerror(void)
{
    /* Start user code for r_Config_SCI10_callback_receiveerror. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */