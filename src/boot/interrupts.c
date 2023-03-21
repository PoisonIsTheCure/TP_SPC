
/* We define interrupt handlers as weak aliases to Default_Handler */

void Default_Handler() { while(1); }

/* ARM Cortex-M4 interrupts */

void NMI_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void HardFault_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void MemManage_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
  
void BusFault_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void UsageFault_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void SVCall_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void Debug_Monitor_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void PendSV_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void SysTick_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));

  
/* STM32F446 specific interrupts */

  
void WWDG_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void PVD_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TAMP_STAMP_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void RTC_WKUP_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void FLASH_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void RCC_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void EXTI0_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void EXTI1_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void EXTI2_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void EXTI3_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void EXTI4_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream0_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream1_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream2_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream3_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream4_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream5_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream6_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void ADC_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_TX_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_RX0_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_RX1_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_SCE_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void EXTI9_5_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TIM1_BRK_TIM9_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TIM1_UP_TIM10_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TIM1_TRG_COM_TIM11_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TIM1_CC_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TIM2_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TIM3_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TIM4_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void I2C1_EV_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void I2C1_ER_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void I2C2_EV_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void I2C2_ER_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void SPI1_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void SPI2_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void USART1_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void USART2_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void USART3_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void EXTI15_10_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void RTC_Alarm_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void OTG_FS_WKUP_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TIM8_BRK_TIM12_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TIM8_UP_TIM13_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TIM8_TRG_COM_TIM14_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TIM8_CC_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Stream7_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void FMC_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void SDIO_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TIM5_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void SPI3_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void UART4_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void UART5_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TIM6_DAC_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void TIM7_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream0_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream1_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream2_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream3_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream4_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void CAN2_TX_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void CAN2_RX0_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void CAN2_RX1_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void CAN2_SCE_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void OTG_FS_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream5_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream6_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DMA2_Stream7_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void USART6_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void I2C3_EV_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void I2C3_ER_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void OTG_HS_EP1_OUT_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void OTG_HS_EP1_IN_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void OTG_HS_WKUP_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void OTG_HS_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void DCMI_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void FPU_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void SPI4_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void SAI1_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void SAI2_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void QuadSPI_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void HDMI_CEC_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void SPDIF_Rx_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void FMPI2C1_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));
void FMPI2C1_error_Handler()
  __attribute__ ((weak, alias ("Default_Handler")));

/* We put handler addresses in main interrupt vector */

typedef void (*isr)();

const __attribute(( section("interrupt_vector") )) 
isr __interrupt_vector[] = {

/* ARM Cortex-M4 Interrupts */

  NMI_Handler,
  HardFault_Handler,
  MemManage_Handler, 
  BusFault_Handler,
  UsageFault_Handler,
  0,
  0,
  0,
  0,
  SVCall_Handler,
  Debug_Monitor_Handler,
  0,
  PendSV_Handler,
  SysTick_Handler,
 
/* STM32F446 specific interrupts */
 
  WWDG_Handler,
  PVD_Handler,
  TAMP_STAMP_Handler,
  RTC_WKUP_Handler,
  FLASH_Handler,
  RCC_Handler,
  EXTI0_Handler,
  EXTI1_Handler,
  EXTI2_Handler,
  EXTI3_Handler,
  EXTI4_Handler,
  DMA1_Stream0_Handler,
  DMA1_Stream1_Handler,
  DMA1_Stream2_Handler,
  DMA1_Stream3_Handler,
  DMA1_Stream4_Handler,
  DMA1_Stream5_Handler,
  DMA1_Stream6_Handler,
  ADC_Handler,
  CAN1_TX_Handler,
  CAN1_RX0_Handler,
  CAN1_RX1_Handler,
  CAN1_SCE_Handler,
  EXTI9_5_Handler,
  TIM1_BRK_TIM9_Handler,
  TIM1_UP_TIM10_Handler,
  TIM1_TRG_COM_TIM11_Handler,
  TIM1_CC_Handler,
  TIM2_Handler,
  TIM3_Handler,
  TIM4_Handler,
  I2C1_EV_Handler,
  I2C1_ER_Handler,
  I2C2_EV_Handler,
  I2C2_ER_Handler,
  SPI1_Handler,
  SPI2_Handler,
  USART1_Handler,
  USART2_Handler,
  USART3_Handler,
  EXTI15_10_Handler,
  RTC_Alarm_Handler,
  OTG_FS_WKUP_Handler,
  TIM8_BRK_TIM12_Handler,
  TIM8_UP_TIM13_Handler,
  TIM8_TRG_COM_TIM14_Handler,
  TIM8_CC_Handler,
  DMA1_Stream7_Handler,
  FMC_Handler,
  SDIO_Handler,
  TIM5_Handler,
  SPI3_Handler,
  UART4_Handler,
  UART5_Handler,
  TIM6_DAC_Handler,
  TIM7_Handler,
  DMA2_Stream0_Handler,
  DMA2_Stream1_Handler,
  DMA2_Stream2_Handler,
  DMA2_Stream3_Handler,
  DMA2_Stream4_Handler,
  0,
  0,
  CAN2_TX_Handler,
  CAN2_RX0_Handler,
  CAN2_RX1_Handler,
  CAN2_SCE_Handler,
  OTG_FS_Handler,
  DMA2_Stream5_Handler,
  DMA2_Stream6_Handler,
  DMA2_Stream7_Handler,
  USART6_Handler,
  I2C3_EV_Handler,
  I2C3_ER_Handler,
  OTG_HS_EP1_OUT_Handler,
  OTG_HS_EP1_IN_Handler,
  OTG_HS_WKUP_Handler,
  OTG_HS_Handler,
  DCMI_Handler,
  0,
  0,
  FPU_Handler,
  0,
  0,
  SPI4_Handler,
  0,
  0,
  SAI1_Handler,
  0,
  0,
  0,
  SAI2_Handler,
  QuadSPI_Handler,
  HDMI_CEC_Handler,
  SPDIF_Rx_Handler,
  FMPI2C1_Handler,
  FMPI2C1_error_Handler
};
