#include <config.h>
/**
  ******************************************************************************
  * 文件名称：snake
  * 作者：无木成林
  * 版本: V1.0
  * 编写日期: 2017/3/28
  * 功能: game核心程序
  ******************************************************************************
  * 说明:
  * gui实现，功能实现，包含中断处理，按键处理
  ******************************************************************************
**/

sbit LED = P1^0; //1熄灭，0显示
struct snake{
	uchar X[25];
	uchar Y[25];
	uchar Node;
	uchar Direction;
	uchar Blood;
	uchar Life;
	uchar Score;
	uchar Rank;
	uchar Dif;
	
}Snake;
  
struct barrier{
   uchar X[52];
   uchar Y[52];
   uchar Num;
}Barrier;

struct food{
	uchar X;
	uchar Y;
	uchar Eat;
}Food;

struct buf{
	char X_Move;
	char Y_Move;
}Buf;

struct life{
	uchar X[6];
	uchar Y[6];
	uchar Num;
}Life;

uchar code Lin_Word[]={0x00,0xDC,0x98,0x98,0xFC,0xDF,0xFC,0xFE,0xBB,0xDC,0x98,0x00,0x00,0x01,0x01,0x07,0x07,0x01,0x03,0x07,0x0D,0x01,0x01,0x00};/*"林",0*/
uchar code A_Word[]={0x04,0x04,0x0C,0x0A,0x0A,0x1E,0x12,0x33};/*"A",0*/
uchar code B_Word[]={0x0F,0x12,0x12,0x0E,0x12,0x12,0x12,0x0F};/*"B",0*//*"B",0*/

uchar code S_Word[]={0x1E,0x11,0x01,0x06,0x08,0x10,0x11,0x0F};/*"S",0*/
uchar code T_Word[]={0x1F,0x15,0x04,0x04,0x04,0x04,0x04,0x0E};/*"T",0*/
uchar code R_Word[]={0x0F,0x12,0x12,0x0E,0x0A,0x12,0x12,0x37};/*"R",0*/
uchar code C_Word[]={0x1E,0x11,0x01,0x01,0x01,0x01,0x11,0x0E};/*"C",0*/
uchar code K_Word[]={0x37,0x12,0x0A,0x06,0x0A,0x12,0x12,0x37};/*"K",0*/


uchar Most_Score=0;
uchar  Step = 1;
uchar Timer0_100ms=0;
uchar Timer1_1s=0;
uchar Timer1_s=0;
uchar Flag_Refresh=0;
uchar Flag_Medicine=0;
uchar Flag_1s=0;
uchar Speed=20;
uchar Key_Buffer=4;
/*********************************
初始化定时器
**********************************/
void InitTimer(void)
{
	TMOD = 0x11; 
	TH0=0x7D;	  //定时器0   20ms中断 
	TL0=0xCB;
	TH1 = 0;	  //定时器1   40ms中断	
	TL1 = 0; 	
	ET0=1;
	ET1 = 1;	
	TR0=1;		
	TR1=1;	
	PT0=1;
	EA=1;  
}


