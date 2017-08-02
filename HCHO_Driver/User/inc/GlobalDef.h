
#ifndef __GLOBAL_DEF_H__
#define  __GLOBAL_DEF_H__

#include "stm32f10x.h"
/******************************** 编译开关begin *********************************/
#define SYS_DEBUG  //调试
#ifdef SYS_DEBUG
#define UART_DEBUG  //串口调试
#endif
#include <stdio.h>
#include <stdarg.h>










#define DEBUG_VERSION   1

#if DEBUG_VERSION
#define os_printf(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#define SW_PRINTF_EN  0       // printf 由软件串口输出(1) 或 由硬件串口输出(0)
#else
#define os_printf(...)
#endif

#define INSERT_DEBUG_INFO()   os_printf("debug, file = %s, line = %d\n", __FILE__, __LINE__)
#define INSERT_ERROR_INFO(err)   os_printf("error = %d, file = %s, line = %d\n", err, __FILE__, __LINE__)

#if DEBUG_VERSION
#define ASSERT_PARAM(expr) ((expr) ? (int)0 : INSERT_ERROR_INFO(0) )   // 检查参数是否为空
#else
#define ASSERT_PARAM(expr) ((void)0)
#endif


#define  SOFT_VERSION    "HCHO Drv V1.31"


#define FREQ_1MHz        (1000000L)
#define FREQ_2MHz        (2000000L)
#define FREQ_8MHz        (8000000L)
#define FREQ_16MHz      (16000000L)
#define FREQ_48MHz      (48000000L)


#define CPU_CLOCK       FREQ_48MHz  // CPU 时钟

#define HSIENABLE         1           // HSI 时钟使能, 内部 8 M
//#define SYS_USING_BOOTLOADER  1         // 使能 bootloader
#define OS_SYS_TICK_EN                // TIM1 10 ms中断一次 作为 Sys Tick
#define OS_PER_TICK_MS   (10)         // 每个tick的时间: 10ms




typedef enum
{
   FLASH_DISK = 0,
   ROM_DISK   = 1,
   SD_DISK    = 2,
}DISK_ENUM;



/******************************** 编译开关end *********************************/

/*********************************内存分配begin ******************************/
//用户可根据需要更改


/*********************************内存分配end ******************************/
/***************************** 类型定义begin  ************************************/
typedef enum
{
   INPUT  =  0,
   OUTPUT =  1,
}E_IO_DIR;   // IO 输出或输入方向

typedef enum
{
    E_FALSE = 0,
	E_TRUE = !E_FALSE
}E_BOOL;

typedef enum
{
   APP_SUCCESS = 0,
   APP_FAILED = 1,
   APP_NULL = 0xFF,
}E_RESULT;

typedef enum
{
   LESS_STATE = 0,  //小于
   EQUAL_STATE,     //等于
   MORE_STATE,      //大于
}Compare_State;  //比较状态值
//信号量的值
typedef enum
{
    UART_IDLE = 0,  //解锁
    UART_BUSY = !UART_IDLE
}Sema_State;
typedef ErrorStatus UartStatus; 
typedef uint8_t  QueueMemType;       //队列缓冲区存储类型
typedef uint16_t QueuePointerType;  //队列指针类型
typedef struct
{
   QueueMemType *Base;  //队列缓冲区存储位置
   QueuePointerType QHead;  //队列头指针, 向队列写入数据
   QueuePointerType QTail;  //队列尾指针, 对发送来说,输出数据是向串口发送; 对接收来说, 输出是从接收队列中取
}T_UartQueue;
typedef struct struc_rtc_time
{
   uint8_t year;
   uint8_t month;
   uint8_t day;
   uint8_t week;
   uint8_t hour;
   uint8_t min;
   uint8_t sec;
   uint8_t century;
}T_RTC_TIME;
/***************************** 类型定义end  **************************************************************/

#define LITTLE_ENDIAN   0
#define BIG_ENDIAN      1


#define PMS5003      0    // 不带甲醛测量
#define PMS5003S     1     // 带甲醛测量
#define HCHO_SENSOR  2
#define SENSOR_SELECT  HCHO_SENSOR


#define HCHO_EN     1   // 甲醛显示使能(1), 0: 禁止
#define PM25_EN     1   // PM2.5显示使能(1),0: 禁止

/********************************* 宏 函数begin **********************************************/
#define NOP()   __NOP()
#define GLOBAL_DISABLE_IRQ()      __disable_irq()  //Global interrupt disable
#define GLOBAL_ENABLE_IRQ()       __enable_irq()   //Global interrupt enable

#define READ_REG_32_BIT(reg,  b)      ((uint32_t)((reg)&((uint32_t)b)))      //b必须为整数, reg为32 bit 寄存器
#define CLEAR_REG_32_BIT(reg, b)      ((reg)&=(uint32_t)(~((uint32_t)b)))   //b必须为整数, reg为32 bit 寄存器
#define SET_REG_32_BIT(reg,   b)      ((reg)|=(uint32_t)(b))                  //b必须为整数, reg为32 bit 寄存器

#define READ_REG_8_BIT(reg,b)     ((uint8_t)((reg)&((uint8_t)b)))           //b必须为8bit整数, reg为寄存器
#define CLEAR_REG_8_BIT(reg,b)    ((reg)&=(uint8_t)(~((uint8_t)b)))        //b必须为8bit整数, reg为寄存器
#define SET_REG_8_BIT(reg,b)      ((reg)|=(uint8_t)(b))                      //b必须为8bit整数, reg为寄存器

// 获取type(结构体类型或者数组) 按字对齐后的长度, STM32 为 4字节对齐
#define WORD_ALIGNED_LEN(type)   ( (sizeof(type) + sizeof(int) - 1) &  ~(sizeof(int) - 1) ) 

/********************************* 宏 函数end  **********************************************/
#endif  // __GLOBAL_DEF_H__


