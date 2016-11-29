/*
 * main.c
 *
 *  Created on: 28 de nov de 2016
 *      Author: cuki
 */

#include "serial.h"
#include "RF1276.h"
#include <stdio.h>

int main(int argc, char **argv) {
	int fd, r;
	radio_data_t radio_d;

	fd = serial_open_port("/dev/ttyS1");

	if (fd == -1)
		return -1;

	r = serial_set_port(9600, fd);

	if (r == -1)
		return -1;

//	r = serial_transaction(fd, req, resp, 8, 25);
	r = RF1276_get_radio_data(fd, &radio_d);

	if (r == -1)
		return -1;

	r = RF1276_get_radio_rssi(fd);

	printf("Rssi: %d\n", r);

	serial_close(fd);
	return 0;
}
