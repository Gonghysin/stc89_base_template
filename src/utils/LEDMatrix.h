

#ifndef __LEDMatrix__

/// @brief 初始化点阵屏
/// @param  
void LEDMatrix_Init(void);

void LEDMatrix_SendByte(unsigned char dat);

void LEDMatrix_ShowColumn(unsigned char Column, unsigned char Data);

/// @brief 滚动显示LED点阵图像数组
/// @param image 图像数据数组指针
/// @param len 数组长度
/// @param scroll_delay_ms 滚动延时（毫秒），每次滚动一列的延迟时间
void LEDMatrix_ScrollDisplay(unsigned char *image, unsigned char len, unsigned int scroll_delay_ms);

#pragma once
#endif