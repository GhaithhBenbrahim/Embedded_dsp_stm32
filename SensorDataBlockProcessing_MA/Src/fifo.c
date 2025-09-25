/*
 * fifo.c
 *
 *  Created on: Sep 22, 2025
 *      Author: benbr
 */


#include "fifo.h"


rx_data_t RX_FIFO[RXFIFOSIZE];

volatile rx_data_t * rx_put_pt;
volatile rx_data_t * rx_get_pt;

/*Initialize the fifo*/
void rx_fifo_init(void)
{
	//Reset fifo
	rx_put_pt = rx_get_pt = &RX_FIFO[0];
}

/*put data into rx fifo*/
uint8_t rx_fifo_put(rx_data_t data)
{
	rx_data_t volatile *rx_next_put_pt;

	rx_next_put_pt = rx_put_pt + 1;

	/*Check if at the end*/
	if (rx_next_put_pt == &RX_FIFO[RXFIFOSIZE])
	{
		/*Wrap it around circular*/
		rx_next_put_pt = &RX_FIFO[0];
	}

	if (rx_next_put_pt == rx_get_pt)
	{
		return RXFIFOFAIL;

	}else
	{
		/*Put data into fifo*/
		*rx_put_pt = data ;
		rx_put_pt = rx_next_put_pt;

		return RXFIFOSUCCUSS;
	}

}

/*Get data from fifo*/
uint8_t rx_fifo_get(rx_data_t * datapt)
{
	/*Check if fifo is empty*/
	if(rx_put_pt == rx_get_pt)
	{
		/*Fifo empty*/
		return RXFIFOFAIL;
	}

	/*get the data*/
	*datapt = *rx_get_pt;

	rx_get_pt++;

	/*Check if at the end*/
	if (rx_get_pt == &RX_FIFO[RXFIFOSIZE])
	{
		/*Wrap it around circular*/
		rx_get_pt = &RX_FIFO[0];
	}

	return RXFIFOSUCCUSS ;
}





