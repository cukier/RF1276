/*
 * radio_sniffer.c
 *
 *  Created on: 7 de dez de 2016
 *      Author: cuki
 */

#include "radio_sniffer.h"
#include "serial.h"
#include "RF1276.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

baud_rate_t discover_radio(int fd, radio_data_t *data) {
	baud_rate_t baud;
	int r;

	baud = B1200BPS;

	do {

		switch (baud) {
		case B1200BPS:
			r = serial_set_port(1200, fd);
			break;
		case B2400BPS:
			r = serial_set_port(2400, fd);
			break;
		case B4800BPS:
			r = serial_set_port(4800, fd);
			break;
		default:
		case B9600BPS:
			r = serial_set_port(9600, fd);
			break;
		case B19200BPS:
			r = serial_set_port(19200, fd);
			break;
		case B38400BPS:
			r = serial_set_port(38400, fd);
			break;
		case B57600BPS:
			r = serial_set_port(57600, fd);
			break;
		case B115200PS:
			r = serial_set_port(115200, fd);
			break;
		}

		if (r == -1)
			return -1;

		//	r = serial_transaction(fd, req, resp, 8, 25);
		r = RF1276_get_radio_data(fd, data);
		usleep(100000);

		if (r == EXIT_FAILURE) {
			fprintf(stderr, "Nao houve resposta do radio\n");
			baud++;

			if (baud == BINVPS) {
				fprintf(stderr, "\nRadio nao encontrado\n");
			}
		}

	} while (r == EXIT_FAILURE && baud != BINVPS);

	return baud;
}

int sniff(char *porta) {
	int fd, r;
	radio_data_t radio_d;

	fd = serial_open_port(porta);

	if (fd == -1) {
		fprintf(stderr, "Porta nao encontrada: %s\n", porta);
		return -1;
	}

	if (discover_radio(fd, &radio_d) != BINVPS) {
		RF1276_parse_radio(radio_d);
		r = RF1276_get_radio_rssi(fd);
		printf("Rssi: %d\n", r);
	}

	serial_close(fd);
	return 0;
}
