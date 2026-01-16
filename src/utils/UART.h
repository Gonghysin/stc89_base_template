#ifndef __UART__

/// @brief 初始化UART串口
/// @param  
void UART_Init(void);


/// @brief 通过串口从单片机向电脑传输一个byte
/// @param Byte 需要传输的字符
void UART_SendByte(unsigned char Byte);



#endif
