/**
* \file led.c
* \author <author-name>
* \date <date>
*
* \brief <Symbolic File name>
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
static LED_ONOFF_t LED__state[LED_ALL] = {LED_OFF, LED_OFF, LED_OFF}; /**< On/Off State of the LED, TRUE is on */

static const RG_Glow_t Glow_table[9] = {
    {255, 0, 0, 500},
    {0, 255, 0, 500},
    {0, 0, 255, 500},
    {0, 0, 0, 100},
    {255, 255, 255, 100},
    {0, 0, 0, 100},
    {255, 255, 255, 100},
    {0, 0, 0, 100},
    {255, 255, 255, 100}
};
/*****************************************************************************/
/* Local function prototypes ('static')                                      */
/*****************************************************************************/


/*****************************************************************************/
/* Function implementation - global ('extern') and local ('static')          */
/*****************************************************************************/

/**
 * Initialises the LED's and turns them off
 * @return RC_SUCCESS if function was executed as exepected, other error code in case of error
 */
RC_t LED_Init()
{
    RC_t result = RC_SUCCESS;
    
    for (LED_id_t i = LED_RED; i < LED_ALL; i++)
    {
        LED__state[i] = OFF;
        
        RC_t clearResult;
        clearResult = LED_Set(i, LED_OFF);
        
        if (clearResult != RC_SUCCESS)
        {
            //memorize error
            result = clearResult;
        }
    }
    
    //Init RGB
    PWM_red_Start();
    PWM_green_Start();
    PWM_yellow_Start();
    
    return result;
}

/**
 * Sets the required LED
 * @param LED_t ledId - Identifier for the LED, ALL will change all LED's
 * @param LED_ONOFF_t ledOnOff - Trun the led on or off
 * @return RC_SUCCESS if function was executed as exepected, other error code in case of error
 */
RC_t LED_Set(LED_id_t ledId, LED_ONOFF_t ledOnOff)
{
    //set individual LED
    switch (ledId)
    {
        case LED_RED : 
            LED_red_Write(ledOnOff); 
            LED__state[ledId] = ledOnOff; 
            break;
        
        case LED_YELLOW : 
            LED_yellow_Write(ledOnOff); 
            LED__state[ledId] = ledOnOff;
            break;
            
        case LED_GREEN : 
            LED_green_Write(ledOnOff); 
            LED__state[ledId] = ledOnOff;
            break;
           
        case LED_ALL : 
            for (LED_id_t i = LED_RED; i < LED_ALL; i++)
            {
                LED_Set(i, ledOnOff);
            }
            break;
        default: return RC_ERROR_BAD_PARAM;
    }
    
    return RC_SUCCESS;
}

/**
 * Set the RYG LED
 * @param uint8_t red - brightness 
 * @param uint8_t yellow - brightness 
 * @param uint8_t green - brightness 
 * @return RC_SUCCESS if function was executed as exepected, other error code in case of error
 */
RC_t LED_RYG_Set(uint8_t red, uint8_t yellow, uint8_t green)
{
    PWM_red_WriteCompare(red);
    PWM_yellow_WriteCompare(yellow);
    PWM_green_WriteCompare(green);
 
    return RC_SUCCESS;
}

/**
 * Set the RGB LED with values from table
 * @param uint8_t index - for table reference
 * @return RC_SUCCESS if function was executed as exepected, other error code in case of error
 */
RC_t RGB_Glow(uint8_t index)
{
    RGB_R_Write(Glow_table[index].red);
    RGB_G_Write(Glow_table[index].green);
    RGB_B_Write(Glow_table[index].blue);
    return RC_SUCCESS;
}

/**
 * Get the delay value from table for given index
 * @param uint8_t index - for table reference
 * @param uint8_t *delay - Pointer to a variable where the delay value will be stored
 * @return RC_SUCCESS if function was executed as exepected, other error code in case of error
 */
RC_t GetDelay(uint8_t index,uint16_t *delay){
    *delay = Glow_table[index].duration;
    return RC_SUCCESS;
}


