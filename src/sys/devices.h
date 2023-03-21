#ifndef __IO_H
#define __IO_H

#include<stdint.h>
#include "cm4.h"

#define __reserved(offset) \
  uint32_t _reserved_ ## offset

#define __reserved_interval(offset1,offset2) \
  uint32_t _reserved_ ## offset1 ## _ ## offset2[((offset2)-(offset1))/4]

/* PM §4.2.10 */ 
struct MPU_registers {
  uint32_t TYPER;
  uint32_t CTRL;
  uint32_t RNR;
  uint32_t RBAR;
  uint32_t RASR;
  uint32_t RBAR_A1;
  uint32_t RASR_A1;
  uint32_t RBAR_A2;
  uint32_t RASR_A2;
  uint32_t RBAR_A3;
  uint32_t RASR_A3;
};

/* PM §4.3.11 */ 
struct NVIC_registers {
  uint32_t ISER[3];
  __reserved_interval(0x0c,0x80);
  uint32_t ICER[3];
  __reserved_interval(0x8c,0x100);
  uint32_t ISPR[3];
  __reserved_interval(0x10c,0x180);
  uint32_t ICPR[3];
  __reserved_interval(0x18c,0x200);
  uint32_t IABR[3];
  __reserved_interval(0x20c,0x300);
  uint8_t  IP[81];
  uint8_t _reserved_bytes[3];
};

/* PM §4.4.19 */ 
struct SCB_registers {
  uint32_t CPUID;
  uint32_t ICSR;
  uint32_t VTOR;
  uint32_t AIRCR;
  uint32_t SCR;
  uint32_t CCR;
  uint32_t SHPR1;
  uint32_t SHPR2;
  uint32_t SHPR3;
  uint32_t SHCRS;
  uint32_t CFSR;
  uint32_t HFSR;
  uint32_t MMAR;
  uint32_t BFAR;
  uint32_t AFSR;
};

/* PM §4.5.6 */ 
struct STK_registers {
  uint32_t CTRL;
  uint32_t LOAD;
  uint32_t VAL;
  uint32_t CALIB;
};

/* PM §4.6 */ 
struct FPU_registers {
  __reserved(0x0);
  uint32_t CCR;
  uint32_t CAR;
  uint32_t DSCR;
};


/* RM §3.6.7 */ 
struct FLASH_registers {
  uint32_t ACR;
  uint32_t KEYR;
  uint32_t OPTKEYR;
  uint32_t SR;
  uint32_t CR;
  uint32_t OPTCR;  
};

/* RM §4.4.4 */
struct CRC_registers {
  uint32_t DR;
  uint32_t ICR;
  uint32_t CR;
};

/* RM §5.5 */
struct PWR_registers {
  uint32_t CR;
  uint32_t CSR;
};

/* RM §6.3.28 */
struct RCC_registers {
  uint32_t CR;
  uint32_t PLLCFGR;
  uint32_t CFGR;
  uint32_t CIR;
  uint32_t AHB1RSTR;
  uint32_t AHB2RSTR;
  uint32_t AHB3RSTR;
  __reserved(0x1c);
  uint32_t APB1RSTR;
  uint32_t APB2RSTR;
  __reserved(0x28);
  __reserved(0x2c);
  uint32_t AHB1ENR;
  uint32_t AHB2ENR;
  uint32_t AHB3ENR;
  __reserved(0x3c);
  uint32_t APB1ENR;
  uint32_t APB2ENR;
  __reserved(0x48);
  __reserved(0x4c);
  uint32_t AHB1LPENR;
  uint32_t AHB2LPENR;
  uint32_t AHB3LPENR;
  __reserved(0x5c);
  uint32_t APB1LPENR;
  uint32_t APB2LPENR;
  __reserved(0x68);
  __reserved(0x6c);
  uint32_t BDCR;
  uint32_t CSR;
  __reserved(0x78);
  __reserved(0x7c);
  uint32_t SSCGR;
  uint32_t PLLI2SCFGR;
  uint32_t PLLSAICFGR;
  uint32_t DCKCFGR;
  uint32_t CKGATENR;
  uint32_t DCKCFGR2;
};

/* RM §7.4.11 */
struct GPIO_registers {
  uint32_t MODER;
  uint32_t OTYPER;
  uint32_t OSPEEDR;
  uint32_t PUPDR;
  uint32_t IDR;
  uint32_t ODR;
  uint32_t BSRR;
  uint32_t LCKR;
  uint32_t AFRL;
  uint32_t AFRH;
};

