/*
 * GPTM.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: HP
 */

#ifndef GPTM_H_
#define GPTM_H_


#include "regmap.h"
#define General_Purpose_Timer_Run_Mode_Clock    0x400FE604
typedef enum{
    Timer0=0 , Timer1=1, Timer2=2, Timer3=3, Timer4=4, Timer5=5
}gptm_timer_module_t;

typedef enum{
    TimerA = 0 , TimerB = 8
}gptm_types_t;

typedef enum{
    One_Shot , Periodic , Real_Time_Clock , Input_Edge_Count,Input_Edge_Time_Mode,PWM
}gptm_modes_t;

typedef enum{
    Count_Up , Count_Down
}gptm_direction_t;

typedef enum{
    positive_edge , negative_edge , both_edges
}gptm_edge_t;
void GPTM_MODULE_CLOCK(gptm_timer_module_t module);
void GPTM_Timer_Mode(gptm_modes_t mode , unsigned char port,gptm_types_t type,gptm_edge_t edge );
void Periodic_Mode(unsigned char port);
void One_Shot_Mode(unsigned char port);
void Real_Time_Clock_Mode(unsigned char port);
void Input_Edge_Count_Mode(unsigned char port,gptm_types_t type, gptm_edge_t edge);
void Input_Time_Mode(unsigned char port,gptm_types_t type,gptm_edge_t edge);
void PWM_Mode(unsigned char port,gptm_types_t type);

#endif /* GPTM_H_ */

