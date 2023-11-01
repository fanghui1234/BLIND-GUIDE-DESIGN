#include "esp8266.h"
#include "string.h"
#include "usart.h"
#include "usart.h"
#include "stm32f10x.h"
#include "sys.h" 
#include "delay.h"

//ESP8266ģ���PC����͸��ģʽ
void esp8266_start_trans(void)
{
	//���ù���ģʽ 1��stationģʽ   2��APģʽ  3������ AP+stationģʽ
	esp8266_send_cmd("AT+CWMODE=1","OK",50);
	
	//��Wifiģ������������
	esp8266_send_cmd("AT+RST","ready",20);
	
	delay_ms(1000);         //��ʱ3S�ȴ������ɹ�
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	
	//��ģ���������Լ���·��
	while(esp8266_send_cmd("AT+CWJAP=\"CIRCLE\",\"10086100\"","WIFI GOT IP",600));
	
	//=0����·����ģʽ     =1����·����ģʽ
	esp8266_send_cmd("AT+CIPMUX=0","OK",20);
	
	//����TCP����  ������ֱ������ Ҫ���ӵ�ID��0~4   ��������  Զ�̷�����IP��ַ   Զ�̷������˿ں�
	while(esp8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.120.2\",8080","CONNECT",200));
	
	//�Ƿ���͸��ģʽ  0����ʾ�ر� 1����ʾ����͸��
	esp8266_send_cmd("AT+CIPMODE=1","OK",200);
	
	//͸��ģʽ�� ��ʼ�������ݵ�ָ�� ���ָ��֮��Ϳ���ֱ�ӷ�������
	esp8266_send_cmd("AT+CIPSEND","OK",50);
}

//ESP8266�˳�͸��ģʽ   ����ֵ:0,�˳��ɹ�;1,�˳�ʧ��
//����wifiģ�飬ͨ����wifiģ����������3��+��ÿ��+��֮�� ����10ms,������Ϊ���������η���+��
u8 esp8266_quit_trans(void)
{
	u8 result=1;
	u3_printf("+++");
	delay_ms(1000);					//�ȴ�500ms̫�� Ҫ1000ms�ſ����˳�
	result=esp8266_send_cmd("AT","OK",20);//�˳�͸���ж�.
	if(result)
		printf("quit_trans failed!");
	else
		printf("quit_trans success!");
	return result;
}


//��ESP8266��������
//cmd:���͵������ַ���;ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��;waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����);1,����ʧ��
u8 esp8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(esp8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 


//ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����;����,�ڴ�Ӧ������λ��(str��λ��)
u8* esp8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}


//��ESP8266��������
//cmd:���͵������ַ���;waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:�������ݺ󣬷������ķ�����֤��
u8* esp8266_send_data(u8 *cmd,u16 waittime)
{
	char temp[5];
	char *ack=temp;
	USART3_RX_STA=0;
	u3_printf("%s",cmd);	//��������
	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
				ack=(char*)USART3_RX_BUF;
				printf("ack:%s\r\n",(u8*)ack);
				USART3_RX_STA=0;
				break;//�õ���Ч���� 
			} 
		}
	}
	return (u8*)ack;
} 
