#include "power.h"
#include "devices.h"
#include "cm4.h"

 
void set_voltage_scale(VOS scale) {
  enable_PWR();
  PWR.CR = (PWR.CR & ~(3<<14)) |((scale&0x3)<<14);
  __DSB();
}

VOS get_voltage_scale() {
  enable_PWR();
  return ((PWR.CR>>14)&0x3);
}

void start_Overdrive() {
  enable_PWR();
  /* Start overdrive */
  PWR.CR |= (1<<16);
  while ((PWR.CSR&(1<<16))==0);
  /* Switch to overdrive */
  PWR.CR |= (1<<17);
  while ((PWR.CSR&(1<<17))==0);
}

void stop_Overdrive() {
  enable_PWR();
  /* Stop overdrive */
  PWR.CR &= ~((1<<16)|(1<<17));
  __DSB();
}
