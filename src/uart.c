// UART communication configuration and control
// Author: Josef Strnadel

#include "MK60D10.h"
#include "uart.h"

void uart_config()
{
	// Configure the TX and RX pins for UART5
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[8] = PORT_PCR_MUX(0x03);
	PORTE->PCR[9] = PORT_PCR_MUX(0x03);
}
/********************************************************************/
/*
 * Initialize the UART for 8N1 operation, interrupts disabled, and
 * no hardware flow-control
 *
 * NOTE: Since the UARTs are pinned out in multiple locations on most
 *       Kinetis devices, this driver does not enable UART pin functions.
 *       The desired pins should be enabled before calling this init function.
 *
 * Parameters:
 *  uartch      UART channel to initialize
 *  sysclk      UART module Clock in kHz(used to calculate baud)
 *  baud        UART baud rate
 */
void uart_init (UART_Type *uartch, int sysclk, int baud)
{
    uint16_t sbr, brfa;
    uint8_t temp;

	/* Enable the clock to the selected UART */
	if(uartch == UART4)
		SIM->SCGC1 |= SIM_SCGC1_UART4_MASK;
    else
    	SIM->SCGC1 |= SIM_SCGC1_UART5_MASK;

    /* Make sure that the transmitter and receiver are disabled while we
     * change settings.
     */
    uartch->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );

    /* Configure the UART for 8-bit mode, no parity */
    uartch->C1 = 0;	/* We need all default settings, so entire register is cleared */

    /* Calculate baud settings */
    sbr = (uint16_t)((sysclk*1000)/(baud * 16));

    /* Save off the current value of the UARTx_BDH except for the SBR field */
    temp = uartch->BDH & ~(UART_BDH_SBR(0x1F));

    uartch->BDH = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
    uartch->BDL = (uint8_t)(sbr & UART_BDL_SBR_MASK);

    /* Determine if a fractional divider is needed to get closer to the baud rate */
    brfa = (((sysclk*32000)/(baud * 16)) - (sbr * 32));

    /* Save off the current value of the UARTx_C4 register except for the BRFA field */
    temp = uartch->C4 & ~(UART_C4_BRFA(0x1F));

    uartch->C4 = temp |  UART_C4_BRFA(brfa);

    /* Enable receiver and transmitter */
	uartch->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
}
/********************************************************************/
/*
 * Wait for a character to be received on the specified UART
 *
 * Parameters:
 *  channel      UART channel to read from
 *
 * Return Values:
 *  the received character
 */
char uart_getchar (UART_Type *channel)
{
    /* Wait until character has been received */
    while (!(channel->S1 & UART_S1_RDRF_MASK));

    /* Return the 8-bit data from the receiver */
    return channel->D;
}
/********************************************************************/
/*
 * Wait for space in the UART Tx FIFO and then send a character
 *
 * Parameters:
 *  channel      UART channel to send to
 *  ch			 character to send
 */
void uart_putchar (UART_Type *channel, char ch)
{
    /* Wait until space is available in the FIFO */
    while(!(channel->S1 & UART_S1_TDRE_MASK));

    /* Send the character */
    channel->D = (uint8_t)ch;
 }
/********************************************************************/
/*
 * Check to see if a character has been received
 *
 * Parameters:
 *  channel      UART channel to check for a character
 *
 * Return values:
 *  0       No character received
 *  1       Character has been received
 */
int uart_getchar_present (UART_Type *channel)
{
    return (channel->S1 & UART_S1_RDRF_MASK);
}
/********************************************************************/
