/*
 * File:   uart.c
 * Author: PC
 *
 * Created on 14 de abril de 2026, 15:15
 */

#include "uart.h"

volatile PARAM_RX paramRx;
volatile PARAM_TX paramTx;

void UART_Init(unsigned long baudrate) {
    uint32_t x;
    x = (uint32_t)(_XTAL_FREQ - baudrate * 64) / (baudrate * 64); // BRGH = 0
    if (x > 255) {
        x = (uint32_t)(_XTAL_FREQ - baudrate * 16) / (baudrate * 16); // BRGH = 1
        TXSTAbits.BRGH = 1;
    } else {
        TXSTAbits.BRGH = 0;
    }
    SPBRG =(unsigned char) x;

    TXSTAbits.SYNC = 0;  // Modo asincrono
    RCSTAbits.SPEN = 1;  // Habilita puerto serial
    TXSTAbits.TXEN = 1;  // Habilita transmision
    RCSTAbits.CREN = 1;  // Habilita recepcion
    TXSTAbits.TX9 = 0;   // 8 bits
    RCSTAbits.RX9 = 0;   // 8 bits

    // Habilita interrupciones UART
    PIE1bits.RCIE = 1;   // Interrupcion por recepcion
    PIE1bits.TXIE = 0;   // Desactivada por ahora (se activa al enviar)
    INTCONbits.PEIE = 1; // Habilita interrupciones perifiricas
    INTCONbits.GIE = 1;  // Habilita interrupciones globales
}

void uart_write(uint8_t data) {
    uint8_t next = (paramTx.headTx + 1) % BUFFER_MAX;
    while (next == paramTx.tailTx);  // Espera si el buffer esta lleno

    paramTx.buffer[paramTx.headTx] = data;
    paramTx.headTx = next;

    // Activa interrupcion TX si esta desactivada
    PIE1bits.TXIE = 1;
}

int uart_read(void) {
    if (paramRx.headRx == paramRx.tailRx) return -1; // No hay datos disponibles
    uint8_t dato = paramRx.buffer[paramRx.tailRx];
    paramRx.tailRx = (paramRx.tailRx + 1) % BUFFER_MAX;
    return dato;
}

uint8_t uart_available(void) {
    return (paramRx.headRx != paramRx.tailRx);
}

void uart_print(uint8_t *s) {
    while (*s) uart_write(*s++);
}
