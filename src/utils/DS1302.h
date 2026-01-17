#include "mylib.h"

#define DS1302_SECOND 0x80
#define DS1302_MINUTE 0x82
#define DS1302_HOUR 0x84
#define DS1302_DATE 0x86
#define DS1302_MONTH 0x88
#define DS1302_DAY 0x8a
#define DS1302_YEAR 0x8c
#define DS1302_WP 0x8E


#define DS1302_SCLK P3_6
#define DS1302_IO P3_4
#define DS1302_CE P3_5

extern unsigned char DS1302_Time[7];


void DS1302_Init(void);

void DS1302_WriteByte(u8 Command, u8 Data) __reentrant;

u8 DS1302_ReadByte(u8 Command) __reentrant;

u8 bcd_to_dec(u8 bcd) __reentrant;

u8 dec_to_bcd(u8 dec) __reentrant;

void DS1302_TimeInitSet(void);

void DS1302_SetTime(u8 *Input_Time) __reentrant;

void DS1302_ReadTime(u8 *DS1302_ReadedTime) __reentrant;
