
#include "stm32f10x.h"
#include "TimerManager.h"
#include "os_timer.h"
#include "os_global.h"
#include "delay.h"
#include "PM25Sensor.h"
#include "Uart_drv.h"
#include "LCD1602_Drv.h"



/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : 配置不同的系统时钟
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void SysClockConfig(void)
{
        /* RCC system reset(for debug purpose) */
        RCC_DeInit();

#ifdef HSIENABLE                //当使用HSI高速内部时钟作为系统时钟时
        RCC_HSICmd(ENABLE);        //打开内部高速时钟
        //等待HSI准备好
        while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);        //开启FLASH预取指功能
        //FLASH时序控制
        //推荐值:SYSCLK = 0~24MHz   Latency=0
        //       SYSCLK = 24~48MHz  Latency=1
        //       SYSCLK = 48~72MHz  Latency=2
        FLASH_SetLatency(FLASH_Latency_2);
        RCC_HCLKConfig(RCC_SYSCLK_Div1);        //设置HCLK(AHB时钟)=SYSCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);                //PCLK2(APB2) = HCLK
        RCC_PCLK1Config(RCC_HCLK_Div2);                //PCLK1(APB1) = HCLK = 24 MHz

        //PLL设置 SYSCLK: HSI / 2 * 12 = 4*12 = 48MHz
        RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);
        //启动PLL
        RCC_PLLCmd(ENABLE);//如果PLL被用于系统时钟,不能被DISABLE
        //等待PLL稳定
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){;}

        //设置系统时钟SYSCLK = PLL输出
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        //等待PLL成功用作于系统时钟的时钟源,并等待稳定
        // 0x00:HSI作为系统时钟
        // 0x04:HSE作为系统时钟
        // 0x08:PLL作为系统时钟
        while(RCC_GetSYSCLKSource() != 0x08);
		
		SystemCoreClock = CPU_CLOCK;
#else
        /* Enable HSE */
        RCC_HSEConfig(RCC_HSE_ON);

        /* Wait till HSE is ready */
        HSEStartUpStatus = RCC_WaitForHSEStartUp();

        if(HSEStartUpStatus == SUCCESS)
        {
                /* HCLK = SYSCLK */
                RCC_HCLKConfig(RCC_SYSCLK_Div1);
                /* PCLK2 = HCLK */
                RCC_PCLK2Config(RCC_HCLK_Div1);
                /* PCLK1 = HCLK/2 */
                RCC_PCLK1Config(RCC_HCLK_Div2);
                /* Flash 2 wait state */
                FLASH_SetLatency(FLASH_Latency_2);
                /* Enable Prefetch Buffer */
                FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
                /* PLLCLK = 8MHz * 6 = 48 MHz */
                RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);
                /* Enable PLL */
                RCC_PLLCmd(ENABLE);
                /* Wait till PLL is ready */
                while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
                {
                }
                /* Select PLL as system clock source */
                RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
                /* Wait till PLL is used as system clock source */
                while(RCC_GetSYSCLKSource() != 0x08);
        }
#endif
        /* Enable DMA1 clock */
       // RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        /* Enable ADC1E clock */
        //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);        
        /* TIM4 clock source enable */
       // RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
        /* Enable GPIOA, GPIOB, GPIOC, GPIOD and AFIO clocks */
       // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
    /* USBCLK = PLLCLK  = 48 MHz */
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_Div1);

    /* Enable USB clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

// 外设时钟初始化
static void RCC_PeriphInit(void)
{
    RCC_APB2PeriphClockCmd(  RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
		                      | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);

	#if 0
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  // 禁止JTAG
    GPIO_PinRemapConfig(GPIO_Remap_PD01, ENABLE);  // 使能PD0 PD1
    #else
    SET_REG_32_BIT(AFIO->MAPR, AFIO_MAPR_PD01_REMAP | AFIO_MAPR_SWJ_CFG_JTAGDISABLE); 
	#endif
}

/**
 * NVIC Configuration
 */
void NVIC_Configuration(void)
{
	/* Set the Vector Table base location at 0x08000000 */
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
}

#include "DHT11.h"

void AppInit(void)
{
   RCC_ClocksTypeDef rcc_clocks;
   char time_str[16];
   char ver_str[16];
   
   GLOBAL_DISABLE_IRQ();
   NVIC_Configuration();
   
   SysClockConfig();  
   SysTick_Init();
   RCC_PeriphInit();


   USART2_Init(115200);  // 串口调试打印
   
   USART1_Init(9600);   // PM2.5 接收口
   USART3_Init(9600);   // HCHO 接收串口
  
  
   GLOBAL_ENABLE_IRQ();
   
   BSP_Init(); 
  

   #if 1  // 打印时钟频率
   RCC_GetClocksFreq(&rcc_clocks);  // 读取系统时钟
   os_printf("SysClk = %d, HCLK = %d, PCLK1 = %d, PCLK2 = %d, ADCLK = %d MHz\r\n", rcc_clocks.SYSCLK_Frequency / FREQ_1MHz,
   	               rcc_clocks.HCLK_Frequency / FREQ_1MHz, rcc_clocks.PCLK1_Frequency / FREQ_1MHz, 
   	               rcc_clocks.PCLK2_Frequency / FREQ_1MHz, rcc_clocks.ADCCLK_Frequency / FREQ_1MHz);
   os_printf("app version: %s %s %s\r\n", SOFT_VERSION, __DATE__, __TIME__);
   #endif


   PM25_Init();  // PM2.5 和甲醛获取的定时任务初始化

   DHT_Init();   // 温湿度 DHT11 初始化
   LCD1602_Init();  // LCD 1602 初始化

   //显示版本号
   os_memset(time_str,  0, sizeof(time_str));
   os_memset(ver_str,   0, sizeof(ver_str));
   os_snprintf(time_str, sizeof(time_str),  "%s ", __DATE__);
   os_strncpy(ver_str, SOFT_VERSION, sizeof(ver_str));
   
   LCD1602_WriteString(0, 0, (const uint8_t *)ver_str);
   LCD1602_WriteString(1, 0, (const uint8_t *)time_str);
}

