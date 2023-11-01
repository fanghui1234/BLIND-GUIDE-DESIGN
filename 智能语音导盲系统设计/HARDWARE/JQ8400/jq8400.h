#ifndef __JQ8400_H
#define	__JQ8400_H


#include "stm32f10x.h"

/*串口2总线*/

/*串口2GPIO*/
// RX
#define USART2_TX_PORT    	    GPIOA			              	/* GPIO端口 */
#define USART2_TX_CLK 	        RCC_APB2Periph_GPIOA			/* GPIO端口时钟 */
#define USART2_TX_PIN		        GPIO_Pin_2			        /* 连接到SCL时钟线的GPIO */

// TX
#define USART2_RX_PORT    	    GPIOA			              
#define USART2_RX_CLK 	        RCC_APB2Periph_GPIOA		
#define USART2_RX_PIN		        GPIO_Pin_3			        

// BUSY
//#define JQ8400_BUSY    	    GPIOC			                /* GPIO端口 */
//#define JQ8400_BUSY_CLK 	  RCC_APB1Periph_GPIOA				/* GPIO端口时钟 */
//#define JQ8400_BUSY_PIN		  GPIO_Pin_3			        /* 连接到SCL时钟线的GPIO */


void Usart2_Send8bit( USART_TypeDef * pUSARTx, uint8_t eight);
void USART2_Init(u32 bound);
void Usart2_Send32bit( USART_TypeDef * pUSARTx, uint32_t ch);
void asong_stop(void);
void playsong(int i);

#endif /* __JQ8400_H */

