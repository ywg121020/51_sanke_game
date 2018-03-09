#include <config.h>

sbit PIN_RXD = P3^0; //接收引脚定义
sbit PIN_TXD = P3^1; //发送引脚定义


void UART_Init(void)
{
     SCON  = 0x40;                   //串口方式1
     PCON  = 0;                     //SMOD=0
     REN  = 1;                               //允许接收
     TMOD = 0x20;                            //定时器1定时方式2
     TR1  = 1;                                   //启动定时器
     ES  = 1;                                                              //UART中断
     EA  = 1;                                                              //中断使能
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