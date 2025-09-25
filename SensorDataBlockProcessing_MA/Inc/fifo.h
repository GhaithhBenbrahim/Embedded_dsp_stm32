/*
 * fifo.h
 *
 *  Created on: Sep 22, 2025
 *      Author: benbr
 */

#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>

#define RXFIFOSIZE     300
#define RXFIFOFAIL     0
#define RXFIFOSUCCUSS  1

typedef uint32_t rx_data_t ;

void rx_fifo_init(void);
uint8_t rx_fifo_put(rx_data_t data);
uint8_t rx_fifo_get(rx_data_t * datapt);

#endif /* FIFO_H_ */
