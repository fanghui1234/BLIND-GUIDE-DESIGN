//说明：本工程代码使用了STM32F1x系列的2022年ST官方更新的标准库
//2022更新的标准库与上一版本的相比，只是优化了Bug，变动没有很大。
//2022ST官方更新的标准库下载网址：https://www.st.com/zh/embedded-software/stsw-stm32054.html
//本工程代码Github网址: https://github.com/fanghui1234/BLIND-GUIDE-DESIGN.git
//作者哔哩哔哩（B站）: 牛马大师兄 https://space.bilibili.com/2142950034
//仅供免费开源参考交流学习使用
//欢迎在B站或Github中交流讨论

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


u8 Weathercount=0;   //用于记录亮度语音播报次数-->解决播报过于频繁，过多长时间进行一次播报白天/黑夜
u8 Strightline=0;    //若长时间没有障碍物，用于安抚盲人心情，提醒一路畅通，继续前行--》比如3分钟没有障碍物则提醒
u8 Whole_time=0;     //用于统计未对白天黑夜播报时间，到一定时间后，对语音播报次数清0，以便达到定时提醒白天黑夜
u8 Disnum=30;		 //超声波报警距离
u8 t=20; 			 //trig引脚高电平持续时间
u8 Distance;		 //超声波测得的距离
u8 buff[64];		 //用于字符串拼接，发送到手机端显示

//extern const u8 BMP1[];
extern const u8 BMP2[];//太阳图标   -->白天
extern const u8 BMP3[];//月亮图标   -->黑夜

int main(void)
{
	
	Hardware_Init();		//硬件模块初始化
			
	BootScreen();			//开机界面显示
	esp8266_send_data("智能语音导盲仪",50);	//提醒：如果需要修改显示的字体内容，必须先在oledfont库文件中进行修改
	esp8266_send_data("设计者：辉",50);
	esp8266_send_data("2022/3/22",50);
	delay_ms(10);
	playsong(1);     		//启动程序-->语音播报：欢迎使用
	delay_s(4);

	while(Wave_SRD_Strat(t))
	{
	}
	
	OLED_Clear();	
	
	while(1)
	{	
		//产生高于10Us的高电平与OLED上显示超声波所测距离		
		Wave_SRD_Strat(t);
		Distance = TIM_GetCounter(TIM3)*340/200.0;
		sprintf((char *)buff,"距离：%d cm",Distance);   //字符串拼接
		esp8266_send_data(buff,50);     //超声波测得的距离发送到手机端
			
		OLED_ShowCH(0,0,"智能语音导盲仪");		//智能语音导盲仪
		OLED_ShowCH(0,6,"距离:");				//显示中文：距离
		OLED_ShowCH(100,6,"cm");				//cm
		OLED_ShowNum(50,6,Distance,6,16);		//显示超声波测量距离
		delay_ms(10);
		
		//光照检测部分PA4-->3/25发现BUG，判断过白天还是黑夜后，后续判断还很频繁    已解决
		if(Weathercount<=1)		//解决判断过于频繁     --->设置比如过了1小时后再次进入提醒一次
		{						//考虑实际，所以设置的是3分钟后  -->真要用1小时后播报，用RTC时钟
			if(Deteclight()==0)       //光敏模块 		     
			{			
				LED0=1;  			  //当前是白昼模式
				OLED_DrawBMP(111,0,127,2,(u8 *)BMP3);   	//太阳图标
				esp8266_send_data("白昼模式",50);	//发送到手机端
				delay_ms(10);
				playsong(6);						//播放白天模式语音				
				delay_s(3);
				
			}else
			{
				LED0=0; //当前是黑夜模式      遮挡灯灭进入黑夜模式模式
				OLED_DrawBMP(111,0,127,2,(u8 *)BMP2);		//月亮图标
				esp8266_send_data("黑夜模式",50);	//发送到手机端
				delay_ms(10);
				playsong(5); 						//播放黑夜模式语音
				delay_s(6);
			
			}
			Weathercount++;
		}else{		     
			Whole_time++;
			delay_s(1);			//3min=180s
			if((Whole_time==60)|(Whole_time==180))							//总体计数到达一定次数和时长后，进行播报白天/黑夜
			{
				Weathercount=0;
			
			}
		
		}
		
			//障碍物检测报警部分
		if(Strightline<=180)     //若长时间没有障碍物（考虑实际演示，设置的是3min），提醒盲人前方一路畅通
		{
		
			if(Distance>0)
			{
				OLED_ShowNum(50,6,Distance,6,16);		//显示超声波测量距离
				delay_ms(10);
				if(Distance<Disnum)                         //检测到障碍物距离小于30cm时报警
				{
					OLED_ShowCH(0,3,"正前方有障碍物");	//正前方有障碍物
					OLED_ShowNum(50,6,Distance,6,16);	//显示超声波测量距离
					esp8266_send_data("正前方有障碍物",50);	//发送到手机端
					LED1=0;								//开警示灯
//					BEEP=1;
					delay_ms(10);
					playsong(2);      					//语音提醒正前方有障碍物				
					delay_s(4);
					LED1=1;								//关灯	
//					BEEP=0;
					Strightline=0;						//有障碍物，计数清0
					
				}else{
					OLED_ShowCH(0,3,"                ");//清除正前方有障碍物字体
					Strightline++;						//没有障碍物，计数
					delay_s(1);
				}
			}
			if(Detect2()==0)					   		//左前方障碍物检测   PA6
			{
				LED1=0;
//				BEEP=0;
				OLED_ShowCH(0,3,"左前方有障碍物");
				esp8266_send_data("左前方有障碍物",50);	//发送到手机端
				delay_ms(10);
				playsong(3);				
				delay_s(4);
				LED1=1;
//				BEEP=1;
				Strightline=0;						//有障碍物，计数清0
				
			}else if(Detect3()==0)					//右前方障碍物检测   PA5
			{
				LED1=0;
//				BEEP=0;
				OLED_ShowCH(0,3,"右前方有障碍物");
				esp8266_send_data("右前方有障碍物",50);	//发送到手机端
				delay_ms(10);
				playsong(4);
				delay_s(4);
				LED1=1;	
//				BEEP=0;
				Strightline=0;						//有障碍物，计数清0
				
			}else{
				Strightline++;						//没有障碍物，计数
				OLED_ShowCH(0,3,"                ");//清除显示有障碍物字体
				OLED_ShowNum(50,6,Distance,6,16);	//显示超声波测量距离
				delay_s(1);
			}
		}
		else{
			
			OLED_ShowCH(0,3,"前方一路畅通");	//清除显示有障碍物字体
			OLED_ShowNum(50,6,Distance,6,16);	//显示超声波测量距离
			esp8266_send_data("前方一路畅通",50);	//发送到手机端
			playsong(7);						//播放语音提醒前方一路畅通
			delay_s(4);
			Strightline=0;
			
		}
		
	}
}