void Barrier_Refresh()
{	 
	 Barrier.X[Barrier.Num]=Random()*79;
	 Barrier.Y[Barrier.Num]=Random()*62+1;
	 while(Barrier.X[Barrier.Num] ==Food.X)
	 {Barrier.X[Barrier.Num]=Random()*79;   }	 //Random()的范围在0-1之间	 
	 while(Barrier.Y[Barrier.Num] ==Food.Y)
	 {Barrier.Y[Barrier.Num]=Random()*62+1;	} 	 	
	 Lcd_PutPixel( Barrier.X[Barrier.Num], Barrier.Y[Barrier.Num],0);
	 Barrier.Num++;
}
/************************************
定时器0 中断服务函数
按键刷新
************************************/
void T0_time()interrupt 1
{	
	TH0=0x7D;	  //定时器0   20ms中断 
	TL0=0xCB;
	Timer0_100ms++;
	if(Timer0_100ms == 3)
	{
		Timer0_100ms = 0;
		switch(Key_Scan())
		{
		   case 1 :	{	if(Snake.Direction == 2 	) {break;}else{Key_Buffer =  1;	}	break;	}
		   case 2 : {	if(Snake.Direction == 1   	) {break;}else{Key_Buffer =  2; 	}	break;	}
		   case 3 : {	if(Snake.Direction == 4		) {break;}else{Key_Buffer =  3; 	}	break;	}
		   case 4 : {	if(Snake.Direction == 3 	) {break;}else{Key_Buffer =  4;	}	break;	}
		   case 5 : Key_Buffer =  5;break;	
		   case 6 : Key_Buffer =  6;break;	
		   default : break;
		}		
	}
}
/************************************
定时器1 中断服务函数
游戏刷新定时
************************************/
void T1_timer()interrupt 3
{
	TH1 = 0;	  //定时器1   40ms中断	
	TL1 = 0;
	Timer1_1s++;
	if(Timer1_1s==25)
	{
	 	Timer1_1s=0;
		Flag_1s =1;
	
	}	
	Timer1_s ++;
	if(Timer1_s == Speed)
	{
	 	Timer1_s =0;
	  	Flag_Refresh = 1;
	}
}
/***********************************
函数作用：游戏结束
***********************************/
void GameOver()
{
	 Lcd_Reset();
	 Lcd_Clear(1);
	 Lcd_WriteStr(2,0,"游戏结束");
	 if(Snake.Score>Most_Score)
	 {

		   Most_Score= Snake.Score;

	 }
	 Lcd_WriteStr(1,2,"当前分：");
    DisPlayNums	(80,40,Snake.Score);
	Lcd_WriteStr(1,3,"最高分：");
    DisPlayNums	(80,56,Most_Score);
	 while(1)
	 {
	 	 Step = 1;
		 if(Key_Buffer == 5)
		 {			  			   
			   break;
		 }
	 
	 }	   
}
/***********************************
蛇头蛇身接触，结束游戏
***********************************/
uchar Kill_Self(char n,uchar Temp1[],uchar Temp2[])
{
	uchar i;
	uchar Flag=0;
	for(i = 1;i < Snake.Node - 1;i++)
	{
		if(Temp1[i] == Temp1[0]+n && Temp2[i] == Temp2[0])
		{
			return 0;
		}											 	  							 
	}
	return 1;
}
/****************************************
食物刷新
****************************************/
void Food_Refresh()
{
	 uchar i;
	 while(1)
	 {
	 	 
		 Food.X=Random()*79;	 //Random()的范围在0-1之间	 
		 while(Food.X % 3 != 0)
		 {
			Food.X=Random()*79;	 //Random()的范围在0-1之间		
		 }
		 Food.Y=Random()*62+1;
		 while((Food.Y-1)%3 !=0)
		 {
			 Food.Y=Random()*62+1;
		 }
		 for(i=0;i<Snake.Node;i++)
		 {
		 	if(Food.X+1 == Snake.X[i]&&Food.Y == Snake.Y[i])
			{
			  break;
			}
	 	 }
		 if(i == Snake.Node)
		 {
			break;
		 }
	 }
	 Lcd_Rectangle(Food.X,Food.Y-1,Food.X+2,Food.Y+1,0);
}

void Life_Refresh()
{
	 Life.X[Life.Num]=Random()*79;
	 Life.Y[Life.Num]=Random()*63;
	 LED=~LED;
	//Random()的范围在0-1之间	 
	while((Life.X[Life.Num]-1) % 3 != 0)
	{
		Life.X[Life.Num]=Random()*79;	 //Random()的范围在0-1之间		
	}
	while((Life.Y[Life.Num]-1) % 3 !=0)
	{
		Life.Y[Life.Num]=Random()*63;
	 }
	ShiZi_Display(Life.X[Life.Num],Life.Y[Life.Num],0);
	Life.Num++; 
}


/*******************************
数据刷新
*******************************/
void Data_Refresh()
{
	 	DisPlayNums(110,5,Snake.Score);
		DisPlayNums(110,18,Snake.Rank);
		DisPlayNums(110,31,Snake.Dif);
		DisPlayNums(110,44,Snake.Blood);
}
/********************************
难度设置
*********************************/
void Dif_Refresh()
{
	Snake.Dif=Snake.Score/5;
	if(Snake.Rank <2)
	{
		if(Snake.Dif < 1)	{Speed =5;}
		if(Snake.Dif >=1 )	{Speed =1;}
	}
	else
	{
	    if(Snake.Dif < 3)	{Speed =5;}
		if(Snake.Dif >=3)	{Speed =1;}		
	}
}




