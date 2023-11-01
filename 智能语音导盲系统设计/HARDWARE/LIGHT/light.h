#ifndef __LIGHT_H
#define __LIGHT_H	 
#include "sys.h"
 
 
#define PA4 PAout(4)	
#define Deteclight() GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)

void LIGHT_Init(void);
 
 				    
#endif
