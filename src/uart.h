// UART communication configuration and control
// Author: Josef Strnadel

#ifndef __UART_H__
#define __UART_H__
#define TERM_PORT UART5
/********************************************************************/

void uart_config();
void uart_init (UART_Type *uartch, int sysclk, int baud);
char uart_getchar (UART_Type *channel);
void uart_putchar (UART_Type *channel, char ch);
int uart_getchar_present (UART_Type *channel);

/********************************************************************/

#endif /* __UART_H__ */