/* RM §8.2.9 */
struct SYSCFG_registers {
  uint32_t MEMRMP;
  uint32_t PMC;
  uint32_t EXTICR1;
  uint32_t EXTICR2;
  uint32_t EXTICR3;
  uint32_t EXTICR4;
  __reserved(0x18);
  __reserved(0x1c);
  uint32_t CMPCR;
  __reserved(0x24);
  __reserved(0x28);
  uint32_t CFGR;
};

/* RM §9.5.11 */
struct DMA_Sx_registers {
  uint32_t CR;
  uint32_t NDTR;
  volatile void* PAR;
  volatile void* M0AR;
  volatile void* M1AR;
  uint32_t FCR;
};
struct DMA_registers {
  uint32_t LISR;
  uint32_t HISR;
  uint32_t LIFCR;
  uint32_t HIFCR;
  struct DMA_Sx_registers S[8];
};

/* RM §10.3.7 */
struct EXTI_registers {
  uint32_t IMR;
  uint32_t EMR;
  uint32_t RTSR;
  uint32_t FTSR;
  uint32_t SWIER;
  uint32_t PR;
};

/* RM §11.8 */
struct FMC_registers {
  uint32_t BCR1;
  uint32_t BCR2;
  uint32_t BCR3;
  uint32_t BCR4;
  uint32_t BTR1;
  uint32_t BTR2;
  uint32_t BTR3;
  uint32_t BTR4;
  uint32_t BWTR1;
  uint32_t BWTR2;
  uint32_t BWTR3;
  uint32_t BWTR4;
  uint32_t PCR;
  uint32_t SR;
  uint32_t PMEM;
  uint32_t PATT;
  uint32_t ECCR;
  uint32_t SDCR1;
  uint32_t SDCR2;
  uint32_t SDTR1;
  uint32_t SDTR2;
  uint32_t SDCMR;
  uint32_t SDRTR;
  uint32_t SDSR;
};

/* RM §12.5.14 */
struct QUADSPI_registers {
  uint32_t CR;
  uint32_t DCR;
  uint32_t SR;
  uint32_t FCR;
  uint32_t DLR;
  uint32_t CCR;
  uint32_t AR;
  uint32_t ABR;
  uint32_t DR;
  uint32_t PSMKR;
  uint32_t PSMAR;
  uint32_t PIR;
  uint32_t LPTR;
};

/* RM §13.13.18 */
struct ADCx_registers {
  uint32_t SR;
  uint32_t CR1;
  uint32_t CR2;
  uint32_t SMPR1;
  uint32_t SMPR2;
  uint32_t JOFR1;
  uint32_t JOFR2;
  uint32_t JOFR3;
  uint32_t JOFR4;
  uint32_t HTR;
  uint32_t LTR;
  uint32_t SQR1;
  uint32_t SQR2;
  uint32_t SQR3;
  uint32_t JSQR;
  uint32_t JDR1;
  uint32_t JDR2;
  uint32_t JDR3;
  uint32_t JDR4;
  uint32_t DR;
};

struct ADC_common_registers {
  uint32_t CSR;
  uint32_t CCR;
  uint32_t CDR;
};


/* RM §14.5.15 */
struct DAC_registers {
  uint32_t CR;
  uint32_t SWTRIGR;
  uint32_t DHR12R1;
  uint32_t DHR12L1;
  uint32_t DHR8R1;
  uint32_t DHR12R2;
  uint32_t DHR12L2;
  uint32_t DHR8R2;
  uint32_t DHR12RD;
  uint32_t DHR12LD;
  uint32_t DHR8RD;
  uint32_t DOR1;
  uint32_t DOR2;
  uint32_t SR;
};

/* RM §15.8.12 */
struct DCMI_registers {
  uint32_t CR;
  uint32_t SR;
  uint32_t RIS;
  uint32_t IER;
  uint32_t MIS;
  uint32_t ICR;
  uint32_t ESCR;
  uint32_t ESUR;
  uint32_t CWSTRT;
  uint32_t CWSIZE;
  uint32_t DR;
};

/* RM §16.4.21 TIM1,8 */
/* RM §17.4.21 TIM2,3,4,5 */
/* RM §18.4.13 TIM9,12 */
/* RM §18.5.1  TIM10,11,13,14 */
/* RM §19.4.9  TIM6,7 */
struct TIMx_registers {
  uint32_t CR1;
  uint32_t CR2;
  uint32_t SMCR;
  uint32_t DIER;
  uint32_t SR;
  uint32_t EGR;
  uint32_t CCMR1;
  uint32_t CCMR2;
  uint32_t CCER;
  uint32_t CNT;
  uint32_t PSC;
  uint32_t ARR;
  uint32_t RCR;
  uint32_t CCR1;
  uint32_t CCR2;
  uint32_t CCR3;
  uint32_t CCR4;
  uint32_t BDTR;
  uint32_t DCR;
  uint32_t DMAR;
  uint32_t OR;
};

