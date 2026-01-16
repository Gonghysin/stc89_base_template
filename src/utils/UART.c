#include <8052.h>
#include "UART.h"


void UART_Init(void){
    SCON = 0x50; // 0100 0000 
    // SM0 = 0, SM1 = 1, 工作在方式1
    PCON &= 0x7F ;

    // 配置定时器1 ，使用双八位模式
    TMOD &= 0x0F;
    TMOD |= 0x20; // 0010 0000
    TL1 = 0xFD;   // 设置定时器初始值
    TH1 = 0xFD;   // 设置定时器重载值
    ET1 = 0;
    TR1 = 1;

}

void UART_SendByte(unsigned char Byte)
{
    SBUF = Byte;
    while(TI == 0);
    TI = 0;
}