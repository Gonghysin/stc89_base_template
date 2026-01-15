#ifndef __MYLIB_H__
#define __MYLIB_H__


#include <mylib.h>
// #include <intrins.h>


// LCD1602引脚定义
#define LCD_DATA P0    // 数据口 D0~D7

// 使用 P2 的位定义（SDCC自动提供）
#define LCD_RS P2_6    // P2.6
#define LCD_WR P2_5    // P2.5
#define LCD_EN P2_7    // P2.7

// 函数声明

void LCD_Delay(unsigned int t);
void LCD_WriteCommand(unsigned char cmd);
void LCD_WriteData(unsigned char dat);
void LCD_Init(void);
void LCD_ShowChar(unsigned char row, unsigned char col, char c);
void LCD_ShowString(unsigned char row, unsigned char col, char *str);
void LCD_ShowNum(unsigned char row, unsigned char col, unsigned int num, unsigned char len);
void LCD_ShowSignedNum(unsigned char row, unsigned char col, int num, unsigned char len);
void LCD_ShowHexNum(unsigned char row, unsigned char col, unsigned int num, unsigned char len);
void LCD_ShowBinNum(unsigned char row, unsigned char col, unsigned int num, unsigned char len);
















#endif