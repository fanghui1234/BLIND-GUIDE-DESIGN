#ifndef __HW_H
#define __HW_H
#include "sys.h"
#define Detect1() GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)   //��ǰ���ϰ�����
#define Detect2() GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)	//��ǰ���ϰ�����
#define Detect3() GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)	//��ǰ���ϰ�����
void infrared_INIT1(void);
void infrared_INIT2(void);
void infrared_INIT3(void);

#endif
