/*
 * File:   timer.c
 * Author: PC
 *
 * Created on 8 de abril de 2026, 10:53
 */


#include <xc.h>
#include "timer.h"

TimerCallback_t user_callback= NULL;
#if 0
void Timer0_Init(void){
    OPTION_REGbits.T0CS = 0; // reloj interno
    OPTION_REGbits.PSA = 0;  // prescaler asignado
    OPTION_REGbits.PS = 0b010; // 1:8

    TMR0 = 6; // ajuste para ~1ms

    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
}
#endif
//ajuste para 1ms
void Timer2_Init(void){
    PR2=124;
    T2CON=0b00000110;
    
    PIR1bits.TMR2IF=0;
    PIE1bits.TMR2IE=1;
    INTCONbits.PEIE=1;
    INTCONbits.GIE=1;
               
}

void Timer_SetCallback(TimerCallback_t callback){
    user_callback=callback;
}

void Timer2_Start(void){
    T2CONbits.TMR2ON=1;
}
void Timer2_Stop(void){
    T2CONbits.TMR2ON=0;
}