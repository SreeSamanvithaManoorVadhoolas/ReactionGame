#include "project.h"
#include "seven.h"

#define SEVEN__A  0b10000000
#define SEVEN__B  0b01000000
#define SEVEN__C  0b00100000
#define SEVEN__D  0b00010000
#define SEVEN__E  0b00001000
#define SEVEN__F  0b00000100
#define SEVEN__G  0b00000010
#define SEVEN__DP 0b00000001


const static uint8_t SEVEN__BitPattern[] = 
{
    0b11111100,     //0
    0b01100000,     //1
    0b11011010,     //2
    0b11110010,     //3
    0b01100110,     //4
    0b10110110,     //5
    0b10111110,     //6
    0b11100000,     //7
    0b11111110,     //8
    0b11110110,     //9
    0b11101110,     //a
    0b00111110,     //b
    0b10011100,     //c
    0b01111010,     //d
    0b10011110,     //e
    0b10001110,     //f
    0b00000001      //decimal point
};
    

static sint8_t SEVEN__latch_value[2] = {-1,-1};
static uint8_t SEVEN__latch_pattern[2] = {0,0};


RC_t SEVEN_Init()
{
    
    SEVEN_Clear(SEVEN_0);
    SEVEN_Clear(SEVEN_1);
    
    return RC_SUCCESS;
}



RC_t SEVEN_Set(SEVEN_id_t sevenId, uint8_t value)
{
    if (value > 0x0F)
    {
        return RC_ERROR_BAD_PARAM;
    }
    
    //Set the selection bit for the display
    SEVEN_SELECT_Write((uint8_t)sevenId);
    
    //Set the bitpattern
    //As we do not have a single port, we have to do it bit by bit
    
    uint8_t pattern = SEVEN__BitPattern[value];
    
    //Store the current pattern in the software latch (needed for the DP function)
    SEVEN__latch_value[(uint8_t)sevenId] = value;
    SEVEN__latch_pattern[(uint8_t)sevenId] = pattern;
    
    SEVEN_reg_Write(pattern);
    return RC_SUCCESS;
    
}

RC_t SEVEN_Get(SEVEN_id_t sevenId, sint8_t* value)
{
    *value = SEVEN__latch_value[(uint8_t)sevenId];
    return RC_SUCCESS;
}

RC_t SEVEN_SetHex(uint8_t value)
{
    SEVEN_Set(SEVEN_1, value / 16);
    SEVEN_Set(SEVEN_0, value % 16);
    
    return RC_SUCCESS;
}

RC_t SEVEN_SetDP(SEVEN_id_t sevenId, uint8_t value)
{
    
    if (value > 1)
    {
        return RC_ERROR_BAD_PARAM;
    }
    
    //Restore the last display pattern
    uint8_t pattern = 0;
    if (SEVEN__latch_value[(uint8_t)sevenId] != -1)
    {
        pattern = SEVEN__latch_pattern[(uint8_t)sevenId];
    }
    
    //Set the selection bit for the display
    SEVEN_SELECT_Write((uint8_t)sevenId);
    
    //Set the port to the pattern currently active
    SEVEN_reg_Write(pattern);
    
    //Set the decimal point
    SEVEN_DP_Write(value);

    return RC_SUCCESS;
}

RC_t SEVEN_Clear(SEVEN_id_t sevenId)
{
    //Set software latch to cleared
    SEVEN__latch_value[(uint8_t)sevenId] = -1;
    
    //Set the selection bit for the display
    SEVEN_SELECT_Write((uint8_t)sevenId);
    
    //Clear every bit
    SEVEN_reg_Write(0);
    
    return RC_SUCCESS;
}