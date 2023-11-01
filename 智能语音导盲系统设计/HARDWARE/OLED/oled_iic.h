#ifndef __OLED_IIC_H
#define	__OLED_IIC_H

#include "sys.h"
#include <inttypes.h>

#define OLED_ADDRESS	0x78 //Ĭ��0x78

//����IIC�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�������뼴������ı�SCL��SDA������
#define OLED_SCL PBout(0)  //SCL����
#define OLED_SDA PBout(1)  //SDA����

#define OLED_SCL_GPIO_PORT	GPIOB			/* GPIO�˿� */
#define OLED_SCL_RCC 	      RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define OLED_SCL_PIN		    GPIO_Pin_0			/* ���ӵ�SCLʱ���ߵ�GPIO */

#define OLED_SDA_GPIO_PORT	GPIOB			/* GPIO�˿� */
#define OLED_SDA_RCC 	      RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
#define OLED_SDA_PIN		    GPIO_Pin_1			/* ���ӵ�SDA�����ߵ�GPIO */


#define OLED_IIC_SDA_READ()  GPIO_ReadInputDataBit(OLED_SDA_GPIO_PORT, OLED_SDA_PIN)	/* ��SDA����״̬ */

extern unsigned int HZ;

unsigned int GB16_NUM(void);
void IIC_GPIO_Config(void);
void IIC_Start(void);
void IIC_Stop(void);
uint8_t IIC_WaitAck(void);
void Write_IIC_Byte(uint8_t _ucByte);
void Write_IIC_Command(u8 IIC_Command);
void OLED_Fill(u8 fill_Data);
void OLED_Set_Pos(u8 x, u8 y);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_Clear(void);
void OLED_ShowChar(u8 x, u8 y, u8 chr);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 mode);
void OLED_ShowCH(u8 x, u8 y, u8 *chs);
void OLED_Init(void);
void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, u8 BMP[]);
#endif
