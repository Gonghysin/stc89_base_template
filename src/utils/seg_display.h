/*===========================================================================
 * seg_display.h - 数码管显示控制
 * 
 * 描述：提供数码管显示的高级接口
 * 作者：hysin
 * 日期：2026-01-13
 *===========================================================================*/

#ifndef SEG_DISPLAY_H
#define SEG_DISPLAY_H

#include "seg_table.h"

// 数码管位数（根据你的硬件）
#define SEG_NUM 8

/*---------------------------------------------------------------------------
 * 数据结构
 *---------------------------------------------------------------------------*/
// 显示缓冲区（存储要显示的内容）
extern unsigned char display_buffer[SEG_NUM];

/*---------------------------------------------------------------------------
 * 基础函数
 *---------------------------------------------------------------------------*/

/**
 * @brief 初始化数码管显示
 * @note 清空显示缓冲区
 */
void seg_init(void);

/**
 * @brief 在指定位置显示指定数字
 * @param pos 位置（0-7，对应LED1-LED8）
 * @param num 要显示的数字（0-9）或特殊字符段码
 * @note 立即更新显示缓冲区
 */
void seg_set_digit(unsigned char pos, unsigned char num);

/**
 * @brief 在指定位置显示数字（带小数点）
 * @param pos 位置（0-7）
 * @param num 要显示的数字（0-9）
 * @param dot 是否显示小数点（1=显示，0=不显示）
 */
void seg_set_digit_dot(unsigned char pos, unsigned char num, unsigned char dot);

/**
 * @brief 清空指定位置的显示
 * @param pos 位置（0-7）
 */
void seg_clear_digit(unsigned char pos);

/**
 * @brief 清空所有显示
 */
void seg_clear_all(void);

/**
 * @brief 数码管扫描显示（需在主循环或定时器中调用）
 * @note 动态扫描所有数码管，刷新率取决于调用频率
 */
void seg_scan(void);

/*---------------------------------------------------------------------------
 * 高级函数
 *---------------------------------------------------------------------------*/

/**
 * @brief 显示一个整数
 * @param num 要显示的整数（0-99999999）
 * @param start_pos 起始位置（0-7，从这一位开始向右显示）
 * @note 自动处理位数，超出范围不显示
 */
void seg_show_number(unsigned long num, unsigned char start_pos);

/**
 * @brief 显示带符号整数
 * @param num 要显示的整数（-9999999 ~ 99999999）
 * @param start_pos 起始位置
 */
void seg_show_signed(long num, unsigned char start_pos);

#endif // SEG_DISPLAY_H