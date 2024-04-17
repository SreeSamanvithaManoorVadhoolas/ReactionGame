
#ifndef SEVEN_H
#define SEVEN_H
    
    
#include "global.h"


typedef enum {
  SEVEN_0 = 0,          /**< Right seven segment display */
  SEVEN_1 = 1,          /**< Left seven segment display */
} SEVEN_id_t;  

#ifdef TOGETHER

class SEVEN
{
public:
#endif /* Together */

RC_t SEVEN_Init();

RC_t SEVEN_Set(SEVEN_id_t sevenId, uint8_t value);


RC_t SEVEN_Get(SEVEN_id_t sevenId, sint8_t* value);

RC_t SEVEN_SetHex(uint8_t value);


RC_t SEVEN_SetDP(SEVEN_id_t sevenId, uint8_t value);

RC_t SEVEN_Clear(SEVEN_id_t sevenId);


#ifdef TOGETHER
//Not visible for compiler, only used for document generation
private:

//Attributes

/**
 * <description>
 */
static type FILE__variable;

//Operations

/**
 * <description>
 * @param <para 1>
 * @return <return description>
 */
static type FILE__function(uint16_t cmd);


};
#endif /* Together */

#endif /* FILE_H */
