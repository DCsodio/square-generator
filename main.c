/*
 * File:   main.c
 * Author: PC
 *
 * Created on 8 de abril de 2026, 12:12
 */


#include <xc.h>
#include "config.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "helpers.h"
#include "uart.h"
#include "timer.h"

#define _XTAL_FREQ 8000000UL
#define BUFFER_SIZE 40

// ===== Variables de control =====
uint32_t TH = 0;
uint32_t TL = 0;
uint32_t CC = 0;

uint32_t ciclos_actual = 0;

// ===== Timer =====
volatile uint32_t contador_ms = 0;

void timer_contar_handler(void){
    contador_ms++;
}

// ===== Estados =====
typedef enum {
    IDLE,
    ESTADO_HIGH,
    ESTADO_LOW
} estado_t;

//ejecucion de la maquina de estado
volatile estado_t estado = IDLE;
bool ejecutar = false;

//Analisis uart
uint8_t buffer[BUFFER_SIZE];
uint8_t indice=0;
bool deteccionDato = false;
bool nuevosDatos = false;

bool primerDato=false;

// ===== MAIN =====
void main(void){

    ANSEL = 0x00;   //Todos los pines a digitales
    CCP1CON=0X00;   //RB3 como digital
    OSCCONbits.SCS=0b00;    //Los fusibles/config.h selecciona que reloj usar
    OSCCONbits.IRCF=0b111;  //Crisal interno 8MHZ
    
    
    TRISBbits.TRISB7 = 0; // salida
    TRISBbits.TRISB1=0; //entrada
    
    PORTBbits.RB1=1;
    __delay_ms(200);
    PORTBbits.RB1=0;
    __delay_ms(200);
    PORTBbits.RB1=1;
    __delay_ms(200);
    PORTBbits.RB1=0;
    
    UART_Init(38400);
    Timer2_Init();
    Timer2_Stop();
    Timer_SetCallback(timer_contar_handler);
    contador_ms=0;

    while(1){

        // ===== RECEPCION UART =====
        if(uart_available() && nuevosDatos == false){
            buffer[indice]=(uint8_t)uart_read();
            
            if(buffer[0] == '>' && deteccionDato == false){
                deteccionDato = true;
                indice = 0;
            }else if (deteccionDato){
                if(indice < BUFFER_SIZE-1){
                    indice++;
                    if(buffer[indice-1] == '<'){
                        buffer[indice-1] = '\0';
                        nuevosDatos = true;
                        deteccionDato = false;
                        indice = 0;
                    }
                }else{
                    indice = 0;
                    deteccionDato = false;
                    nuevosDatos=false;
                    uart_print((uint8_t*)"Ingrese datos correctos!\r\n");
                }
            }
        }

        // ===== PARSEO =====
        if(nuevosDatos){
            if(!primerDato){
                primerDato=true;
                uart_print((uint8_t*)"GENERADOR DE PULSOS PERSONALIZADOS!\r\n");
                __delay_ms(2000);
            }
            uart_print((uint8_t*)"Nuevos datos...\r\n");
            
            if(parse_buffer(buffer,&TH, &TL, &CC) == 3){
                uint8_t numeroA[6];
                uint8_t numeroB[6];
                uint8_t numeroC[6];
                uint32_to_str(TH,numeroA);
                uint32_to_str(TL,numeroB);
                uint32_to_str(CC,numeroC);
                uart_print((uint8_t*)"up:");
                uart_print((uint8_t*)numeroA);
                uart_print((uint8_t*)"\r\n");
                uart_print((uint8_t*)"down:");
                uart_print((uint8_t*)numeroB);
                uart_print((uint8_t*)"\r\n");
                uart_print((uint8_t*)"ciclos:");
                uart_print((uint8_t*)numeroC);
                uart_print((uint8_t*)"\r\n");
                
                PORTBbits.RB1=1;
                ejecutar = true;
                estado = IDLE;
            }else{
                uart_print((uint8_t*)"ERROR VOLVER A ENVIAR\r\n");
                ejecutar = false;
                estado = IDLE;
            }
            nuevosDatos = false;
        }

        //PROTECCION contador_ms
        INTCONbits.GIE=0;
        uint32_t contador_ms_safe=contador_ms;
        INTCONbits.GIE=1;
        
        // ===== MAQUINA DE ESTADOS =====
        switch(estado){
         
            case IDLE:
                if(ejecutar){
                    Timer2_Start();
                    ciclos_actual = 0;
                    PORTBbits.RB7 = 1;
                    contador_ms = 0;
                    estado = ESTADO_HIGH;
                }
            break;

            case ESTADO_HIGH:
                
                if(contador_ms_safe >= TH){
                    PORTBbits.RB7 = 0;
                    contador_ms = 0;
                    estado = ESTADO_LOW;
                }
            break;

            case ESTADO_LOW:
                if(contador_ms_safe >= TL){
                    contador_ms = 0;
                    ciclos_actual++;

                    if(ciclos_actual >= CC){
                        estado = IDLE;
                        PORTBbits.RB1=0;
                        ejecutar = false;
                        Timer2_Stop();
                    }else{
                        PORTBbits.RB7 = 1;
                        estado = ESTADO_HIGH;
                    }
                }
            break;
        }
    }
}