/* RM §20.4.5 */
struct IWDG_registers {
  uint32_t KR;
  uint32_t PR;
  uint32_t RLR;
  uint32_t SR;
};

/* RM §21.6.4 */
struct WWDG_registers {
  uint32_t CR;
  uint32_t CFR;
  uint32_t SR;
};

/* RM §22.6.21 */
struct RTC_registers {
  uint32_t TR;
  uint32_t DR;
  uint32_t CR;
  uint32_t ISR;
  uint32_t PRER;
  uint32_t WUTR;
  uint32_t CALIBR;
  uint32_t ALRMAR;
  uint32_t ALRMBR;
  uint32_t WPR;
  uint32_t SSR;
  uint32_t SHIFTR;
  uint32_t TSTR;
  uint32_t TSSSR;
  uint32_t CALR;
  uint32_t TAFCR;
  uint32_t ALRMASSR;
  uint32_t ALRMBSSR;
  uint32_t BKPR[20];
};

/* RM §23.7.12 */
struct FMPI2C_registers {
  uint32_t CR1;
  uint32_t CR2;
  uint32_t OAR1;
  uint32_t OAR2;
  uint32_t TIMINGR;
  uint32_t TIMEOUTR;
  uint32_t ISR;
  uint32_t ICR;
  uint32_t PECR;
  uint32_t RXDR;
  uint32_t TXDR;
};

/* RM §24.6.14 */
struct I2C_registers {
  uint32_t CR1;
  uint32_t CR2;
  uint32_t OAR1;
  uint32_t OAR2;
  uint32_t DR;
  uint32_t SR1;
  uint32_t SR2;
  uint32_t CCR;
  uint32_t TRISE;
  uint32_t FLTR;
};

/* RM §25.6.8 */
struct USART_registers {
  uint32_t SR;
  uint32_t DR;
  uint32_t BRR;
  uint32_t CR1;
  uint32_t CR2;
  uint32_t CR3;
  uint32_t GTPR;
};

/* RM §26.7.10 */
struct SPI_I2S_registers {
  uint32_t CR1;
  uint32_t CR2;
  uint32_t SR;
  uint32_t DR;
  uint32_t CRCPR;
  uint32_t RXCRCR;
  uint32_t TXCRCR;
  uint32_t I2SCFGR;
  uint32_t I2SPR;
};

/* RM §27.5.10 */
struct SPDIFRX_registers {
  uint32_t CR;
  uint32_t IMR;
  uint32_t SR;
  uint32_t IFCR;
  uint32_t DR;
  uint32_t CSR;
  uint32_t DIR;
};

/* RM §28.5.10 */
struct SAI_x_registers {
  uint32_t CR1;
  uint32_t CR2;
  uint32_t FRCR;
  uint32_t SLOTR;
  uint32_t IM;
  uint32_t SR;
  uint32_t CLRFR;
  uint32_t DR;
};

struct SAI_registers {
  uint32_t GCR;
  struct SAI_x_registers A;
  struct SAI_x_registers B;
};

/* RM §29.8.16 */
struct SDIO_registers {
  uint32_t POWER;
  uint32_t CLKCR;
  uint32_t CMD;
  uint32_t RESPCMD;
  uint32_t RESP1;
  uint32_t RESP2;
  uint32_t RESP3;
  uint32_t RESP4;
  uint32_t DTIMER;
  uint32_t DLEN;
  uint32_t DCTRL;
  uint32_t DCOUNT;
  uint32_t STA;
  uint32_t ICR;
  uint32_t MASK;
  __reserved(0x40);
  __reserved(0x44);
  uint32_t FIFOCNT;
  __reserved_interval(0x4c,0x80);
  uint32_t FIFO[32];
};
  
/* RM §30.9.5 */
struct bxCAN_TxRx_registers {
  uint32_t IR;
  uint32_t DTR;
  uint32_t DLR;
  uint32_t DHR;
};

