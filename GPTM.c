/*
 * GPTM.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: HP
 */
volatile unsigned long int * GPTMTAILR = 0x40030028;
volatile unsigned long int * GPTMTBILR = 0x40030008;
volatile unsigned long int *GPTMCTL  = 0x4003000C;

#include "GPTM.h"
#include "std_types.h"
#include "utils.h"
void GPTM_MODULE_CLOCK(gptm_timer_module_t module){
    /**     11.4 Initialization and Configuration of GPTM  **/
       //To use a GPTM, the appropriate TIMERn bit must be set in the RCGCTIMER

    SetBit(General_Purpose_Timer_Run_Mode_Clock , module );
}

void GPTM_Timer_Mode(gptm_modes_t mode , unsigned char port,gptm_types_t type,gptm_edge_t edge ){
    if( mode == Periodic){
        Periodic_Mode(port);
   }
    else if(mode == One_Shot){
        One_Shot_Mode(port);
    }
    else if(mode == Real_Time_Clock){
        Real_Time_Clock_Mode(port);
       }
    else if(mode == Input_Edge_Count){
        Input_Edge_Count_Mode(port,type,edge);
    }
    else if(mode == Input_Edge_Time_Mode){
        Input_Time_Mode(port,type,edge);
       }
    else if(mode == PWM){
        PWM_Mode(port,type);
    }
}


void Periodic_Mode(unsigned char port){
    //1. Ensure the timer is disabled(the TnEN bit in the GPTMCTL register is cleared) before making any changes.
        ClearBit(port+GPTM_Control_Offset,0);

     //2. Write the GPTM Configuration Register (GPTMCFG) with a value of 0x0000.0000.
        WRITE_REG(port+GPTM_Control_Offset,0x00000000);

        //3. Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR)
        WRITE_REG(port+GPTM_Timer_A_Mode_Offset,0x00000010);

        //4. Optionally configure the TnSNAPS, TnWOT, TnMTE, and TnCDIR bits in the GPTMTnMR register
        //to select whether to capture the value of the free-running timer at time-out, use an external
        //trigger to start counting, configure an additional trigger or interrupt, and count up or down
        ClearBit(port+GPTM_Timer_A_Mode_Offset,4);

        //5. Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR)
       // WRITE_REG(port+GPTM_Timer_A_Interval_Load_Offset,0x00F42400);
        *GPTMTAILR = 0x00F42400;
        //7. Set the TnEN bit in the GPTMCTL register to enable the timer and start counting
        *GPTMCTL |= (1<<0);

}

void One_Shot_Mode(unsigned char port){
    //1. Ensure the timer is disabled(the TnEN bit in the GPTMCTL register is cleared) before making any changes.
            ClearBit(port+GPTM_Control_Offset,0);

         //2. Write the GPTM Configuration Register (GPTMCFG) with a value of 0x0000.0000.
            WRITE_REG(port+GPTM_Control_Offset,0x00000000);

            //3. Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR)
            WRITE_REG(port+GPTM_Timer_A_Mode_Offset,0x00000001);

            //4. Optionally configure the TnSNAPS, TnWOT, TnMTE, and TnCDIR bits in the GPTMTnMR register
            //to select whether to capture the value of the free-running timer at time-out, use an external
            //trigger to start counting, configure an additional trigger or interrupt, and count up or down
            ClearBit(port+GPTM_Timer_A_Mode_Offset,4);

            //5. Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR)
           // WRITE_REG(port+GPTM_Timer_A_Interval_Load_Offset,0x00F42400);
            *GPTMTAILR = 0x00F42400;
            //7. Set the TnEN bit in the GPTMCTL register to enable the timer and start counting
            *GPTMCTL |= (1<<0);
}

