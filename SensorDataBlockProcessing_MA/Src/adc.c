/*
 * adc.c
 *
 *  Created on: Aug 4, 2025
 *      Author: benbr
 */


#include "adc.h"

void pa1_adc_init(void) {
    /************** GPIO CONFIGURATION ****************/
    // 1. Enable clock access to GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // 2. Set PA1 to analog mode (MODER1 = 11)
    GPIOA->MODER |= (3U << 2);   // Bit 3:2 = 11 for PA1
    GPIOA->PUPDR &= ~(3U << 2);  // No pull-up/pull-down

    /************** ADC CONFIGURATION *****************/
    // 3. Enable clock access to ADC1
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // 4. Set ADC prescaler in ADC Common Control Register (optional but recommended)
    ADC->CCR |= (3U << 16); // PCLK2 divided by 8

    // 5. Set the sequence length to 1 conversion
    ADC1->SQR1 &= ~(0xF << 20); // L[3:0] = 0000 for 1 conversion

    // 6. Set the conversion sequence: first conversion in regular sequence = channel 1 (PA1 = ADC1_IN1)
    ADC1->SQR3 &= ~0x1F;         // Clear SQ1 bits
    ADC1->SQR3 |= 1U;            // Set SQ1 to channel 1

    // 7. Set sampling time for channel 1 (ADC_SMPR2, channels 0–9)
    ADC1->SMPR2 &= ~(7U << 3);   // Clear bits for channel 1
    ADC1->SMPR2 |= (4U << 3);    // e.g., 84 cycles (recommended for accuracy)

    // 8. Enable ADC1
    ADC1->CR2 |= ADC_CR2_ADON;
}


void start_conversion(void)
{
    // 1. Enable continuous conversion mode
    ADC1->CR2 |= ADC_CR2_CONT;

    // 2. Start ADC conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

uint32_t adc_read(void)
{
    // 1. Wait for the end of conversion (EOC) flag
    while (!(ADC1->SR & ADC_SR_EOC));

    // 2. Read converted result from ADC data register
    return ADC1->DR;
}






