/*
5.��Ŀ��ɣ�180�ȶ��ģ��
6.��Ŀ���ܣ���������ת���Ƕȣ����ڷ��ض��ת���Ƕ�����
7.��Ҫԭ������ο�SG90��������ֲ�
	�����
		VCC--5V
		GND--GND
		�ź���--PA1
	������
		��--PB9
		��--PB8
180�Ȳ�����λ������Ƕȿɿأ��ϵ縴λ��0�㡣û��������λ���ֶ�����360��ת��

��ɫ��GND

��ɫ��VCC��4.8V-6V��

��ɫ���źŶˣ�3.3V��5V��IO�ڼ���������
*/
//		TIM_SetCompare1(TIM2, 250);//pa5
//		delay_ms(2000);
//		TIM_SetCompare2(TIM2, 100);//pa1
//		delay_ms(2000);
//		TIM_SetCompare3(TIM2, 50);//pa2
//		delay_ms(2000);
//		TIM_SetCompare4(TIM2, 150);//pa3



#include "sys.h"
#include "delay.h"
#include "pwm.h"
#include "usart.h"
#include "usart2.h"	
#include "key.h"
#include "syn6288.h"


/**************оƬ��������*********************/
u8 SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //ֹͣ�ϳ�
u8 SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //��ͣ�ϳ�
u8 SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //�ָ��ϳ�
u8 SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //״̬��ѯ
u8 SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //����POWER DOWN ״̬����

u32 tpingg(u32 tmp)
{
        u32 num=1;
        if(tmp==0) return 1;
        while(tmp--)
                num*=10;
              return num;
} 
int main(void)
{
	u8 t,t2;
	u8 len,len2;		
	u16 times=0,times2=0;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);      //��ʼ����ʱ����
	TIM3_Init(1999,839);
	uart_init(115200);		//��ʼ������
	usart2_init(9600);
	KEY_Init();
	
	while(1)
	{
		
if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
			
			
			
			printf("\r\n�����͵���ϢΪ:\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);         //�򴮿�1��������
				
				
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			printf("\r\n\r\n");//���뻻��
			                  if(len==5)
                        {
                            TIM_SetCompare1(TIM3, 250);//pb4
														delay_ms(1000);
														TIM_SetCompare1(TIM3, 100);//pb4   
														SYN_FrameInfo(2, "[v10][m0][t5]�ɻ�����");
														delay_ms(1500);
														delay_ms(1500);
//														delay_ms(1500);
//														delay_ms(1500);
//														delay_ms(1500);
//														delay_ms(1500);
                                 
                        }
												 else if(len==6)
													{
														TIM_SetCompare2(TIM3, 250);//pb5
														delay_ms(1000);
														TIM_SetCompare2(TIM3, 100);
														SYN_FrameInfo(2, "[v10][m0][t5]��������");
														delay_ms(1500);
														delay_ms(1500);
													}	
													else if(len==8)
													{
														TIM_SetCompare3(TIM3, 250);//pb0
														delay_ms(1000);
														TIM_SetCompare3(TIM3, 100);
														SYN_FrameInfo(2, "[v10][m0][t5]�к�����");
														delay_ms(1500);
														delay_ms(1500);
													}	
                          else if(len==4)
													{
														TIM_SetCompare4(TIM3, 250);//pb1
														delay_ms(1000);
														TIM_SetCompare4(TIM3, 100);
														SYN_FrameInfo(2, "[v10][m0][t5]��������");
														delay_ms(1500);
														delay_ms(1500);
													}											
														else
															{
																times++;
																if(times%5000==0)
																{
																	printf("\r\nALIENTEK ̽����STM32F407������ ����ʵ��\r\n");
																	printf("����ԭ��@ALIENTEK\r\n\r\n\r\n");
																}
																if(times%200==0)printf("����������,�Իس�������\r\n");  
													//			if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
																delay_ms(10);   
															 }
																while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				                       USART_RX_STA=0;	
															 
														 }	
		

//		if(USART2_RX_STA&0x8000)
//		{					   
//			len2=USART2_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			
//			
//			
//			printf("\r\n�����͵���ϢΪ:\r\n");
//			for(t2=0;t2<len2;t2++)
//			{
//				USART_SendData(USART2, USART2_RX_BUF[t]);         //�򴮿�1��������
//				
//				
//				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
//			}
//			printf("\r\n\r\n");//���뻻��
//			                  if(USART2_RX_BUF[0]=='5')
//                        {
//                           
//													SYN_FrameInfo(2, "[v7][m1][t5]�ɻ�������");
//                                 
//                        }
//												 else if(USART2_RX_BUF[0]=='6')
//													{
//														SYN_FrameInfo(2, "[v7][m1][t5]�к�����");
//													}	
//													else if(USART2_RX_BUF[0]=='7')
//													{
//														SYN_FrameInfo(2, "[v7][m1][t5]��������");
//													}	
//                          else if(USART2_RX_BUF[0]=='8')
//													{
//														SYN_FrameInfo(2, "[v7][m1][t5]��������");
//													}														
//       															
//       		
//		
//														else
//															{
//																times2++;
//																if(times2%5000==0)
//																{
//																	printf("\r\nALIENTEK ̽����STM32F407������ ����ʵ��\r\n");
//																	printf("����ԭ��@ALIENTEK\r\n\r\n\r\n");
//																}
//																if(times2%200==0)printf("����������,�Իس�������\r\n");  
//													//			if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
//																delay_ms(10);   
//															 }
//		USART2_RX_STA=0;			
//														 }	
//		
//		
		
		
}

}