void Real_Time_Clock_Mode(unsigned char port){
    //1. Ensure the timer is disabled (the TAEN bit is cleared) before making any changes
    ClearBit(port+GPTM_Control_Offset,0);

    //2. If the timer has been operating in a different mode prior to this, clear any residual set bits in the
    //GPTM Timer n Mode (GPTMTnMR) register before reconfiguring.


    //3. Write the GPTM Configuration Register (GPTMCFG) with a value of 0x0000.0001.
    WRITE_REG(port+GPTM_Control_Offset,0x00000001);

    //4. Write the match value to the GPTM Timer n Match Register (GPTMTnMATCHR)

    //5. Set/clear the RTCEN and TnSTALL bit in the GPTM Control Register (GPTMCTL) as needed.

    //7. Set the TAEN bit in the GPTMCTL register to enable the timer and start counting.
    *GPTMCTL |= (1<<0);
}

void Input_Edge_Count_Mode(unsigned char port,gptm_types_t type, gptm_edge_t edge){
    //1. Ensure the timer is disabled (the TnEN bit is cleared) before making any changes.
    ClearBit(port+GPTM_Control_Offset,0);

    //2. Write the GPTM Configuration (GPTMCFG) register with a value of 0x0000.0004.
    WRITE_REG(port+GPTM_Control_Offset,0x00000004);

    //3. In the GPTM Timer Mode (GPTMTnMR) register, write the TnCMR field to 0x0 and the TnMR field to 0x3.


    if(type == TimerA){
           ClearBit(port+GPTM_Timer_A_Mode_Offset,2);
           WRITE_REG(port+GPTM_Timer_A_Mode_Offset,0x00000011);
    }

    else if(type == TimerB){
            ClearBit(port+GPTM_Timer_B_Mode_Offset,2);
            WRITE_REG(port+GPTM_Timer_B_Mode_Offset,0x00000011);

    }

    //4. Configure the type of event(s) that the timer captures by writing the TnEVENT field of the GPTM
           // Control (GPTMCTL) register
    if(edge == positive_edge)     WRITE_REG(port+GPTM_Control_Offset,0x00000000);
    else if(edge == negative_edge)     WRITE_REG(port+GPTM_Control_Offset,0x00000100);
    else if(edge ==  both_edges)     WRITE_REG(port+GPTM_Control_Offset,0x00001100);

    //5. Program registers according to count direction:
   // In down-count mode, the GPTMTnMATCHR and GPTMTnPMR registers are configured so
    //that the difference between the value in the GPTMTnILR and GPTMTnPR registers and the
    //GPTMTnMATCHR and GPTMTnPMR registers equals the number of edge events that must
    //be counted.

   // In up-count mode, the timer counts from 0x0 to the value in the GPTMTnMATCHR and
    //GPTMTnPMR registers. Note that when executing an up-count, the value of the GPTMTnPR
    //and GPTMTnILR must be greater than the value of GPTMTnPMR and GPTMTnMATCHR.

    // 7. Set the TnEN bit in the GPTMCTL register to enable the timer and begin waiting for edge events.
    *GPTMCTL |= (1<<0);
}

