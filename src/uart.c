#include <config.h>

sbit PIN_RXD = P3^0; //�������Ŷ���
sbit PIN_TXD = P3^1; //�������Ŷ���


void UART_Init(void)
{
     SCON  = 0x40;                   //���ڷ�ʽ1
     PCON  = 0;                     //SMOD=0
     REN  = 1;                               //�������
     TMOD = 0x20;                            //��ʱ��1��ʱ��ʽ2
     TR1  = 1;                                   //������ʱ��
     ES  = 1;                                                              //UART�ж�
     EA  = 1;                                                              //�ж�ʹ��
}



void UART_Receive_Int() interrupt 4
{       
     if(RI == 1)
     {
         RI = 0;           
         if(  index < 1 )
         {
              Receive[index] = SBUF;
              index++;
		 }
		 else
		 {
               index  = 0;
         }                
     }       
}