void  Game2_Start()
{
	uchar i;
	if(Snake.Score >=10)
	{
		if(Flag_Medicine == 0)
		{
			for(i=0;i <= Barrier.Num;i++)
			{
				if(Barrier.Y[i]>=(Snake.Y[0]-1)&&Barrier.Y[i]<=(Snake.Y[0]+1)&&Barrier.X[i]>=(Snake.X[0]-1)&&Barrier.X[i]<=(Snake.X[0]+1))				
				{
					 Barrier.Y[i]=86;
					 Barrier.X[i]=80;
					 Snake.Blood--;
				 	 break;
				}
			}
		}
		Snake.Rank=2;
	}
}









	


void Game1_Start()
{
	  uchar i;	  
	  Flag_Refresh = 0;
	  Food_Refresh();
	  Food.Eat=1;
	  Snake.X[0]=16;
	  Snake.Y[0]=1;
	  Snake.X[1]=13;
	  Snake.Y[1]=1;
	  Snake.X[2]=10;
	  Snake.Y[2]=1;
	  Snake.X[3]=7;
	  Snake.Y[3]=1;
	  Snake.X[4]=4;
	  Snake.Y[4]=1;
	  Snake.Node = 5;
	  Snake.Direction = 4;
	  Snake.Life  = 1;
	  Snake.Blood  = 3;
	  Snake.Score = 0;
	  Snake.Rank  = 1;
	  Snake.Dif   = 1;
	  Barrier.Num=0;
	  Life.Num=0;	  	  
	  Data_Refresh();
	  Lcd_Rectangle(Snake.X[0]-1,Snake.Y[0]-1,Snake.X[0]+1,Snake.Y[0]+1,0);
      Lcd_Rectangle(Snake.X[1]-1,Snake.Y[1]-1,Snake.X[1]+1,Snake.Y[1]+1,0);
	  Lcd_Rectangle(Snake.X[2]-1,Snake.Y[2]-1,Snake.X[2]+1,Snake.Y[2]+1,0);
      Lcd_Rectangle(Snake.X[3]-1,Snake.Y[3]-1,Snake.X[3]+1,Snake.Y[3]+1,0);
	  Lcd_Rectangle(Snake.X[4]-1,Snake.Y[4]-1,Snake.X[4]+1,Snake.Y[4]+1,0);
	  Delaynms(1000);
	  Key_Buffer = 4;
	  while(Step == 2)
	  {
		  if(Flag_Refresh == 1 )
		  {  							 
				Flag_Refresh=0;			
			  	if(	Snake.Score == 20)
				{
					Lcd_Reset();
	 			    Lcd_Clear(1);
					Lcd_WriteStr(0,0,"！不敢相信赢了！");
					Lcd_WriteStr(1,1,"！你太牛了！");
					Lcd_WriteStr(0,2,"！英雄留下名字！");
					 while(1)
					 {
						Step = 1;
						if(Key_Buffer == 5)
						{										   
							break;
						}						 
					}
				}
				
				
				
				if(Key_Buffer == 6)
				{
					 while(1)
					 {
						if(Key_Buffer == 5)
						{															
							Flag_Refresh=0;		
							break;
						}
					 }
					 Flag_Refresh=0;
				}

				if(Key_Buffer!=5 && Key_Buffer!=6)
				{
					Snake.Direction=Key_Buffer;
				}												
				//判断食物是否还存在				
				Dif_Refresh();				
														
			 	//判断是否接触
				switch(Snake.Direction)
				{
					case 1 : {	if(Snake.Y[0] == 1 || Snake.Y[0] == 0 ||Kill_Self(-3,Snake.Y,Snake.X) == 0){GameOver();break;}							
								else{Buf.X_Move=0;Buf.Y_Move=-3;}	break;	}
					case 2 : {	if(Snake.Y[0] == 61|| Snake.Y[0] == 64 ||Kill_Self( 3,Snake.Y,Snake.X) == 0){GameOver();break;}							
								else{Buf.X_Move=0;Buf.Y_Move=3;}	break; 	}
					case 3 : {	if(Snake.X[0] == 1 || Snake.X[0] == 0||Kill_Self(-3,Snake.X,Snake.Y) == 0){GameOver();break;}					
								else{Buf.X_Move=-3;Buf.Y_Move=0;}	break; 	}
					case 4 : {	if(Snake.X[0] == 79|| Snake.X[0] == 82||Kill_Self( 3,Snake.X,Snake.Y) == 0){GameOver();break;}								
								else {Buf.X_Move=3;Buf.Y_Move=0;}	break;	}
					default : break;
				}
				if(Snake.Blood == 0)
				{
					  GameOver();
				}												
				//判断是否吃到食物
				if(Snake.X[0]+Buf.X_Move == Food.X + 1  &&  Snake.Y[0]+Buf.Y_Move == Food.Y)
				{
					Food.Eat = 0;
					Snake.Node++;						
					for(i=Snake.Node - 1;i>0;i--)
					{
						Snake.X[i] = Snake.X[i-1];
						Snake.Y[i] = Snake.Y[i-1];
					}	
					Snake.X[0]+=Buf.X_Move;					
					Snake.Y[0]+=Buf.Y_Move;																			
					Snake.Score++;												
				}
				else
				{
					Lcd_Rectangle(Snake.X[Snake.Node-1]-1,Snake.Y[Snake.Node-1]-1,Snake.X[Snake.Node-1]+1,Snake.Y[Snake.Node-1]+1,1);
					for(i=Snake.Node - 1;i>0;i--)
					{
						Snake.X[i] = Snake.X[i-1];
						Snake.Y[i] = Snake.Y[i-1];
					}	
					Snake.X[0]+=Buf.X_Move;
					Snake.Y[0]+=Buf.Y_Move;			
					Lcd_Rectangle(Snake.X[0]-1,Snake.Y[0]-1,Snake.X[0]+1,Snake.Y[0]+1,0);
			    }
				if(Food.Eat == 0)
				{		 
					Food.Eat = 1;
					Food_Refresh();
				}
				//判断是否吃到补血剂
				for(i=0;i<=Life.Num;i++)
				{
					if(	Snake.X[0] == Life.X[i] && Snake.Y[0] == Life.Y[i])
					{
						Snake.Blood++;
						Flag_Medicine = 1;						
						Lcd_PutPixel(Life.X[i],Life.Y[i],1); 						
						Life.X[i]=86;
				    	Life.Y[i]=80;												
						break;
					}
					else
					{
					    Flag_Medicine=0;
					}														
				}
								
				Game2_Start();			
				Data_Refresh();			
		  	} 			
		  	if(Flag_1s == 1)
			{
		   		Flag_1s=0;
		   		if(Snake.Rank == 2 )
		  	 	{
				
					if(Barrier.Num<52)
					{							
						Barrier_Refresh();
					}
					if( Life.Num<=4 &&Barrier.Num %10==0)
					{
						Life_Refresh();	
					}										
		  		}
			}		  
	 }
}