void Input_Time_Mode(unsigned char port,gptm_types_t type,gptm_edge_t edge){
    //1. Ensure the timer is disabled (the TnEN bit is cleared) before making any changes
    ClearBit(port+GPTM_Control_Offset,0);

    //2. Write the GPTM Configuration (GPTMCFG) register with a value of 0x0000.0004.
    WRITE_REG(port+GPTM_Control_Offset,0x00000004);

    //3. In the GPTM Timer Mode (GPTMTnMR) register, write the TnCMR field to 0x1 and the TnMR
   // field to 0x3 and select a count direction by programming the TnCDIR bit.
    if(type == TimerA){
        ClearBit(port+GPTM_Timer_A_Mode_Offset,2);
        WRITE_REG(port+GPTM_Timer_A_Mode_Offset,0x00000011);

        //4. Configure the type of event that the timer captures by writing the TnEVENT field of the GPTM
            //Control (GPTMCTL) register
        if(edge == positive_edge)     WRITE_REG(port+GPTM_Control_Offset,0x00000000);
             else if(edge == negative_edge)     WRITE_REG(port+GPTM_Control_Offset,0x00000100);
             else if(edge ==  both_edges)     WRITE_REG(port+GPTM_Control_Offset,0x00001100);

        //6. Load the timer start value into the GPTM Timer n Interval Load (GPTMTnILR) register.
             *GPTMTAILR = 0x00F42400;

             //8. Set the TnEN bit in the GPTM Control (GPTMCTL) register to enable the timer and start counting.
             SetBit(port+GPTM_Control_Offset,0);
 }

 else if(type == TimerB){
         ClearBit(port+GPTM_Timer_B_Mode_Offset,2);
         WRITE_REG(port+GPTM_Timer_B_Mode_Offset,0x00000011);

         //4. Configure the type of event that the timer captures by writing the TnEVENT field of the GPTM
                     //Control (GPTMCTL) register
                 if(edge == positive_edge)     WRITE_REG(port+GPTM_Control_Offset,0x00000000);
                      else if(edge == negative_edge)     WRITE_REG(port+GPTM_Control_Offset,0x00000100);
                      else if(edge ==  both_edges)     WRITE_REG(port+GPTM_Control_Offset,0x00001100);

          //6. Load the timer start value into the GPTM Timer n Interval Load (GPTMTnILR) register.
                *GPTMTBILR = 0x00F42400;

       //8. Set the TnEN bit in the GPTM Control (GPTMCTL) register to enable the timer and start counting.
                SetBit(port+GPTM_Control_Offset,8);
 }


    //5. If a prescaler is to be used, write the prescale value to the GPTM Timer n Prescale Register
    //(GPTMTnPR)

    //???????
}

void PWM_Mode(unsigned char port,gptm_types_t type){
    //1. Ensure the timer is disabled (the TnEN bit is cleared) before making any changes.
    ClearBit(port+GPTM_Control_Offset,0);

    //2. Write the GPTM Configuration (GPTMCFG) register with a value of 0x0000.0004
    WRITE_REG(port+GPTM_Control_Offset,0x00000004);

    //3. In the GPTM Timer Mode (GPTMTnMR) register, set the TnAMS bit to 0x1, the TnCMR bit to
   // 0x0, and the TnMR field to 0x2.


   if(type == TimerA)
   {
       SetBit(port+GPTM_Timer_A_Mode_Offset,3);
       ClearBit(port+GPTM_Timer_A_Mode_Offset,2);
       WRITE_REG(port+GPTM_Timer_A_Mode_Offset,0x00000010);


       //4. Configure the output state of the PWM signal (whether or not it is inverted) in the TnPWML field
               //of the GPTM Control (GPTMCTL) register.
       SetBit(port+GPTM_Control_Offset,6);

       //7. Load the timer start value into the GPTM Timer n Interval Load (GPTMTnILR) register
       *GPTMTAILR = 0x00F42400;

       //9. Set the TnEN bit in the GPTM Control (GPTMCTL) register to enable the timer and begin
          // generation of the output PWM signal.
       SetBit(port+GPTM_Control_Offset,0);
   }
   else if(type == TimerB){
       SetBit(port+GPTM_Timer_B_Mode_Offset,3);
       ClearBit(port+GPTM_Timer_B_Mode_Offset,2);
       WRITE_REG(port+GPTM_Timer_B_Mode_Offset,0x00000010);


       //4. Configure the output state of the PWM signal (whether or not it is inverted) in the TnPWML field
               //of the GPTM Control (GPTMCTL) register.
       SetBit(port+GPTM_Control_Offset,14);

       //7. Load the timer start value into the GPTM Timer n Interval Load (GPTMTnILR) register
       *GPTMTBILR = 0x00F42400;

       //9. Set the TnEN bit in the GPTM Control (GPTMCTL) register to enable the timer and begin
          // generation of the output PWM signal.
       SetBit(port+GPTM_Control_Offset,8);
   }


    //5. If a prescaler is to be used, write the prescale value to the GPTM Timer n Prescale Register
   // (GPTMTnPR).



    //8. Load the GPTM Timer n Match (GPTMTnMATCHR) register with the match value


}

