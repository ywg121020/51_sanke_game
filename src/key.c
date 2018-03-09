#include <config.h>
/**
  ******************************************************************************
  * �ļ����ƣ�key
  * ���ߣ���ľ����
  * �汾: V1.0
  * ��д����: 2017/3/28
  * ����: ��������
  ******************************************************************************
  * ˵��:
  * ������ʼ����ɨ��
  ******************************************************************************
**/

void Key_Init(void)
{
	P1 = 0xFC;
}

unsigned char  Key_Scan(void)
{
	  #if 	KEY_LONG_EN > 0 	  
		  unsigned char Temp,i;	
		  Temp = P1;	
		  for(i=2; i<8; i++)
		  {
			 if((Temp & (1 << i)) == 0)
			 {
			 	break;			
			 }
		  }
		  Delaynms(100); 
		  Temp = P1;
		  if((Temp & (1 << i)) == 1)
		  {
			  i=0;
		  }
		  return i-1;
	 #endif

	 #if KEY_LONG_EN <= 0 
	 	  static unsigned char KeySample = 0;
		  unsigned char Temp,i;	
		  Temp = P1;
		  for(i=2; i<9; i++)
		  {
			 if((Temp & (1 << i)) == 0)
			 {
				 break;
			 }
		  }
		  if(i == 8)
		  {	
		  		KeySample = 1;
		    	return 0; 
	 	  }
		  Delaynms(100); 
		  Temp = P1;
		  if((Temp & (1 << i)) == 1)
		  {
			  	i=0;
		  }
		  if(KeySample == 1)
		  {
			   KeySample = 0;
			   return i-1;
		  }
		  else
		    return 0;
	   #endif

}



