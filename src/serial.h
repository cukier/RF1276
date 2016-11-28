/*
 * serial.h
 *
 *  Created on: 07/07/2015
 *      Author: cuki
 */

#ifndef SERIAL_H_
#define SERIAL_H_

extern int serial_open_port(char *porta);
extern int serial_set_port(int baud_rate, int fd);
extern int serial_transaction(int fd, unsigned char *request,
		unsigned char *response, int req_size, int res_size);
extern int serial_close(int fd);

#endif /* SERIAL_H_ */
