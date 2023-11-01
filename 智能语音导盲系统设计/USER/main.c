//˵���������̴���ʹ����STM32F1xϵ�е�2022��ST�ٷ����µı�׼��
//2022���µı�׼������һ�汾����ȣ�ֻ���Ż���Bug���䶯û�кܴ�
//2022ST�ٷ����µı�׼��������ַ��https://www.st.com/zh/embedded-software/stsw-stm32054.html
//�����̴���Github��ַ: https://github.com/fanghui1234/BLIND-GUIDE-DESIGN.git
//��������������Bվ��: ţ���ʦ�� https://space.bilibili.com/2142950034
//������ѿ�Դ�ο�����ѧϰʹ��
//��ӭ��Bվ��Github�н�������

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdio.h>
#include <string.h>
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "sys.h"
#include "jq8400.h"
#include "esp8266.h"
#include "HW.h"
#include "oled_iic.h"
#include "wave.h"
#include "led.h"
#include "light.h"
#include "beep.h"


u8 Weathercount=0;   //���ڼ�¼����������������-->�����������Ƶ�������೤ʱ�����һ�β�������/��ҹ
u8 Strightline=0;    //����ʱ��û���ϰ�����ڰ���ä�����飬����һ·��ͨ������ǰ��--������3����û���ϰ���������
u8 Whole_time=0;     //����ͳ��δ�԰����ҹ����ʱ�䣬��һ��ʱ��󣬶���������������0���Ա�ﵽ��ʱ���Ѱ����ҹ
u8 Disnum=30;		 //��������������
u8 t=20; 			 //trig���Ÿߵ�ƽ����ʱ��
u8 Distance;		 //��������õľ���
u8 buff[64];		 //�����ַ���ƴ�ӣ����͵��ֻ�����ʾ

//extern const u8 BMP1[];
extern const u8 BMP2[];//̫��ͼ��   -->����
extern const u8 BMP3[];//����ͼ��   -->��ҹ

