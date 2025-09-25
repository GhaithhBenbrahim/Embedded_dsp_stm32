/*
 * fir_filter.h
 *
 *  Created on: Sep 20, 2025
 *      Author: benbr
 */

#ifndef FIR_FILTER_H_
#define FIR_FILTER_H_

#include <stdint.h>
#include "arm_math.h"

#define FIR_FILTER_LEN 32

typedef struct
{
	float32_t *buff;
	uint32_t buff_idx;
	float32_t *kernel;
	uint32_t kernel_len;
	float32_t out;
}fir_filter_t;

void fir_fiter_init(fir_filter_t *fir ,float32_t *fltr_kernel, uint32_t fltr_kernel_len);
float32_t fir_filter_update(fir_filter_t *fir, float32_t curr_sample)
;

#endif /* FIR_FILTER_H_ */
