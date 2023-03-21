#include"clock.h"
#include"devices.h"
#include "cm4.h"

void start_PLL(PLLSRC SRC,
		 int32_t M,int32_t N,int32_t P,int32_t Q,int32_t R) { 
  /* setup parameters */
  RCC.PLLCFGR = 
    (M<<0)    | (N<<6)  | ((P/2-1)<<16) |
    (SRC<<22) | (Q<<24) | (R<<28) ;
  
  /* Switch on PLL : PLLON=1; */ 
  RCC.CR |= 1<<24;
  
  /* wait for PLLRDY==1; */
  while ((RCC.CR&(1<<25))==0);
}  

void stop_PLL() {
  /* Switch off PLL : PLLON=0; */ 
  RCC.CR &= ~(1<<24);
  __DSB();
}  

void set_SYSCLK(SW SRC) {
  RCC.CFGR = (RCC.CFGR & ~(0x3<<0)) | ((SRC & 0x3)<<0);
  while (((RCC.CFGR>>2) & 3)!=SRC);
}

void set_bus_clock_dividers_exp(uint32_t HPRE__exp,
				  uint32_t PPRE1_exp,
				  uint32_t PPRE2_exp) {
  uint32_t HPRE =(HPRE__exp==0?0:(0x8|((HPRE__exp-1) & 0x7)));
  uint32_t PPRE1=(PPRE1_exp==0?0:(0x4|((PPRE1_exp-1) & 0x3)));
  uint32_t PPRE2=(PPRE2_exp==0?0:(0x4|((PPRE2_exp-1) & 0x3)));
  RCC.CFGR=
    (RCC.CFGR & ~0xfcf0) | 
    ((HPRE<<4) | (PPRE1<<10) | (PPRE2<<13));
}

void set_MCO(MCO1SRC SRC1,uint32_t DIV1,MCO2SRC SRC2,uint32_t DIV2) {
  uint32_t MCO1=SRC1&0x3;
  uint32_t MCO2=SRC2&0x3;
  uint32_t MCO1PRE=((DIV1<=1)?0:((DIV1+2)&0x7));
  uint32_t MCO2PRE=((DIV2<=1)?0:((DIV2+2)&0x7));
  RCC.CFGR = (RCC.CFGR & ~0xff600000) |
    ((MCO1<<21) | (MCO1PRE<<24) |
     (MCO2PRE<<27) | (MCO2<<30));
}

void enable_MCO_GPIO() {

  enable_GPIOA();
  enable_GPIOC();

  /* setup PA8,PC9 alternate function : MCO1 */
  /* AFRH8=0; */
  GPIOA.AFRH &= ~0x0000000f;
  GPIOC.AFRH &= ~0x000000f0;

  /* switch PA8,PC9 to alternate function mode */
  /* MODERx=2; */
  GPIOA.MODER = 
    (GPIOA.MODER & ~(3<<16)) | (2<<16);
  GPIOC.MODER = 
    (GPIOC.MODER & ~(3<<18)) | (2<<18);
}

uint32_t get_PLL_PCLK() {
  uint32_t pllcfgr=RCC.PLLCFGR;
  uint32_t 
    M=(pllcfgr>>0)&0x3f,
    N=(pllcfgr>>6)&0x1ff,
    P=((pllcfgr>>16)&0x3)*2+2;
  PLLSRC SRC=(pllcfgr>>22)&0x1;
  if (SRC==PLLSRC_HSI) {
    return (HSI_FREQ*MHz/M*N/P);
  } else {
    return (HSE_FREQ*MHz/M*N/P);
  }
}

uint32_t get_PLL_RCLK() {
  uint32_t pllcfgr=RCC.PLLCFGR;
  uint32_t 
    M=(pllcfgr>>0)&0x3f,
    N=(pllcfgr>>6)&0x1ff,
    R=(pllcfgr>>28)&0x7;
  PLLSRC SRC=(pllcfgr>>22)&0x1;
  if (SRC==PLLSRC_HSI) {
    return (HSI_FREQ*MHz/M*N/R);
  } else {
    return (HSE_FREQ*MHz/M*N/R);
  }
}

uint32_t get_SYSCLK() {
  SW SWS=(RCC.CFGR>>2)&0x3;
  switch (SWS) {
  case SW_HSI:
    return (HSI_FREQ*MHz);
  case SW_HSE:
    return (HSE_FREQ*MHz);
  case SW_PLL_P:
    return get_PLL_PCLK();
  case SW_PLL_R:
    return get_PLL_RCLK();
  }
  return 0;
}

uint32_t get_AHBCLK() {
  uint32_t cfgr=RCC.CFGR;
  if ((cfgr&(1<<7))==0) {
    return (get_SYSCLK());
  } else {
    return(get_SYSCLK() >> (((cfgr>>4)&0x7)+1));
  } 
}

uint32_t get_APB1CLK() {
  uint32_t cfgr=RCC.CFGR;
  if ((cfgr&(1<<12))==0) {
    return (get_AHBCLK());
  } else {
    return(get_AHBCLK() >> (((cfgr>>10)&0x3)+1));
  } 
}

uint32_t get_APB2CLK() {
  uint32_t cfgr=RCC.CFGR;
  if ((cfgr&(1<<15))==0) {
    return (get_AHBCLK());
  } else {
    return(get_AHBCLK() >> (((cfgr>>13)&0x3)+1));
  } 
}

uint32_t get_APB1TIMCLK() {
  uint32_t cfgr=RCC.CFGR;
  if ((cfgr&(1<<12))==0) {
    return (get_APB1CLK());
  } else {
    return (get_APB1CLK()*2);
  } 
}

uint32_t get_APB2TIMCLK() {
  uint32_t cfgr=RCC.CFGR;
  if ((cfgr&(1<<15))==0) {
    return (get_APB2CLK());
  } else {
    return (get_APB2CLK()*2);
  } 
}

