#include <xc.h>

#include "uart.h"
#include "timer.h"

extern volatile PARAM_RX paramRx;
extern volatile PARAM_TX paramTx;

extern volatile uint32_t contador_ms;

extern TimerCallback_t user_callback;

void __interrupt() ISR(void){

       if (PIR1bits.RCIF) {
        if (RCSTAbits.OERR) {  // Error de sobrecarga
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
        } else {
            uint8_t next = (paramRx.headRx + 1) % BUFFER_MAX;
            if (next != paramRx.tailRx) { // Si hay espacio
                paramRx.buffer[paramRx.headRx] = RCREG;
                paramRx.headRx = next;
            }
        }
        PIR1bits.RCIF = 0;
    }

    // --- Transmision ---
    if (PIR1bits.TXIF && PIE1bits.TXIE) {
        if (paramTx.headTx != paramTx.tailTx) {
            TXREG = paramTx.buffer[paramTx.tailTx];
            paramTx.tailTx = (paramTx.tailTx + 1) % BUFFER_MAX;
        } else {
            PIE1bits.TXIE = 0; // Desactiva interrupcion si no hay mas datos
        }
    }

    // TIMER0
    if(INTCONbits.TMR0IF){
        TMR0 = 6;
        if(user_callback){
            user_callback();
        }
        INTCONbits.TMR0IF = 0;
    }
    // TIMER1
    if(PIR1bits.TMR2IF){
        if(user_callback){
            user_callback();
        }
        PIR1bits.TMR2IF=0;
    }
}
