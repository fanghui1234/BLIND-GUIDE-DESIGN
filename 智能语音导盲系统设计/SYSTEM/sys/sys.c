#include "sys.h"


//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//系统中断分组设置化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/10
//版本：V1.4
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************  
void NVIC_Configuration(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级

}


/*
************************************************************
*	函数名称：	BootScreen
*
*	函数功能：	OLED屏开机界面显示
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		LED0用于白天/黑夜状态，LED1用于提醒障碍物
************************************************************
*/
void BootScreen(void)
{
	OLED_Clear();
	delay_ms(10);
	OLED_ShowCH(0,0,"智能语音导盲仪");		//智能语音导盲仪
	OLED_ShowCH(0,3,"设计者:辉");       	//设计者：辉
	OLED_ShowCH(0,6,"2022/3/22");			//测试显示中文
	delay_ms(10);
}

/*
************************************************************
*	函数名称：	Hardware_Init
*
*	函数功能：	硬件初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：初始化单片机功能以及外接设备  -->上电后，需要等待一段较长时间才能进行WIFI连接和数据传输通信
************************************************************
*/

void Hardware_Init(void)
{
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置
		
		USART1_Init(115200);//串口1初始化     用于与PC机进行通信
		USART2_Init(9600);  //串口2初始化  	  JQ8400用  RX->接PA2    TX->接PA3
		USART3_Init(115200);//串口3初始化     ESP8266用 RX->接PB10   TX->接PB11
		
		delay_init();		//systick初始化
		Wave_SRD_Init();	//初始化超声波   Trig->PB7  Echo->PB6 用于正前方障碍物检测
//		infrared_INIT1();   //正前方障碍物检测模块初始化  OUT->PA7
		infrared_INIT2();	//左前方障碍物检测模块初始化	OUT->PA6
		infrared_INIT3();	//右前方障碍物检测模块初始化	OUT->PA5
		LIGHT_Init();		//光敏模块初始化             D0->PA4
		LED_Init();			//LED模块初始化   LED0->PB5  LED1->PE5
		BEEP_Init();		//蜂鸣器模块初始				BEEP->PB8
		OLED_Init();		//OLED屏初始化   SCL->PB0	SDA->PB1
		Wave_SRD_Init();	//初始化超声波   Trig->PB7  Echo->PB6     Trig更改为PE7      Echo更改为PE8
		Timer_SRD_Init(9998,7199);//定时器初始化	
		
		esp8266_start_trans();							//esp8266进行初始化
		
		esp8266_send_data("Boot Test",50);
		
//		esp8266_quit_trans();     //退出透传，就是把在串口显示的数据通过WIFI输出到手机端显示	
}
