#include "DS1302.h"
#include <8052.h>
#include "mylib.h"

u8 DS1302_Time[] = {26, 1, 1, 0, 0, 0, 4}; // 26年1月1日00:00:00 星期4

void DS1302_Init(void) __reentrant
{
    DS1302_SCLK = 0;
    DS1302_CE = 0;
}

/// @brief 
/// @param Command 指令
/// @param Data 写入的数据
void DS1302_WriteByte(u8 Command,u8 Data) __reentrant
{
    
    u8 i;
    DS1302_CE = 1;

    // 发送命令字节（低位先入，上升沿采样）
    for(i = 0; i < 8; i++)
    {
        DS1302_SCLK = 0;          // ✅ 先确保SCLK为低
        DS1302_IO = (Command & (0x01<<i)) ? 1 : 0;
        _nop_();
        DS1302_SCLK = 1;          // ✅ 上升沿，DS1302采样数据
        _nop_();
    }

    // 发送数据字节（低位先入，上升沿采样）
    for(i = 0; i < 8; i++)
    {
        DS1302_SCLK = 0;          // ✅ 先确保SCLK为低
        DS1302_IO = (Data & (0x01<<i)) ? 1 : 0;
        _nop_();
        DS1302_SCLK = 1;          // ✅ 上升沿，DS1302采样数据
        _nop_();
    }

    DS1302_CE = 0;
    DS1302_SCLK = 0;
}

/// @brief 
/// @param Command 指令
/// @return 
u8 DS1302_ReadByte(u8 Command) __reentrant
{
    u8 i, Data = 0;
    DS1302_CE = 1;
    Command |= 0x01;

    // 发送命令字节（低位先入，上升沿采样）
    for(i = 0; i < 8; i++)
    {
        DS1302_SCLK = 0;          // ✅ 先确保SCLK为低
        DS1302_IO = (Command & (0x01<<i)) ? 1 : 0;
        _nop_();
        DS1302_SCLK = 1;          // ✅ 上升沿，DS1302采样命令
        _nop_();
    }

    // 读取数据字节（低位先入，下降沿后读取）
    for(i = 0; i < 8; i++)
    {
        DS1302_SCLK = 1;
        _nop_();
        DS1302_SCLK = 0;          // ✅ 下降沿
        _nop_();
        if(DS1302_IO)             // ✅ 下降沿后读取
            Data |= (0x01<<i);
    }

    DS1302_CE = 0;
    DS1302_SCLK = 0;
    DS1302_IO = 0;
    return Data;
}

/// @brief 十六进制（bcd）转十进制（dec）
/// @param bcd 
/// @return 
u8 bcd_to_dec(u8 bcd) __reentrant 
{
    return bcd/16*10 + bcd % 16;
}

/// @brief 十进制（dec）转十六进制（bcd）
/// @param dec 
/// @return 
u8 dec_to_bcd(u8 dec) __reentrant
{
    return dec/10*16 + dec%10;
}

/// @brief 将时钟时间初始化地设定为 26年1月1日00:00:00 星期4
/// @param  
void DS1302_TimeInitSet(void)
{
    DS1302_WriteByte(DS1302_WP,0x00);
    DS1302_WriteByte(DS1302_YEAR,dec_to_bcd(DS1302_Time[0]));    // 年
    DS1302_WriteByte(DS1302_MONTH,dec_to_bcd(DS1302_Time[1]));   // 月
    DS1302_WriteByte(DS1302_DATE,dec_to_bcd(DS1302_Time[2]));    // 日
    DS1302_WriteByte(DS1302_HOUR,dec_to_bcd(DS1302_Time[3]));    // 时
    DS1302_WriteByte(DS1302_MINUTE,dec_to_bcd(DS1302_Time[4]));  // 分 ✅ 修复
    DS1302_WriteByte(DS1302_SECOND,dec_to_bcd(DS1302_Time[5]));  // 秒
    DS1302_WriteByte(DS1302_DAY,dec_to_bcd(DS1302_Time[6]));     // 星期 ✅ 修复
    DS1302_WriteByte(DS1302_WP,0x80);
}

/// @brief 设定时间
/// @param Input_Time 传入一个数组指针：(u8 *)malloc(7*sizeof(u8))，格式是{年，月，日，时，分，秒，星期}
void DS1302_SetTime(u8 *Input_Time) __reentrant
{
    DS1302_WriteByte(DS1302_WP,0x00);
    DS1302_WriteByte(DS1302_YEAR,dec_to_bcd(Input_Time[0]));
    DS1302_WriteByte(DS1302_MONTH,dec_to_bcd(Input_Time[1]));
    DS1302_WriteByte(DS1302_DATE,dec_to_bcd(Input_Time[2]));
    DS1302_WriteByte(DS1302_HOUR,dec_to_bcd(Input_Time[3]));
    DS1302_WriteByte(DS1302_MINUTE,dec_to_bcd(Input_Time[4]));
    DS1302_WriteByte(DS1302_SECOND,dec_to_bcd(Input_Time[5]));
    DS1302_WriteByte(DS1302_DAY,dec_to_bcd(Input_Time[6]));
    DS1302_WriteByte(DS1302_WP,0x80);
}



/// @brief 读取当前时刻时钟上的时间
/// @param DS1302_ReadedTime 传入一个数组指针：(u8 *)malloc(7*sizeof(u8))，读取到的时间将被写入到这个数组,格式是{年，月，日，时，分，秒，星期}
void DS1302_ReadTime(u8 *DS1302_ReadedTime)
{
    u8 temp;
    
    temp = DS1302_ReadByte(0x8D);  // 读年
    DS1302_ReadedTime[0] = temp/16*10 + temp%16;
    
    temp = DS1302_ReadByte(0x89);  // 读月
    DS1302_ReadedTime[1] = temp/16*10 + temp%16;
    
    temp = DS1302_ReadByte(0x87);  // 读日
    DS1302_ReadedTime[2] = temp/16*10 + temp%16;
    
    temp = DS1302_ReadByte(0x85);  // 读时
    DS1302_ReadedTime[3] = temp/16*10 + temp%16;
    
    temp = DS1302_ReadByte(0x83);  // 读分
    DS1302_ReadedTime[4] = temp/16*10 + temp%16;
    
    temp = DS1302_ReadByte(0x81);  // 读秒
    DS1302_ReadedTime[5] = temp/16*10 + temp%16;
    
    temp = DS1302_ReadByte(0x8B);  // 读星期
    DS1302_ReadedTime[6] = temp/16*10 + temp%16;
}
