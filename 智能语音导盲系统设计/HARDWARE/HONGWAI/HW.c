#include "HW.h"

void infrared_INIT1(void)   //��ǰ���ϰ�����
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	   

}

void infrared_INIT2(void)   //��ǰ���ϰ�����
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	   

}
 
void infrared_INIT3(void)   //��ǰ���ϰ�����
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	 
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	  
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	   

}

