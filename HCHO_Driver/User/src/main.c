


#include "stm32f10x.h"
#include "delay.h"	

#include "Application.h"
#include "TimerManager.h"
#include "os_timer.h"


/************************************
V3.5库函数版本不用调用系统时钟设置函数，因为
已经在程序开始的时候已经用汇编指令调用该函数了
SystemInit()这个函数
************************************/



//#include "stm32f1_temp_sensor.h"

int main(void)
{
    //uint16_t key_result;
	
    AppInit();
	//os_printf("PM25 Sensor Version %s, %s %s\r\n", "V2.00", __DATE__, __TIME__);
	
    while(1)
    {
        if(flag10ms)
        {
            flag10ms = 0;

            OS_TimerCheck();
        }
    } 
}



