#include "pwm.h" 
#include "delay.h"
// 84M / (839 + 1) = 0.1M �ʼ�һ������ʱ��Ϊ10us  ��ʱ���ڣ���1999 + 1�� * 10us = 20ms
#define SERVO_TIM_ARR  1999
#define SERVO_TIM_PSC  839

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)
// ռ�ձ�����


void TIM3_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// ����ΪPWMģʽ2
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3);
	
	//������ʱ��ʱ��,���ڲ�ʱ��CK_INT=84M 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period=arr;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= psc;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=0;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/*--------------------����ȽϽṹ���ʼ��-------------------*/	
	

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//TIM_OCInitStructure.TIM_Channel =  TIM_Channel_4;
	// ���ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// ���ͨ����ƽ��������	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse=0;
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ���
 
 	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
 
 	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR3�ϵ�Ԥװ�ؼĴ���
	
 	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR4�ϵ�Ԥװ�ؼĴ���
 
 
  TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ�� 
	// ʹ�ܼ�����
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
////0.5ms--0��  2.5ms--180��
//void servo_angle2(uint16_t angle)
//{
//  uint16_t pulse;
//  
//  //��Զ����ת�Ƕ��޷�
//  if(angle <= 5)
//    angle = 5;
//  if(angle >= 175)
//    angle = 175;
//  //���Ƕ�ֵת��Ϊ����ֵ  
//  pulse = (uint16_t)(50 + angle * 100/90.0);  //��ת����ʽ�����pwm��arr��psc����������Ӧ�仯
//  TIM_SetCompare2(TIM2, pulse);
//}

//  void servo_angle1(uint16_t angle)
//{
//  uint16_t pulse;
//  
//  //��Զ����ת�Ƕ��޷�
//  if(angle <= 5)
//    angle = 5;
//  if(angle >= 175)
//    angle = 175;
//  //���Ƕ�ֵת��Ϊ����ֵ  
//  pulse = (uint16_t)(50 + angle * 100/90.0);  //��ת����ʽ�����pwm��arr��psc����������Ӧ�仯
//  TIM_SetCompare1(TIM2, pulse);
//}
//void servo_angle3(uint16_t angle)
//{
//  uint16_t pulse;
//  
//  //��Զ����ת�Ƕ��޷�
//  if(angle <= 5)
//    angle = 5;
//  if(angle >= 175)
//    angle = 175;
//  //���Ƕ�ֵת��Ϊ����ֵ  
//  pulse = (uint16_t)(50 + angle * 100/90.0);  //��ת����ʽ�����pwm��arr��psc����������Ӧ�仯
//  TIM_SetCompare3(TIM2, pulse);
//}
//void servo_angle4(uint16_t angle)
//{
//  uint16_t pulse;
//  
//  //��Զ����ת�Ƕ��޷�
//  if(angle <= 5)
//    angle = 5;
//  if(angle >= 175)
//    angle = 175;
//  //���Ƕ�ֵת��Ϊ����ֵ  
//  pulse = (uint16_t)(50 + angle * 100/90.0);  //��ת����ʽ�����pwm��arr��psc����������Ӧ�仯
//  TIM_SetCompare4(TIM2, pulse);
//}


