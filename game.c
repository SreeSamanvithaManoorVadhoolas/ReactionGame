/**
* \file game.c
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

/*****************************************************************************/
/* Include files                                                             */
/*****************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "global.h"
#include "project.h"
#include "seven.h"
#include "game.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/
#define BUFFER_SIZE 50
/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/
typedef enum {
    GAME__ISIDLE,
    GAME_ISWAITINGRANDOMTIME,
    GAME_ISWAITINGBUTTON} GAME__State_t;

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/
static GAME__State_t GAME__State = GAME__ISIDLE;
char buffer[BUFFER_SIZE];
static uint8_t randomValue = 0;

/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/
/**
 * Starts a random timer for the Reaction game between 1-3s
 */
static void GAME__StartRandomTimer();

/**
 * Displays a random pattern for the Reaction game between 1 or 2 on Seven segment
 */
static void GAME__ShowRandomPattern();

/**
 * Sets an timer with a 1-second delay
 */
static void GAME__Start1sTimer();
/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 * Initiates and manages the Reaction game
 * @param ev - the event triggering the game state transition 
 * @return RC_SUCCESS if ok
 */
RC_t GAME_Play(GAME_event_t ev)
{
    static uint16_t start_time = 0;
    static uint8_t round = 1;
    switch (GAME__State)
    {
        case GAME__ISIDLE :
            if (GAME_BUTTON == ev)
            {
                UART_LOG_PutString("\nReaction Game Round ");
                sprintf(buffer, "%d\n", round);
                UART_LOG_PutString(buffer);
                GAME__StartRandomTimer();
                GAME__State = GAME_ISWAITINGRANDOMTIME;
            }
                
            break;
            
        case GAME_ISWAITINGRANDOMTIME:
            if (GAME_DELAY_TIMEOUT == ev)
            {
                CancelAlarm(alrm_randomDelay);
                GAME__ShowRandomPattern();
                //getting start time for reaction calculation
                start_time = timer;
                GAME__Start1sTimer();
                GAME__State = GAME_ISWAITINGBUTTON;
            }
            
            break;
            
        case GAME_ISWAITINGBUTTON:
            round++;
            static uint8_t score = 0;
            static uint16_t totalTime = 0;//game object
            uint16_t reaction_time = 0,end_time = 0;
            //condition if the received event is button press
            if (GAME_BUTTON == ev)
            {
                //getting end time for reaction calculation
                end_time = button_press_time;
                if ((Button_1_Read()== 1 && randomValue == 2)||(Button_2_Read()== 1 && randomValue == 1) ){
                    score++;
                    //reaction time calculation for correct button press
                    reaction_time = end_time - start_time; 
                    UART_LOG_PutString("\nGreat - correct button pressed! \nReaction time ms: ");
                    //updating total time
                    totalTime+=reaction_time;
                    sprintf(buffer, "%d\n", reaction_time);
                    UART_LOG_PutString(buffer);
                    }
                else{
                    UART_LOG_PutString("\nWrong button pressed!\n");
                    }                    
            
            }
            //condition when user input is slower than 1s
            else if (GAME_DELAY_TIMEOUT == ev){
                UART_LOG_PutString("\nToo slow!\n");
            }
            
            SEVEN_Clear(0);
            SEVEN_Clear(1);
            GAME__State = GAME__ISIDLE;
            CancelAlarm(alrm_randomDelay);
            //scenario when the number of rounds is higher than 10
            if(round>10){
                if(score>0){
                    sprintf(buffer, "\nScore: %d Total time: %d Average time: %d\n",score,totalTime,(totalTime / score));
                    UART_LOG_PutString(buffer);
                    UART_LOG_PutString("\nNew Game");
                    }
                else{
                    UART_LOG_PutString("\nScore = 0\n\nNew Game");
                    }
                totalTime = 0;
                score = 0;
                round = 1;
            }
            UART_LOG_PutString("\nNext round - Press one of the two buttons to start...");
    }
    return RC_SUCCESS;
}

/**
 * Starts a random timer for the Reaction game between 1-3s
 */
static void GAME__StartRandomTimer(){
    uint16_t randomDelay = 0;
    randomDelay = rand() % 2001 + 1000;
    SetRelAlarm(alrm_randomDelay,randomDelay,0);
}

/**
 * Displays a random pattern for the Reaction game between 1 or 2 on Seven segment
 */
static void GAME__ShowRandomPattern(){
    randomValue = rand() % 2 + 1;
    SEVEN_Set(SEVEN_0,randomValue);
    SEVEN_Set(SEVEN_1,randomValue);
}

/**
 * Sets an timer with a 1-second delay
 */
static void GAME__Start1sTimer(){
    SetRelAlarm(alrm_randomDelay,1000,0);
}