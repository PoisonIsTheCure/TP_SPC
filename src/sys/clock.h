#include<stdint.h>

#define HSI_FREQ  16
#define HSE_FREQ  00
#define MHz  1000000

typedef enum {PLLSRC_HSI=0,PLLSRC_HSE=1} PLLSRC;
typedef enum {SW_HSI=0,SW_HSE=1,SW_PLL_P=2,SW_PLL_R=3} SW;
typedef enum {MCO1_HSI=0,MCO1_LSE=1,MCO1_HSE=2,MCO1_PLL=3} MCO1SRC;
typedef enum {MCO2_SYS=0,MCO2_I2S=1,MCO2_HSE=2,MCO2_PLL=3} MCO2SRC;

void start_PLL(PLLSRC SRC,
	       int32_t M,int32_t N,int32_t P,int32_t Q,int32_t R);
void stop_PLL();

void set_SYSCLK(SW SRC);
void set_bus_clock_dividers_exp(uint32_t HPRE__exp,
				  uint32_t PPRE1_exp,
				  uint32_t PPRE2_exp);

void set_MCO(MCO1SRC SRC1,uint32_t DIV1,MCO2SRC SRC2,uint32_t DIV2);
void enable_MCO_GPIO();

uint32_t get_PLL_PCLK();
uint32_t get_PLL_RCLK();
uint32_t get_SYSCLK();
uint32_t get_AHBCLK();
uint32_t get_APB1CLK();
uint32_t get_APB2CLK();
uint32_t get_APB1TIMCLK();
uint32_t get_APB2TIMCLK();