int main(void)
{
	
	Hardware_Init();		//Ӳ��ģ���ʼ��
			
	BootScreen();			//����������ʾ
	esp8266_send_data("����������ä��",50);	//���ѣ������Ҫ�޸���ʾ���������ݣ���������oledfont���ļ��н����޸�
	esp8266_send_data("����ߣ���",50);
	esp8266_send_data("2022/3/22",50);
	delay_ms(10);
	playsong(1);     		//��������-->������������ӭʹ��
	delay_s(4);

	while(Wave_SRD_Strat(t))
	{
	}
	
	OLED_Clear();	
	
	while(1)
	{	
		//��������10Us�ĸߵ�ƽ��OLED����ʾ�������������		
		Wave_SRD_Strat(t);
		Distance = TIM_GetCounter(TIM3)*340/200.0;
		sprintf((char *)buff,"���룺%d cm",Distance);   //�ַ���ƴ��
		esp8266_send_data(buff,50);     //��������õľ��뷢�͵��ֻ���
			
		OLED_ShowCH(0,0,"����������ä��");		//����������ä��
		OLED_ShowCH(0,6,"����:");				//��ʾ���ģ�����
		OLED_ShowCH(100,6,"cm");				//cm
		OLED_ShowNum(50,6,Distance,6,16);		//��ʾ��������������
		delay_ms(10);
		
		//���ռ�ⲿ��PA4-->3/25����BUG���жϹ����컹�Ǻ�ҹ�󣬺����жϻ���Ƶ��    �ѽ��
		if(Weathercount<=1)		//����жϹ���Ƶ��     --->���ñ������1Сʱ���ٴν�������һ��
		{						//����ʵ�ʣ��������õ���3���Ӻ�  -->��Ҫ��1Сʱ�󲥱�����RTCʱ��
			if(Deteclight()==0)       //����ģ�� 		     
			{			
				LED0=1;  			  //��ǰ�ǰ���ģʽ
				OLED_DrawBMP(111,0,127,2,(u8 *)BMP3);   	//̫��ͼ��
				esp8266_send_data("����ģʽ",50);	//���͵��ֻ���
				delay_ms(10);
				playsong(6);						//���Ű���ģʽ����				
				delay_s(3);
				
			}else
			{
				LED0=0; //��ǰ�Ǻ�ҹģʽ      �ڵ���������ҹģʽģʽ
				OLED_DrawBMP(111,0,127,2,(u8 *)BMP2);		//����ͼ��
				esp8266_send_data("��ҹģʽ",50);	//���͵��ֻ���
				delay_ms(10);
				playsong(5); 						//���ź�ҹģʽ����
				delay_s(6);
			
			}
			Weathercount++;
		}else{		     
			Whole_time++;
			delay_s(1);			//3min=180s
			if((Whole_time==60)|(Whole_time==180))							//�����������һ��������ʱ���󣬽��в�������/��ҹ
			{
				Weathercount=0;
			
			}
		
		}
		
			//�ϰ����ⱨ������
		if(Strightline<=180)     //����ʱ��û���ϰ������ʵ����ʾ�����õ���3min��������ä��ǰ��һ·��ͨ
		{
		
			if(Distance>0)
			{
				OLED_ShowNum(50,6,Distance,6,16);		//��ʾ��������������
				delay_ms(10);
				if(Distance<Disnum)                         //��⵽�ϰ������С��30cmʱ����
				{
					OLED_ShowCH(0,3,"��ǰ�����ϰ���");	//��ǰ�����ϰ���
					OLED_ShowNum(50,6,Distance,6,16);	//��ʾ��������������
					esp8266_send_data("��ǰ�����ϰ���",50);	//���͵��ֻ���
					LED1=0;								//����ʾ��
//					BEEP=1;
					delay_ms(10);
					playsong(2);      					//����������ǰ�����ϰ���				
					delay_s(4);
					LED1=1;								//�ص�	
//					BEEP=0;
					Strightline=0;						//���ϰ��������0
					
				}else{
					OLED_ShowCH(0,3,"                ");//�����ǰ�����ϰ�������
					Strightline++;						//û���ϰ������
					delay_s(1);
				}
			}
			if(Detect2()==0)					   		//��ǰ���ϰ�����   PA6
			{
				LED1=0;
//				BEEP=0;
				OLED_ShowCH(0,3,"��ǰ�����ϰ���");
				esp8266_send_data("��ǰ�����ϰ���",50);	//���͵��ֻ���
				delay_ms(10);
				playsong(3);				
				delay_s(4);
				LED1=1;
//				BEEP=1;
				Strightline=0;						//���ϰ��������0
				
			}else if(Detect3()==0)					//��ǰ���ϰ�����   PA5
			{
				LED1=0;
//				BEEP=0;
				OLED_ShowCH(0,3,"��ǰ�����ϰ���");
				esp8266_send_data("��ǰ�����ϰ���",50);	//���͵��ֻ���
				delay_ms(10);
				playsong(4);
				delay_s(4);
				LED1=1;	
//				BEEP=0;
				Strightline=0;						//���ϰ��������0
				
			}else{
				Strightline++;						//û���ϰ������
				OLED_ShowCH(0,3,"                ");//�����ʾ���ϰ�������
				OLED_ShowNum(50,6,Distance,6,16);	//��ʾ��������������
				delay_s(1);
			}
		}
		else{
			
			OLED_ShowCH(0,3,"ǰ��һ·��ͨ");	//�����ʾ���ϰ�������
			OLED_ShowNum(50,6,Distance,6,16);	//��ʾ��������������
			esp8266_send_data("ǰ��һ·��ͨ",50);	//���͵��ֻ���
			playsong(7);						//������������ǰ��һ·��ͨ
			delay_s(4);
			Strightline=0;
			
		}
		
	}
}
