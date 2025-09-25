/*
 * fir_filter.c
 *
 *  Created on: Sep 20, 2025
 *      Author: benbr
 */

#include <stdlib.h>
#include "fir_filter.h"

void fir_fiter_init(fir_filter_t *fir ,float32_t *fltr_kernel, uint32_t fltr_kernel_len)
{
	/*Reset buffer index */
	fir->buff_idx = 0;

	/*Clear output*/
	fir->out = 0.00f;

	/*Set filter kernel*/
	fir->kernel = fltr_kernel;

	/*Set filter kernel length*/
	fir->kernel_len = fltr_kernel_len;

	/*Clear Buffer*/
	fir->buff = (float32_t *) calloc(fir->kernel_len, sizeof(float32_t));
	/*for (int i = 0; i < fltr_kernel_len; i++ )
	{
		fir->buff[i] = 0;
	}*/

}

float32_t fir_filter_update(fir_filter_t *fir, float32_t curr_sample)
{
	uint32_t sum_idx;

	/*Store latest sample in the buffer*/
	fir->buff[fir->buff_idx] = curr_sample;

	/*Increment buff index and wrap around if at the end*/
	fir->buff_idx++;

	if (fir->buff_idx == fir->kernel_len)
	{
		fir->buff_idx = 0;
	}

	/*Perform Convolution*/
	fir->out = 0.00f;

	sum_idx = fir->buff_idx;

	for(int i = 0; i< fir->kernel_len; i++)
	{
		if (sum_idx > 0)
		{
			sum_idx -- ;
		}
		else
		{
			sum_idx = (fir->kernel_len) -1 ;
		}
		fir->out = fir->kernel[i]*fir->buff[sum_idx];

	}

	return fir->out;




}
