/**
* \file //arcadian.c
* \author Sree Samanvitha Manoor Vadhoolas
* \date 1.12.2023
*
* \brief declaration of fader and glower related functions
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
#include "project.h"
#include "led.h"
#include "arcadian.h"

/*****************************************************************************/
/* Local pre-processor symbols/macros ('#define')                            */
/*****************************************************************************/

/*****************************************************************************/
/* Global variable definitions (declared in header file with 'extern')       */
/*****************************************************************************/

/*****************************************************************************/
/* Local type definitions ('typedef')                                        */
/*****************************************************************************/

/*****************************************************************************/
/* Local variable definitions ('static')                                     */
/*****************************************************************************/


/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/

/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/
/**
 * Function to fade and cycle through Red Yellow Green LEDS
 * @return RC_SUCCESS if ok
 */
RC_t LED_fader(){
    // Static variables to maintain state between function calls
    static uint8_t intensityR = 0;
    static uint8_t intensityY = 0;
    static uint8_t intensityG = 0;
    
    // Cancel the LED alarm to prevent overlapping calls
    CancelAlarm(alrm_led);
    if (intensityR < 255 && intensityY == 0 && intensityG == 0) {
        // Increase Red
        LED_RYG_Set(intensityR, 0, 255 - intensityR);
        intensityR++;
    } else if (intensityR == 255 && intensityY < 255 && intensityG == 0) {
        // Decrease Red, Increase Yellow
        LED_RYG_Set(255 - intensityY, intensityY, 0);
        intensityY++;
        if(intensityY==255)
            intensityR=0;
    } else if (intensityR==0 && intensityY == 255 && intensityG < 255) {
        // Decrease Green, Increase Green
        LED_RYG_Set(0, 255 - intensityG, intensityG);
        intensityG++;
        if(intensityG == 255)
            intensityY=0;
    } else {
        // Reset intensities
        intensityR = 0;
        intensityY = 0;
        intensityG = 0;
    }
    SetRelAlarm(alrm_led,10,10);
    return RC_SUCCESS;
}

/**
 * Function to glow RGB colors on an LED with a specified delay in glow table
 * @return RC_SUCCESS if ok
 */
RC_t LED_glower(){
    // Static variables to maintain state between function calls
    static uint8_t index = 0; 
    static uint16_t delay = 0;
    
    //fetching the delay from glow table for current index
    GetDelay(index,&delay);
    
    //Glow LEDs if the index value is less than the table size
    if(index<9){
        if(timer2<=delay){
            RGB_Glow(index);
        }
        //increment the table index to next and reset timer
        else{
            index++;
            timer2=0;
        }   
    }
    //Rest index and timer
    else{
        index = 0;
    }
    return RC_SUCCESS;
}