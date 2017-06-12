/*
 * Clock configuration for nucleo board with STM32F401xe chip
 * use HSE and PLL for maximal frequency supported by the chip
 * core clock = 84 MHz
 * APB1 Clock = 42 MHz
 * APB2 Clock = 84 MHz
 */


#include <stm32f4xx.h>


void set_clk(void)
{
    /* define port instance */
    RCC_TypeDef * rcc = RCC;
    FLASH_TypeDef * flash = FLASH;

	PWR->CR |= PWR_CR_VOS_1;

	rcc->CR |= RCC_CR_HSEBYP; /* external oscillator */
    /* enable clock using HSE instead of HSI*/   
    rcc->CR |= RCC_CR_HSEON  ;/*1<<16  HSE ON | RCC_CR_HSEBYP*/
    /* wait for HSE to be stable */
	while ((rcc->CR & RCC_CR_HSERDY) == 0) ;

    /* PLL configuration */
    rcc->PLLCFGR &= ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ );
    rcc->PLLCFGR |=  (RCC_PLLCFGR_PLLM & (8<<0))
                        | (RCC_PLLCFGR_PLLN & (336<<6))
                        | (RCC_PLLCFGR_PLLP & (1 << 16))
                        | (RCC_PLLCFGR_PLLQ & (7<<24));
	rcc->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE ;
                        
    /* PLL enable */
    rcc->CR |= RCC_CR_PLLON;
    /* wait for the PLL to be stable */
    while ((rcc->CR & RCC_CR_PLLRDY) == 0) ;
    
    /* increase the flash WS first */
    flash->ACR = (flash->ACR & (~FLASH_ACR_LATENCY)) | FLASH_ACR_LATENCY_2WS ;

    /* modify CPU clock sources */
//	while ((rcc->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){
		rcc->CFGR &= ~RCC_CFGR_SW;
		rcc->CFGR |= RCC_CFGR_SW_PLL ;
//	}
		/* switch off HCI */
		rcc->CR &= ~RCC_CR_HSION;
    /* modify AHB divisor (no DIV)*/
    rcc->CFGR = (rcc->CFGR & (~RCC_CFGR_HPRE))| RCC_CFGR_HPRE_DIV1 ;
    /* Modify APB1 (low speed) divisor DIV2 */ 
    rcc->CFGR = (rcc->CFGR & (~RCC_CFGR_PPRE1))| RCC_CFGR_PPRE1_DIV2 ;
     /* Modify APB2 (high speed) no div */ 
    rcc->CFGR = (rcc->CFGR & (~RCC_CFGR_PPRE2))| RCC_CFGR_PPRE2_DIV1 ;
  
    SystemCoreClockUpdate(); //SystemCoreClock = 84000000; ((HSE_VALUE / PLLM) / PLLP) * PLLN / HPRE_DIVN
}

