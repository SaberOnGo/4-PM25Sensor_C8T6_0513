
#ifndef  __PM25SENSOR_H__
#define   __PM25SENSOR_H__

#include "GlobalDef.h"


#define DISPLAY_CC      0   // 显示 PM2.5 PM10 浓度
#define DISPLAY_PC      1   // 显示 PM2.5 PM10 粒子数
#define DISPLAY_AQI_US  2   // 显示 AQI US 标准
#define DISPLAY_AQI_CN  3   // 显示 AQI CN 标准
#define DISPLAY_HCHO    4   // 显示 甲醛浓度

#define HCHO_DISPLAY_MASSFRACT   0  // 显示质量分数, ug/m3
#define HCHO_DISPLAY_PPM         1  // 显示体积分数, ppm

// 对应 PM25 的LED等级, 7级
typedef enum
{
   AQI_GOOD = 0,           // 优,      绿灯
   AQI_Moderate = 1,       // 中等,    绿黄灯
   AQI_LightUnhealthy = 2, // 轻度污染, 黄灯
   AQI_MidUnhealthy = 3,   // 中度污染, 黄灯
   AQI_VeryUnhealthy = 4,  // 重度污染, 红灯
   AQI_JustInHell    = 5,  // 刚进入地狱,   黄 红灯慢闪
   AQI_HeavyInHell   = 6,  // 深度地狱模式, 3灯狂闪
   AQI_LEVEL_END,
}E_AQI_LEVEL;

// PM25 Sensor的数据

#pragma pack(1)
// HCHO 浓度(PMS5003S)或版本号(PMS5003)
typedef union
{
   uint16_t hcho;   // 甲醛浓度, 真实浓度 = 本数值 / 1000, unit: mg/m3
   uint8_t  ver[2]; // ver[0] 为版本号, ver[1] 为错误代码
}U_HCHO_VERSION;

typedef struct
{
   uint8_t start_char_0;
   uint8_t start_char_1;
   uint16_t len;   // 数据长度: 2 x 13 + 2
   
   uint16_t pm1_cf1;     // PM1.0 ug/m3
   uint16_t pm25_cf1;    // PM25   ug/m3
   uint16_t pm10_cf1;    // PM10   ug/m3
   uint16_t pm1_air;     // PM1.0, 大气环境下
   uint16_t pm25_air;    // PM25, 大气环境下
   uint16_t pm10_air;    // PM10, 大气环境下
   
   uint16_t PtCnt_0p3um;      // Particle Count, 0.1 L 空气中0.3um以上颗粒物个数
   uint16_t PtCnt_0p5um;
   uint16_t PtCnt_1p0um;
   uint16_t PtCnt_2p5um;
   uint16_t PtCnt_5p0um;
   uint16_t PtCnt_10p0um;
   
   U_HCHO_VERSION extra;  // 额外数据
   uint16_t sum;             // 校验和
}T_PM25_Sensor;

// AQI 标准
typedef enum
{
   AQI_US = 0,
   AQI_CN,
}E_AQI_STD;

// AQI 计算
typedef struct
{
   float C_low_us;     // 浓度低值, 美国标准
   float C_high_us;    // 浓度高值, 美国标准
   float C_low_cn;     // 浓度低值, 中国标准
   float C_high_cn;    // 浓度高值, 中国标准
   uint16_t I_low;     // AQI 低值
   uint16_t I_high;    // AQI 高值
}T_AQI;

// HCHO 数据
typedef struct
{
    uint8_t start_char_0;   // 起始头 0xFF
    uint8_t gas_name;       // 气体名称
    uint8_t unit;           // 单位： Ppb
    uint8_t decimal_digits;   // 小数位数
    uint8_t air_high;          // 气体浓度高位
    uint8_t air_low;           // 气体浓度低位
    uint8_t full_scale_high;    // 满量程高位
    uint8_t full_scale_low;     // 满量程低位
    uint8_t check_sum;            // 校验和
}T_HCHO_Sensor;
#pragma pack()

void PM25_Init(void);
//#if (SENSOR_SELECT != HCHO_SENSOR)
void PM25_Receive(uint8_t * buf, uint16_t len);
void PM25_Display_Init(uint8_t display_mode);
//#else
void HCHO_Receive(uint8_t * buf, uint16_t len);
uint8_t HCHO_CheckSum(uint8_t * buf, uint16_t len);
void HCHO_SwitchToQueryMode(void);
void HCHO_QueryHCHOConcentration(void);
void HCHO_SwitchToActiveMode(void);
void HCHO_Indicate(uint16_t val);
void HCHO_DisplayInit(uint8_t display_mode);
void LED_IndicateColorOfHCHO(void);
void HCHO_CaliSet(void);

//#endif

void PM25_UART_IRQHandler(void);
void HCHO_UART_IRQHandler(void);

#endif

