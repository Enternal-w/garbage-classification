/*
5.项目组成：180度舵机模块
6.项目功能：按键调节转动角度，串口返回舵机转动角度数据
7.主要原理：具体参考SG90舵机数据手册
	舵机：
		VCC--5V
		GND--GND
		信号线--PA1
	按键：
		加--PB9
		减--PB8
180度不带限位舵机：角度可控，上电复位到0°。没有物理限位，手动可以360旋转。

棕色：GND

红色：VCC（4.8V-6V）

橘色：信号端（3.3V或5V的IO口即可驱动）
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


/**************芯片设置命令*********************/
u8 SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //停止合成
u8 SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //暂停合成
u8 SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //恢复合成
u8 SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //状态查询
u8 SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //进入POWER DOWN 状态命令

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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);      //初始化延时函数
	TIM3_Init(1999,839);
	uart_init(115200);		//初始化串口
	usart2_init(9600);
	KEY_Init();
	
	while(1)
	{
		
if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			
			
			
			printf("\r\n您发送的消息为:\r\n");
			for(t=0;t<len;t++)
			{
				USART_SendData(USART1, USART_RX_BUF[t]);         //向串口1发送数据
				
				
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			printf("\r\n\r\n");//插入换行
			                  if(len==5)
                        {
                            TIM_SetCompare1(TIM3, 250);//pb4
														delay_ms(1000);
														TIM_SetCompare1(TIM3, 100);//pb4   
														SYN_FrameInfo(2, "[v10][m0][t5]可回收物");
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
														SYN_FrameInfo(2, "[v10][m0][t5]厨余垃圾");
														delay_ms(1500);
														delay_ms(1500);
													}	
													else if(len==8)
													{
														TIM_SetCompare3(TIM3, 250);//pb0
														delay_ms(1000);
														TIM_SetCompare3(TIM3, 100);
														SYN_FrameInfo(2, "[v10][m0][t5]有害垃圾");
														delay_ms(1500);
														delay_ms(1500);
													}	
                          else if(len==4)
													{
														TIM_SetCompare4(TIM3, 250);//pb1
														delay_ms(1000);
														TIM_SetCompare4(TIM3, 100);
														SYN_FrameInfo(2, "[v10][m0][t5]其他垃圾");
														delay_ms(1500);
														delay_ms(1500);
													}											
														else
															{
																times++;
																if(times%5000==0)
																{
																	printf("\r\nALIENTEK 探索者STM32F407开发板 串口实验\r\n");
																	printf("正点原子@ALIENTEK\r\n\r\n\r\n");
																}
																if(times%200==0)printf("请输入数据,以回车键结束\r\n");  
													//			if(times%30==0)LED0=!LED0;//闪烁LED,提示系统正在运行.
																delay_ms(10);   
															 }
																while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
				                       USART_RX_STA=0;	
															 
														 }	
		

//		if(USART2_RX_STA&0x8000)
//		{					   
//			len2=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
//			
//			
//			
//			printf("\r\n您发送的消息为:\r\n");
//			for(t2=0;t2<len2;t2++)
//			{
//				USART_SendData(USART2, USART2_RX_BUF[t]);         //向串口1发送数据
//				
//				
//				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
//			}
//			printf("\r\n\r\n");//插入换行
//			                  if(USART2_RX_BUF[0]=='5')
//                        {
//                           
//													SYN_FrameInfo(2, "[v7][m1][t5]可回收垃圾");
//                                 
//                        }
//												 else if(USART2_RX_BUF[0]=='6')
//													{
//														SYN_FrameInfo(2, "[v7][m1][t5]有害垃圾");
//													}	
//													else if(USART2_RX_BUF[0]=='7')
//													{
//														SYN_FrameInfo(2, "[v7][m1][t5]厨余垃圾");
//													}	
//                          else if(USART2_RX_BUF[0]=='8')
//													{
//														SYN_FrameInfo(2, "[v7][m1][t5]其他垃圾");
//													}														
//       															
//       		
//		
//														else
//															{
//																times2++;
//																if(times2%5000==0)
//																{
//																	printf("\r\nALIENTEK 探索者STM32F407开发板 串口实验\r\n");
//																	printf("正点原子@ALIENTEK\r\n\r\n\r\n");
//																}
//																if(times2%200==0)printf("请输入数据,以回车键结束\r\n");  
//													//			if(times%30==0)LED0=!LED0;//闪烁LED,提示系统正在运行.
//																delay_ms(10);   
//															 }
//		USART2_RX_STA=0;			
//														 }	
//		
//		
		
		
}

}



