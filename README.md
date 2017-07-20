# 4-PM25Sensor_C8T6_0513
带模拟U盘的甲醛检测仪程序

硬件：
STM32F103C8T6,  16M/32M SPI FLASH ...

集成的功能：
1 虚拟U盘, 带多个分卷, 物理实体可以是ROM FLASH, SPI FLASH, SD CARD。
2 甲醛传感器，DHT11 传感器驱动。
3 软件定时器功能, os_timer.c, os_timer.h

功能模块
一 软件定时器 
[1] Soft Timer API:

os_timer_setfn(timer, func, parg) 
设置软件定时器的环境；参数: timer: 定时器指针; func： 定时时间到时, 执行的回调函数的指针; parg：回调函数的参数指针, void *类型 .

void os_timer_arm(os_timer_t * timer, uint32_t tick, uint8_t is_repeat) 
功能: 设置该定时器多少tick超时, 是否为循环定时器或单次定时器, 参数为null 则直接返回.
参数: timer: 定时器指针;  tick: 定时的tick次数, 每个tick时间由系统决定, 这里是10ms;  is_repeat: 1: 循环定时器; 0: 单次定时器.

os_timer_disarm(timer)
功能: 停止该定时器, 参数为null直接返回.


os_timer_is_stop(timer)
功能: 检查该定时器是否已停止计时, timer 参数为null 返回0
返回值: 1: 已停止; 0: 还在计时.

调试：
宏 TIMER_DEBUG_EN 为 1, 同时实现os_printf函数

[2] Usage:
   1 实现 os_get_tick()  函数, 返回系统累计运行的tick数, 一般one tick 为 5 - 100 ms
   2 在系统主循环或任务中调用: os_timer_check(), 检查定时器任务链表
   3 包括头文件 "os_timer.h"
   4 定义1个定时器及对应的回调函数:
     os_timer_t tTestTimer;
     void TestTimer_CallBack(void * arg)
     { 
        printf("time is up");
        // Add your process code here when timer is timeout.
        // ...
     }
     
   5 将回调函数与定时器绑定,并设置定时时间.
   os_timer_setfn(&tTestTimer, TestTimer_CallBack, null);
   os_timer_arm(&tTestTimer, 10, 1);  // 10 tick 定时,  循环定时器 
   
     
