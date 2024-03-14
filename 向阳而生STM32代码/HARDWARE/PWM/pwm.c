#include "pwm.h" 
#include "delay.h"
// 84M / (839 + 1) = 0.1M 故记一个数的时间为10us  定时周期：（1999 + 1） * 10us = 20ms
#define SERVO_TIM_ARR  1999
#define SERVO_TIM_PSC  839

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)
// 占空比配置


void TIM3_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// 配置为PWM模式2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3);
	
	//开启定时器时钟,即内部时钟CK_INT=84M 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period=arr;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= psc;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=0;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/*--------------------输出比较结构体初始化-------------------*/	
	

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//TIM_OCInitStructure.TIM_Channel =  TIM_Channel_4;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 输出通道电平极性配置	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse=0;
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM2在CCR1上的预装载寄存器
 
 	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
 
 	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM2在CCR3上的预装载寄存器
	
 	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM2在CCR4上的预装载寄存器
 
 
  TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 
	// 使能计数器
	TIM_Cmd(TIM3, ENABLE);
}
//void servo_init(void)
//{
//	TIM2_Init(SERVO_TIM_ARR,SERVO_TIM_PSC);
//	//TIM_SetCompare2(TIM2, 1000);
//	servo_angle1(30);
//	servo_angle2(30);
//	servo_angle3(30);
//	servo_angle4(30);
//}
// 
////0.5ms--0°  2.5ms--180°
//void servo_angle2(uint16_t angle)
//{
//  uint16_t pulse;
//  
//  //针对舵机可转角度限辐
//  if(angle <= 5)
//    angle = 5;
//  if(angle >= 175)
//    angle = 175;
//  //将角度值转换为脉冲值  
//  pulse = (uint16_t)(50 + angle * 100/90.0);  //此转换公式需根据pwm的arr及psc配置来做相应变化
//  TIM_SetCompare2(TIM2, pulse);
//}

//  void servo_angle1(uint16_t angle)
//{
//  uint16_t pulse;
//  
//  //针对舵机可转角度限辐
//  if(angle <= 5)
//    angle = 5;
//  if(angle >= 175)
//    angle = 175;
//  //将角度值转换为脉冲值  
//  pulse = (uint16_t)(50 + angle * 100/90.0);  //此转换公式需根据pwm的arr及psc配置来做相应变化
//  TIM_SetCompare1(TIM2, pulse);
//}
//void servo_angle3(uint16_t angle)
//{
//  uint16_t pulse;
//  
//  //针对舵机可转角度限辐
//  if(angle <= 5)
//    angle = 5;
//  if(angle >= 175)
//    angle = 175;
//  //将角度值转换为脉冲值  
//  pulse = (uint16_t)(50 + angle * 100/90.0);  //此转换公式需根据pwm的arr及psc配置来做相应变化
//  TIM_SetCompare3(TIM2, pulse);
//}
//void servo_angle4(uint16_t angle)
//{
//  uint16_t pulse;
//  
//  //针对舵机可转角度限辐
//  if(angle <= 5)
//    angle = 5;
//  if(angle >= 175)
//    angle = 175;
//  //将角度值转换为脉冲值  
//  pulse = (uint16_t)(50 + angle * 100/90.0);  //此转换公式需根据pwm的arr及psc配置来做相应变化
//  TIM_SetCompare4(TIM2, pulse);
//}


