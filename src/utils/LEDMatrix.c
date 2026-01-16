
#include "timer.h"
#include "LEDMatrix.h"

#define RCLK P3_5
#define SER P3_4
#define SRCLK P3_6

void LEDMatrix_SendByte(unsigned char dat)
{
    unsigned char i;
    for(i = 0; i < 8; i++)
    {
        if(dat & 0x80)
            SER = 1;
        else    
            SER = 0;

        SRCLK = 0;
        _nop_();
        SRCLK = 1;

        dat = dat << 1;
    }

    RCLK = 0;
    _nop_();
    RCLK = 1;
}


/// @brief 
/// @param Column 列数 0-7 ， 0在最左边
/// @param Data 
void LEDMatrix_ShowColumn(unsigned char Column, unsigned char Data)
{
    LEDMatrix_SendByte(Data);
    P0 = ~(0x80 >> Column);
    delay_ms_timer(3);
    P0 = 0xFF;
}

