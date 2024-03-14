#include "key.h"
#include "sys.h"
#include "delay.h"

u8 key_num;

//������ʼ������
void KEY_Init(void) //IO��ʼ��
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(KEY1_PORT_CLK|KEY2_PORT_CLK,ENABLE);//ʹ��PORTA,PORTCʱ��

  GPIO_InitStructure.GPIO_Pin  = KEY1_PORT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(KEY1_PORT, &GPIO_InitStructure);//��ʼ��GPIO

  GPIO_InitStructure.GPIO_Pin  = KEY2_PORT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(KEY2_PORT, &GPIO_InitStructure);//��ʼ��GPIO

}


//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
u8 KEY_Scan(u8 mode)
{
  static u8 key_up=1;//�������ɿ���־
  if(mode)key_up=1;  //֧������
  if(key_up&&(KEY1==0||KEY2==0))
  {
    delay_ms(5);//ȥ����
    key_up=0;
    if(KEY1==0)return 1;
    else if(KEY2==0)return 2;
  }
  else if(KEY1==1&&KEY2==1)key_up=1;
  return 0;// �ް�������
}

