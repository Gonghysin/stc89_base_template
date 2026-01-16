/**
 * @file LCD1602.h
 * @brief LCD1602液晶显示屏驱动库
 * @details 提供LCD1602的初始化和显示功能，支持字符、字符串、数字等多种显示模式
 */

#ifndef __LCD1602_H__
#define __LCD1602_H__

#include <mylib.h>

/** @defgroup LCD1602_Pin_Definitions LCD1602引脚定义
 * @{
 */
#define LCD_DATA P0    /**< LCD数据口 D0~D7 连接到 P0.0-P0.7 */
#define LCD_RS P2_6    /**< 寄存器选择 (0=命令, 1=数据) */
#define LCD_WR P2_5    /**< 读写控制 (0=写, 1=读) */
#define LCD_EN P2_7    /**< 使能信号 (高电平有效) */
/** @} */

/** @defgroup LCD1602_Functions LCD1602功能函数
 * @{
 */

/**
 * @brief LCD延时函数
 * @param t 延时时间参数
 * @note 实际延时时间取决于晶振频率
 */
void LCD_Delay(unsigned int t);

/**
 * @brief 向LCD写入命令
 * @param cmd 命令字节
 * @note 用于LCD初始化和控制，如清屏、光标移动等
 */
void LCD_WriteCommand(unsigned char cmd);

/**
 * @brief 向LCD写入数据
 * @param dat 数据字节
 * @note 用于显示字符，自动处理ASCII码
 */
void LCD_WriteData(unsigned char dat);

/**
 * @brief 初始化LCD1602
 * @details 配置LCD为8位数据接口，2行显示，5x7点阵
 * @note 必须在使用其他LCD函数前调用
 */
void LCD_Init(void);

/**
 * @brief 在指定位置显示单个字符
 * @param row 行号 (1-2)
 * @param col 列号 (1-16)
 * @param c 要显示的字符
 */
void LCD_ShowChar(unsigned char row, unsigned char col, char c);

/**
 * @brief 在指定位置显示字符串
 * @param row 行号 (1-2)
 * @param col 列号 (1-16)
 * @param str 要显示的字符串指针
 * @note 字符串必须以'\0'结尾
 */
void LCD_ShowString(unsigned char row, unsigned char col, char *str);

/// @brief 清除一行
/// @param row 行号（1-2）
void LCD_Clean_row(unsigned char row);

/**
 * @brief 在指定位置显示无符号十进制数
 * @param row 行号 (1-2)
 * @param col 列号 (1-16)
 * @param num 要显示的数字 (0-65535)
 * @param len 显示位数 (1-5)
 * @note 高位补0，如显示123为00123 (len=5)
 */
void LCD_ShowNum(unsigned char row, unsigned char col, unsigned int num, unsigned char len);

/**
 * @brief 在指定位置显示有符号十进制数
 * @param row 行号 (1-2)
 * @param col 列号 (1-16)
 * @param num 要显示的数字 (-32768 ~ 32767)
 * @param len 显示位数，不包括符号位
 * @note 自动显示+/-符号
 */
void LCD_ShowSignedNum(unsigned char row, unsigned char col, int num, unsigned char len);

/**
 * @brief 在指定位置显示十六进制数
 * @param row 行号 (1-2)
 * @param col 列号 (1-16)
 * @param num 要显示的数字 (0-65535)
 * @param len 显示位数 (1-4)
 * @note 使用大写字母A-F表示10-15
 */
void LCD_ShowHexNum(unsigned char row, unsigned char col, unsigned int num, unsigned char len);

/**
 * @brief 在指定位置显示二进制数
 * @param row 行号 (1-2)
 * @param col 列号 (1-16)
 * @param num 要显示的数字 (0-65535)
 * @param len 显示位数 (1-16)
 * @note 高位补0
 */
void LCD_ShowBinNum(unsigned char row, unsigned char col, unsigned int num, unsigned char len);

/** @} */


/// @brief 将正整数转换为字符串
/// @param num 正整数
/// @return 字符串指针
char * tran_num_to_string(signed int num);















#endif