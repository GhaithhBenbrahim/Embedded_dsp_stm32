/*
 * tim.c
 *
 *  Created on: Sep 22, 2025
 *      Author: benbr
 */

#include "tim.h"

/*APB1 Timer clock is 100mhz*/
#define TIM2EN    (1U<<0)
#define CR1_CEN   (1U<<0)
#define DIER_UIE  (1U<<0)

void tim2_1hz_interrupt_init(void)
{
	/*Enable Clock access to TIM2*/
	RCC->APB1ENR = TIM2EN;
	/*Set the prescaler value*/
	TIM2->PSC  = 10000 -1; //100 000 000 / 10 000 = 10000
	/*Set the auto-reload value*/
	TIM2->ARR = 10 -1 ;  //10000/10
	/*Clear counter*/
	TIM2->CNT = 0 ;
	/*Enable Timer*/
	TIM2->CR1 = CR1_CEN ;
	/*Enable Timer interrupt */
	TIM2->DIER |= DIER_UIE ;
	/*Enable Timer interrupt NVIC*/
	NVIC_EnableIRQ(TIM2_IRQn);
}




