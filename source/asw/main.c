/**
* \file main.c
* \author Sree Samanvitha Manoor Vadhoolas
* \date 1.12.2023
*
* \brief Main control function for the reaction game
*
* \copyright Copyright ©2016
* Department of electrical engineering and information technology, Hochschule Darmstadt - University of applied sciences (h_da). All Rights Reserved.
* Permission to use, copy, modify, and distribute this software and its documentation for educational, and research purposes in the context of non-commercial
* (unless permitted by h_da) and official h_da projects, is hereby granted for enrolled students of h_da, provided that the above copyright notice,
* this paragraph and the following paragraph appear in all copies, modifications, and distributions.
* Contact Prof.Dr.-Ing. Peter Fromm, peter.fromm@h-da.de, Birkenweg 8 64295 Darmstadt - GERMANY for commercial requests.
*
* \warning This software is a PROTOTYPE version and is not designed or intended for use in production, especially not for safety-critical applications!
* The user represents and warrants that it will NOT use or redistribute the Software for such purposes.
* This prototype is for research purposes only. This software is provided "AS IS," without a warranty of any kind.
*/
#include <stdlib.h>
#include <stdio.h>

#include "project.h"
#include "global.h"
#include "seven.h"
#include "led.h"
#include "game.h"
#include "arcadian.h"

volatile uint16_t  timer = 0,button_press_time=0,timer2=0;


ISR(systick_handler)
{ 
    CounterTick(cnt_systick);  
}

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    // Seed the random number generator
    srand((uint32_t)CySysTickGetValue());
    
    //Set systick period to 1 ms. Enable the INT and start it.
    EE_systick_set_period(MILLISECONDS_TO_TICKS(1,BCLK__BUS_CLK__HZ));
    EE_systick_enable_int();

    for(;;)	    
    	StartOS(OSDEFAULTAPPMODE);
    
}

TASK (tsk_init){
    //Init MCAL Drivers
    LED_Init();
    SEVEN_Init();
    //Start UART
    UART_LOG_Start();
    UART_LOG_PutString("Press one of the two buttons to start...");
 
    //Reconfigure ISRs with OS parameters.
    //This line MUST be called after the hardware driver 
    //initialization!
    EE_system_init();
    // Must be started after interrupt reconfiguration
    EE_systick_start();
 
    //Start the alarm with 1ms cycle time
    SetRelAlarm(alrm_timer,1,1);
    ActivateTask(tsk_led);
    
    //Activate LED and Button tasks
    ActivateTask(tsk_button);
    TerminateTask();

}

// Button Task for handling the reaction game
TASK(tsk_button)
{
    EventMaskType ev = 0;
    while(1){
        WaitEvent(ev_Button|ev_Delay_Timeout);
        GetEvent(tsk_button, &ev);
        ClearEvent(ev);
        // Seed the random number generator
        //srand((uint32_t)CySysTickGetValue()); better placed here as user is interacting
        //true rand no generator check
        GAME_Play(ev);
    }
    TerminateTask();  
}

//Time task for timer increment
TASK(tsk_time){
    timer++;
    timer2++;
    TerminateTask();
}

//Task LED for arcadian-glower and fader
TASK(tsk_led){
    LED_fader();  
    LED_glower();
    TerminateTask();
}

// Random Delay Task to set timeout event
TASK(tsk_randomDelay){
    SetEvent(tsk_button,ev_Delay_Timeout);
    TerminateTask();
}

// Button ISR
ISR2(isr_Button)
{
    // Record button press time
    button_press_time=timer;
    // Set event for button task to handle button press
    if ((Button_1_Read() == 1) || (Button_2_Read() == 1)) SetEvent(tsk_button,ev_Button); 
}