#ifndef __LCD12864_H
#define __LCD12864_H

//color   1ÎªµãÁÁ

#define  LcdData P2
void Lcd_PutPixel(unsigned char x,unsigned char y,unsigned char Color);
void Lcd_Reset(void); 
void Lcd_WriteStr(unsigned char x,unsigned char y,unsigned char *Str);
void Lcd_Clear(unsigned char Mode);	void Lcd_HoriLine(unsigned char x,unsigned char y,unsigned char Length,unsigned char Color);
void Lcd_VertLine(unsigned char x,unsigned char y,unsigned char Length,unsigned char Color); 
void Lcd_HoriLine(unsigned char x,unsigned char y,unsigned char Length,unsigned char Color);
void Lcd_Rectangle(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char Color);
void ShiZi_Display(unsigned char x,unsigned char y, unsigned char color);


#endif