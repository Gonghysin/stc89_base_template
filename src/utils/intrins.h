/* intrins.h - Keil C51 兼容头文件 */
#ifndef __INTRINS_H__
#define __INTRINS_H__

/* 常用函数声明 */
extern void _nop_ (void);
extern void _push_ (unsigned char _sfr);
extern void _pop_ (unsigned char _sfr);
extern unsigned char _crol_ (unsigned char val, unsigned char n);
extern unsigned char _cror_ (unsigned char val, unsigned char n);
extern unsigned char _irol_ (unsigned int val, unsigned char n);
extern unsigned char _iror_ (unsigned int val, unsigned char n);
extern unsigned long _lrol_ (unsigned long val, unsigned char n);
extern unsigned long _lror_ (unsigned long val, unsigned char n);
extern void _testbit_ (unsigned char val);

/* 如果使用 SDCC 编译器，可以添加以下实现来通过编译 */
#if defined(__SDCC)
    // SDCC 的内联汇编实现 NOP
    #define _nop_() __asm__("nop")__endasm
#endif

#endif
