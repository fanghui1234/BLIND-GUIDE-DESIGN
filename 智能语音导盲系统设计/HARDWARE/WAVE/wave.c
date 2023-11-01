#include "wave.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"


#define Trig GPIO_Pin_7//������������pb7
#define Echo GPIO_Pin_6//��������������pb6

void Wave_SRD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	//��ʼ��������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin=Trig;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin=Echo;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//�ⲿ�ж������ŵ����ã��ж���˿�6��ӳ��
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
	//�ⲿ�ж�����
	EXTI_InitStruct.EXTI_Line=EXTI_Line6;
	EXTI_InitStruct.EXTI_LineCmd= ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	//�ж����ȼ�����
	NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);
	
	
}

//�ⲿ�жϷ�����
void EXTI9_5_IRQHandler(void)
{
	delay_us(10);
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET)//Echo��⵽�ߵ�ƽ
	{
		TIM_SetCounter(TIM3,0);//������ʱ��
		TIM_Cmd(TIM3,ENABLE);//ʹ�ܶ�ʱ��
		while(GPIO_ReadInputDataBit(GPIOB,Echo));//�ȴ�Echo���ֵ͵�ƽ
		
		TIM_Cmd(TIM3,DISABLE);//�رն�ʱ��
		
	EXTI_ClearITPendingBit(EXTI_Line6);//����жϱ�־
  }
}

//��������������
//Trig����һ��������10us�ĸߵ�ƽ�ź�
u8 Wave_SRD_Strat(u8 time)
{
	if(time>10)
	{
	GPIO_SetBits(GPIOB,Trig);//����TrigΪ�ߵ�ƽ���
	delay_us(time);
	GPIO_ResetBits(GPIOB,Trig);
	return 0;//��������10us�ߵ�ƽ������0
	}
	else return 1;//δ��������10us�ߵ�ƽ����1
}

