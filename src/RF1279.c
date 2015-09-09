/*
 * RF1279.c
 *
 *  Created on: Sep 8, 2015
 *      Author: cuki
 */

#include "RF1279.h"

unsigned char crc(unsigned char *data, unsigned char size) {
	int acumulo, cont;

	acumulo = 0;
	for (cont = 0; cont < size; ++cont)
		acumulo += data[cont];

	return (unsigned char) acumulo % 256;
}

int make_radio_read_request(unsigned char *request) {
	int cont;
	unsigned char aux[20];
	read_t read;

	for (cont = 0; cont < data_size; ++cont)
		read.str.data[cont] = 0;

	read.str.sync_word = 0xAFAF;
	read.str.id_code = 0x0000;
	read.str.header = 0xAF;
	read.str.command_xx = 0x80;
	read.str.command_yy = (unsigned short) cmd_read;
	read.str.length = data_size;

	for (cont = 0; cont < sizeof(aux); ++cont)
		aux[cont] = read.raw[cont];

	read.str.cs = crc(aux, sizeof(aux));
	read.str.end_code = (unsigned short) 0x0D0A;

	for (cont = 0; cont < sizeof(read_command_str_t); ++cont)
		request[cont] = read.raw[cont];

	return cont;
}