struct bxCAN_registers {
  uint32_t MCR;
  uint32_t MSR;
  uint32_t TSR;
  uint32_t RF0R;
  uint32_t RF1R;
  uint32_t IER;
  uint32_t ESR;
  uint32_t BTR;
  __reserved_interval(0x20,0x180);
  struct bxCAN_TxRx_registers Tx[3];
  struct bxCAN_TxRx_registers Rx[2];
  __reserved_interval(0x1d0,0x200);
  uint32_t FMR;
  uint32_t FM1R;
  __reserved(0x208);
  uint32_t FS1R;
  __reserved(0x210);
  uint32_t FFA1R;
  __reserved(0x218);
  uint32_t FA1R;
  __reserved_interval(0x220,0x240);
  struct {uint32_t R1; uint32_t R2;} F[28];
};

/* RM §32.7.7 */
struct OTG_registers {
   /* TODO */
};

/* RM §32.7.7 */
struct HDMI_CEC_registers {
  uint32_t CR;
  uint32_t CFGR;
  uint32_t TXDR;
  uint32_t RXDR;
  uint32_t ISR;
  uint32_t IER;
};

/* RM §33.18 */
struct DBGMCU_registers {
  uint32_t IDCODE;
  uint32_t CR;
  uint32_t APB1FZ;
  uint32_t APB2FZ;
};

/* Constant values */

extern const uint16_t Package_data;
extern const uint16_t TS_CAL2;
extern const uint16_t TS_CAL1;
extern const uint16_t VREFIN_CAL;
extern const uint16_t Flash_size;
extern const uint32_t U_ID[3];

/* Core Devices base addresses */

extern volatile struct DBGMCU_registers DBG;

extern volatile struct FPU_registers FPU;
extern volatile uint32_t NVIC_STIR;
extern volatile struct MPU_registers MPU;
extern volatile uint32_t FPU_CPACR;
extern volatile struct SCB_registers SCB;
extern volatile struct NVIC_registers NVIC;
extern volatile struct STK_registers SysTick;

/* Devices base addresses */

extern volatile struct QUADSPI_registers QUADSPI;

extern volatile struct DCMI_registers DCMI;
extern volatile struct OTG_registers USB_OTG_FS;

extern volatile struct OTG_registers USB_OTG_HS;
extern volatile struct DMA_registers DMA2;
extern volatile struct DMA_registers DMA1;
extern uint32_t BKPSRAM[1024];
extern volatile struct FLASH_registers FLASH;
extern volatile struct RCC_registers RCC;
extern volatile struct CRC_registers CRC;
extern volatile struct GPIO_registers GPIOH;
extern volatile struct GPIO_registers GPIOG;
extern volatile struct GPIO_registers GPIOF;
extern volatile struct GPIO_registers GPIOE;
extern volatile struct GPIO_registers GPIOD;
extern volatile struct GPIO_registers GPIOC;
extern volatile struct GPIO_registers GPIOB;
extern volatile struct GPIO_registers GPIOA;

extern volatile struct SAI_registers SAI2;
extern volatile struct SAI_registers SAI1;

extern volatile struct TIMx_registers TIM11;
extern volatile struct TIMx_registers TIM10;
extern volatile struct TIMx_registers TIM9;
extern volatile struct EXTI_registers EXTI;
extern volatile struct SYSCFG_registers SYSCFG;
extern volatile struct SPI_registers SPI4;
extern volatile struct SPI_registers SPI1;
extern volatile struct SDIO_registers SDMMC;
extern volatile struct ADCx_registers ADC1;
extern volatile struct ADCx_registers ADC2;
extern volatile struct ADCx_registers ADC3;
extern volatile struct ADC_common_registers ADC_common;
extern volatile struct USART_registers USART6;
extern volatile struct USART_registers USART1;
extern volatile struct TIMx_registers TIM8;
extern volatile struct TIMx_registers TIM1;

extern volatile struct DAC_registers DAC;
extern volatile struct PWR_registers PWR;
extern volatile struct HDMI_CEC_registers HDMI_CEC;
extern volatile struct bxCAN_registers CAN2;
extern volatile struct bxCAN_registers CAN1;
extern volatile struct FMPI2C_registers FMPI2C1;
extern volatile struct I2C_registers I2C3;
extern volatile struct I2C_registers I2C2;
extern volatile struct I2C_registers I2C1;
extern volatile struct USART_registers UART5;
extern volatile struct USART_registers UART4;
extern volatile struct USART_registers USART3;
extern volatile struct USART_registers USART2;
extern volatile struct SPDIF_RX_registers SPDIF_RX;
extern volatile struct SPI_I2S_registers SPI3_I2S3;
extern volatile struct SPI_I2S_registers SPI2_I2S2;
extern volatile struct IWDG_registers IWDG;
extern volatile struct WWDG_registers WWDG;
extern volatile struct RTC_registers RTC_BKP;
extern volatile struct TIMx_registers TIM14;
extern volatile struct TIMx_registers TIM13;
extern volatile struct TIMx_registers TIM12;
extern volatile struct TIMx_registers TIM7;
extern volatile struct TIMx_registers TIM6;
extern volatile struct TIMx_registers TIM5;
extern volatile struct TIMx_registers TIM4;
extern volatile struct TIMx_registers TIM3;
extern volatile struct TIMx_registers TIM2;

