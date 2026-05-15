
#ifndef UART_H
#define	UART_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

#define BUFFER_MAX 50
#define _XTAL_FREQ 8000000UL


typedef struct{
    uint8_t buffer[BUFFER_MAX];
    uint8_t headTx;
    uint8_t tailTx;
}PARAM_TX;

typedef struct{
    uint8_t buffer[BUFFER_MAX];
    uint8_t headRx;
    uint8_t tailRx;
}PARAM_RX;

void UART_Init(unsigned long baudrate);
void uart_write(uint8_t data);
int uart_read(void);
uint8_t uart_available(void);
void uart_print(uint8_t *s);


#endif	/* XC_HEADER_TEMPLATE_H */

