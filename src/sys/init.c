#include "devices.h"
#include "clock.h"
#include "power.h"
#include "serial_io.h"
#include "cm4.h"


void set_FLASH_latency(int32_t latency) {
  FLASH.ACR= 
    (FLASH.ACR & ~(15<<0) & ~(7<<8)) | 
    ((((latency>0)?7:0)<<8)|((latency & 15)<<0));
  /* if latency > 0, we also enable cache and prefetch */ 
  while(((FLASH.ACR>>0)&15)!=latency);
}

void fpu_init() { 
  /* enable FPU for unpriviledged access  */
  FPU_CPACR |= (3<<20)|(3<<22);
  __DSB();
}

void enable_io_compensation() {
  enable_SYSCFG();
  SYSCFG.CMPCR|=(1<<0);
  while((SYSCFG.CMPCR & (1<<8)) == 0);
}

void clock_init() {
  set_MCO(MCO1_PLL,5,MCO2_SYS,5);
  set_voltage_scale(scale1);
  start_Overdrive();
  start_PLL(PLLSRC_HSI,16,336,2,7,7);
  set_bus_clock_dividers_exp(0,2,1);
  set_FLASH_latency(5);
  set_SYSCLK(SW_PLL_P);
  enable_io_compensation();
  enable_MCO_GPIO();
}

void software_init_hook() { /* called from crt0.S */
  fpu_init();
  clock_init();
  serial_io_init();
}

  
