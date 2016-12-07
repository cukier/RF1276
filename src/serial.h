/*
 * serial.h
 *
 *  Created on: 07/07/2015
 *      Author: cuki
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdint.h>

extern int serial_open_port(char *porta);
extern int serial_set_port(int baud_rate, int fd);
extern int serial_transaction(int fd, uint8_t *request, uint8_t *response,
		uint16_t req_size, uint16_t res_size);
extern int serial_close(int fd);

#endif /* SERIAL_H_ */
