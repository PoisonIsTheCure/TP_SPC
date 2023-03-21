#include "serial_io.h"
#include "clock.h"
#include "devices.h"
#include "cm4.h"
#include <stdio.h>

void enable_USART2_GPIO() {
  /*  Enable GPIOA  */

  enable_GPIOA();

  /* setup PA2,PA3 alternate function : USART2_Tx/Rx */
  /* AFRL2=7; AFRL3=7; */

  GPIOA.AFRL = 
    (GPIOA.AFRL & ~((15<<8) | (15<<12))) | ((7<<8)|(7<<12));

  /* switch PA2,PA3 to alternate function mode */
  /* MODER2=2; MODER3=2; */

  GPIOA.MODER = 
    (GPIOA.MODER & ~((3<<4)|(3<<6))) | ((2<<4)|(2<<6));
}

void setup_USART2(uint32_t baudrate) {
  /*  Enable USART2 */

  enable_USART2();

  /* Setup baudrate (OVER8=0) */
  USART2.BRR = get_APB1CLK()/baudrate;

  /* SC OFF, IR OFF,HD OFF */
  /* SCEN=0; IREN=0; HDSEL=0; */  

  USART2.CR3 = (0<<1)|(0<<3)|(0<<5);

  /* LIN OFF,1 stop bit,CLK OFF */
  /* LINEN=0; STOP=0; CLKEN=0; */  

  USART2.CR2 = (0<<11)|(0<<12)|(0<<14);

  /* USART/Tx/Rx enable, no parity, 8 bits */ 
  /* RE=1; TE=1; PCE=0; M=0; OVER8=0; */

  USART2.CR1 = (1<<2) | (1<<3) | (0<<10) | (0<<12) | (1<<13) | (0<<15);
}

void serial_io_init() {
  setup_USART2(9600);
  enable_USART2_GPIO();

  /* disable input/output buffering */
  setvbuf(stdin ,NULL,_IONBF,0);
  setvbuf(stdout,NULL,_IONBF,0);
  setvbuf(stderr,NULL,_IONBF,0);
}
