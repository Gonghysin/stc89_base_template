/*===========================================================================
 * seg_table.h - 数码管段码表
 * 
 * 描述：共阴极数码管的段码定义
 * 作者：hysin
 * 日期：2026-01-13
 *===========================================================================*/

#ifndef SEG_TABLE_H
#define SEG_TABLE_H

// 兼容不同编译器的代码存储声明
#ifndef __code
#define __code
#endif

// 数字0-9的段码（共阴极）
// 段码顺序：dp g f e d c b a
extern __code unsigned char SEG_CODE[10];

// 特殊字符段码
#define SEG_BLANK 0x00  // 熄灭
#define SEG_MINUS 0x40  // 负号 "-"
#define SEG_E     0x79  // 字母 "E"
#define SEG_H     0x76  // 字母 "H"
#define SEG_L     0x38  // 字母 "L"
#define SEG_P     0x73  // 字母 "P"
#define SEG_DOT   0x80  // 小数点（可与数字相或）

#endif // SEG_TABLE_H