void Start()
{
	 Lcd_Clear(1);//清屏
	 Lcd_WriteStr(2,1,"开始游戏");
	 DisPlay(114,52,Lin_Word);	 
	 Lcd_PutPixel(25,45,0); 	
	 Lcd_PutPixel(25,47,0); 	
	 DisPlayNum(18,40,A_Word);
	 DisPlayNum(28,40,S_Word);
	 DisPlayNum(34,40,T_Word);
	 DisPlayNum(40,40,A_Word);
	 DisPlayNum(46,40,R_Word);
	 DisPlayNum(52,40,T_Word);
	 
	  DisPlayNum(72,40,B_Word);
	  Lcd_PutPixel(79,45,0); 	
	 Lcd_PutPixel(79,47,0);
	 DisPlayNum(82,40,B_Word);
	 DisPlayNum(88,40,A_Word);
	 DisPlayNum(96,40,C_Word);
	 DisPlayNum(102,40,K_Word);


	 while(1)
	 {
		  if(Key_Buffer == 6)
		  {
		  		Step=2;		 		 	
		  		break;
		  }
	 }
	 
}


 void Snake_Game()
{
	switch(Step)
	{
		 case 1 :Start(); 
		 		Lcd_Reset();
				Lcd_Clear(1);//清屏	
				Delaynms(1000);
				InitInterFace();
				InitRandom(TL0);
				break;
		 case 2 :Game1_Start();
		 		Lcd_Reset();
				Lcd_Clear(1);//清屏	
				Delaynms(1000);	break;		
	}  	 
}




