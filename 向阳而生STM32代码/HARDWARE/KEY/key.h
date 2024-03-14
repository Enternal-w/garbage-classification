#ifndef __KEY_H
#define __KEY_H
#include "sys.h"


#define KEY1 PBin(9)
#define KEY2 PBin(8)

//

#define KEY1_PORT_CLK  RCC_AHB1Periph_GPIOB
#define KEY1_PORT			 GPIOB
#define KEY1_PORT_PIN  GPIO_Pin_9

#define KEY2_PORT_CLK  RCC_AHB1Periph_GPIOB
#define KEY2_PORT			 GPIOB
#define KEY2_PORT_PIN  GPIO_Pin_8

extern u8 key_num;


void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8 mode);//按键扫描函数
#endif
