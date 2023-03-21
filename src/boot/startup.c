#include<string.h>

typedef void (*fnptr)();

void hardware_init_hook() {
  extern const char __data_load__; 
  extern char __data_start__;
  extern char __data_end__;

  memcpy(&__data_start__,&__data_load__,(&__data_end__ - &__data_start__));
}

struct boot_struct {
  void* initial_SP;
  void (*initial_PC)();
};

extern char __stack;
extern void _start();

struct boot_struct __boot_sector 
__attribute__(( section("boot") ))  = 
  {&__stack,_start};
