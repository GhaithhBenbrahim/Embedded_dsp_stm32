/*
 * Clock.c
 *
 *  Created on: Aug 3, 2025
 *      Author: benbr
 */

#include "clock.h"

/*
 * system clock source = HSE
 * SYSCLK              = 100Mhz
 * HCL                 = 100Mhz
 * AHB Prescaler       = 1
 * APB1 Prescaler      = 2
 * APB2 Prescaler      = 1
 * HSE                 = 8Mhz
 */

#include "stm32f4xx.h"

void clock_100Mhz_config(void)
{
    /* 1. Enable HSE (High Speed External) */
    RCC->CR |= RCC_CR_HSEON;

    /* 2. Wait till HSE is ready */
    while (!(RCC->CR & RCC_CR_HSERDY));

    /* 3. Set Power Enable Clock and Voltage Regulator */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR |= PWR_CR_VOS;

    /* 4. Configure Flash Latency for 100 MHz:
       - 3 wait states (LATENCY = 3) for 100 MHz
       - Enable prefetch buffer, instruction cache, data cache
    */
    FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_3WS;

    /* 5. Configure the PLL
       PLL_VCO = (HSE / PLL_M) * PLL_N
               = (8 MHz / 8) * 200 = 200 MHz
       SYSCLK = PLL_VCO / PLL_P
              = 200 MHz / 2 = 100 MHz
    */
    RCC->PLLCFGR = (8 << RCC_PLLCFGR_PLLM_Pos)   |  // PLL_M = 8
                   (200 << RCC_PLLCFGR_PLLN_Pos) |  // PLL_N = 200
                   (0 << RCC_PLLCFGR_PLLP_Pos)   |  // PLL_P = 2 (00)
                   (RCC_PLLCFGR_PLLSRC_HSE);        // HSE as PLL source

    /* 6. Enable the PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* 7. Wait until PLL is ready */
    while (!(RCC->CR & RCC_CR_PLLRDY));

    /* 8. Set AHB Prescaler (HCLK) to /1 */
    RCC->CFGR &= ~RCC_CFGR_HPRE;

    /* 9. Set APB1 Prescaler to /2 (max 50 MHz) */
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

    /* 10. Set APB2 Prescaler to /1 */
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    /* 11. Select PLL as system clock source */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* 12. Wait until PLL is used as system clock source */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}







