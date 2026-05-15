

#ifndef TIMER_H
#define	TIMER_H

#include <xc.h> // include processor files - each processor file is guarded.

typedef void (*TimerCallback_t)(void);

extern TimerCallback_t user_callback;

#if 0
void Timer0_Init(void);
#endif

void Timer2_Init(void);
void Timer_SetCallback(TimerCallback_t callback);
void Timer2_Start(void);
void Timer2_Stop(void);

#endif	/* XC_HEADER_TEMPLATE_H */