#define __set_bit(_w,_i) ((_w)|=(1<<(_i)))
#define __clr_bit(_w,_i) ((_w) &= ~(1<<(_i)))
#define __really_inline__ static inline __attribute__((always_inline)) 
#define __sync __DSB

#define __declare_device_control(name,bus,bus_bit) \
  void __really_inline__ enable_##name() { \
    __set_bit(RCC.bus##ENR,bus_bit); __sync(); } \
  void __really_inline__ low_power_enable_##name() { \
    __set_bit(RCC.bus##LPENR,bus_bit); __sync(); } \
  void __really_inline__ disable_##name() { \
    __clr_bit(RCC.bus##ENR,bus_bit); __sync(); } \
  void __really_inline__ low_power_disable_## name() { \
    __clr_bit(RCC.bus##LPENR,bus_bit); __sync(); } \
  void __really_inline__ reset_##name() { \
    __set_bit(RCC.bus##RSTR,bus_bit); __sync(); } \
\

/* __declare_device_control(FMC,AHB3,0) FMC not available in LQFP64 package */
__declare_device_control(QUADSPI,AHB3,1)

__declare_device_control(DCMI,AHB2,0)
__declare_device_control(USB_OTG_FS,AHB2,7)

__declare_device_control(USB_OTG_HS,AHB1,29)
__declare_device_control(DMA2,AHB1,22)
__declare_device_control(DMA1,AHB1,21)
__declare_device_control(CRC,AHB1,12)
__declare_device_control(GPIOH,AHB1,7)
__declare_device_control(GPIOG,AHB1,6)
__declare_device_control(GPIOF,AHB1,5)
__declare_device_control(GPIOE,AHB1,4)
__declare_device_control(GPIOD,AHB1,3)
__declare_device_control(GPIOC,AHB1,2)
__declare_device_control(GPIOB,AHB1,1)
__declare_device_control(GPIOA,AHB1,0)

__declare_device_control(SAI2,APB2,23)
__declare_device_control(SAI1,APB2,22)

__declare_device_control(TIM11,APB2,18)
__declare_device_control(TIM10,APB2,17)
__declare_device_control(TIM9,APB2,16)
__declare_device_control(SYSCFG,APB2,14)
__declare_device_control(SPI4,APB2,13)
__declare_device_control(SPI1,APB2,12)
__declare_device_control(SDMMC,APB2,11)
__declare_device_control(ADC,APB2,8)
__declare_device_control(ADC1,APB2,8)
__declare_device_control(ADC2,APB2,8)
__declare_device_control(ADC3,APB2,8)
__declare_device_control(USART6,APB2,5)
__declare_device_control(USART1,APB2,4)
__declare_device_control(TIM8,APB2,1)
__declare_device_control(TIM1,APB2,0)

__declare_device_control(DAC,APB1,29)
__declare_device_control(PWR,APB1,28)
__declare_device_control(HDMI_CEC,APB1,27)
__declare_device_control(CAN2,APB1,26)
__declare_device_control(CAN1,APB1,25)
__declare_device_control(FMPI2C1,APB1,24)
__declare_device_control(I2C3,APB1,23)
__declare_device_control(I2C2,APB1,22)
__declare_device_control(I2C1,APB1,21)
__declare_device_control(UART5,APB1,20)
__declare_device_control(UART4,APB1,19)
__declare_device_control(USART3,APB1,18)
__declare_device_control(USART2,APB1,17)
__declare_device_control(SPDIF_RX,APB1,16)
__declare_device_control(SPI3_I2S3,APB1,15)
__declare_device_control(SPI2_I2S2,APB1,14)
__declare_device_control(WWDG,APB1,11)
__declare_device_control(TIM14,APB1,8)
__declare_device_control(TIM13,APB1,7)
__declare_device_control(TIM12,APB1,6)
__declare_device_control(TIM7,APB1,5)
__declare_device_control(TIM6,APB1,4)
__declare_device_control(TIM5,APB1,3)
__declare_device_control(TIM4,APB1,2)
__declare_device_control(TIM3,APB1,1)
__declare_device_control(TIM2,APB1,0)





#endif


















