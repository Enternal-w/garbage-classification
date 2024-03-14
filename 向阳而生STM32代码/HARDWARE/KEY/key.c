#include "key.h"
#include "sys.h"
#include "delay.h"

u8 key_num;

//按键初始化函数
void KEY_Init(void) //IO初始化
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(KEY1_PORT_CLK|KEY2_PORT_CLK,ENABLE);//使能PORTA,PORTC时钟

  GPIO_InitStructure.GPIO_Pin  = KEY1_PORT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(KEY1_PORT, &GPIO_InitStructure);//初始化GPIO

  GPIO_InitStructure.GPIO_Pin  = KEY2_PORT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(KEY2_PORT, &GPIO_InitStructure);//初始化GPIO

}


//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
u8 KEY_Scan(u8 mode)
{
  static u8 key_up=1;//按键按松开标志
  if(mode)key_up=1;  //支持连按
  if(key_up&&(KEY1==0||KEY2==0))
  {
    delay_ms(5);//去抖动
    key_up=0;
    if(KEY1==0)return 1;
    else if(KEY2==0)return 2;
  }
  else if(KEY1==1&&KEY2==1)key_up=1;
  return 0;// 无按键按下
}

