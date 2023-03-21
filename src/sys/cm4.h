#ifndef __CM4_BUILTIN_H
#define __CM4_BUILTIN_H

#include<stdint.h>

#define __STATIC_INLINE static inline
#define __ASM asm
#define __CORTEX_M 0x04U
#define __FPU_PRESENT 1U
#define __FPU_USED 1U

/* explicit conversion for use in printf */ 
static inline __attribute__((always_inline)) double ftod(float f) {
  return f;
}

#include "cmsis_gcc.h"

#endif
