#include  "common.h"
#include  "app.h"
#include  "usb_app.h"


/***********************************************************************
 * main主函数
 * 输入参数 ：无
 * 返回值   ：无
 * 函数功能 ：主程序入口函数，各个模块初始化以及各个模块分支子函数的轮询
 ***********************************************************************/
int main(void)
{	
	
	SystemInit();
	
	uart_init(DEBUG_UART, UART_BAUD_RATE);
	
	usb_initial();     

	while(1)
	{
  }
}

