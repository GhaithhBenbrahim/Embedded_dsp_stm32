/*
 * tim.h
 *
 *  Created on: Sep 22, 2025
 *      Author: benbr
 */

#ifndef TIM_H_
#define TIM_H_

#include "stm32f4xx.h"

#define SR_UIF   (1U<<0)

void tim2_1hz_interrupt_init(void);

#endif /* TIM_H_ */
