/*
 * RF1279.c
 *
 *  Created on: Sep 8, 2015
 *      Author: cuki
 */

#include <stdlib.h>
#include <serial.h>

#include "RF1279.h"

unsigned char crc(unsigned char *data, unsigned char size) {
	int acumulo, cont;

	acumulo = 0;
	for (cont = 0; cont < size; ++cont)
		acumulo += data[cont];

	return (unsigned char) acumulo % 256;
}

unsigned char * make_radio_request(command_yy_t cmd_type, unsigned char *data,
		int size) {

	unsigned char *ret;
	unsigned char aux[size + 8];
	int cont;

	ret = (unsigned char *) malloc(size + 11);

	ret[0] = 0xAF;
	ret[1] = 0xAF;
	ret[2] = 0x00;
	ret[3] = 0x00;
	ret[4] = 0xAF;
	ret[5] = (unsigned char) cmd_xx_sending;
	ret[6] = (unsigned char) cmd_type;
	ret[7] = (unsigned char) size;

	for (cont = 0; cont < size; ++cont)
		ret[cont + 8] = data[cont];

	for (cont = 0; cont < sizeof(aux); ++cont)
		aux[cont] = ret[cont];

	ret[size + 8] = crc(aux, sizeof(aux));
	ret[size + 9] = 0x0D;
	ret[size + 10] = 0x0A;

	return ret;
}

unsigned char * make_radio_read_command(int size) {
	int cont;
	unsigned char data[size];

	for (cont = 0; cont < size; ++cont)
		data[cont] = 0;

	return make_radio_request(cmd_read, data, size);
}

int make_radio_read_transaction(int fd, unsigned char * response) {
	unsigned char *request;
	int cont;

	response = malloc(command_size);
	request = make_radio_read_command(data_size);

	if (make_serial_transaction(fd, request, response,
	command_size) == EXIT_FAILURE)
		return EXIT_FAILURE;

	for (cont = 0; cont < 8; ++cont)
		if ((cont == 5 && request[cont] != 0)
				|| (cont != 5 && response[cont] != request[cont]))
			return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

float uchartofreq(unsigned char freq_hl, unsigned char freq_lh,
		unsigned char freq_ll) {
	int aux = 0;
	float ret = 0;

	aux &= freq_hl << 24;
	aux &= freq_lh << 16;
	aux &= freq_ll;

	ret = (float) aux * 0.000061035;

	return ret;
}

radio_data_t get_radio_data(int fd) {
	static radio_data_t data;
	unsigned char *response;

	response = malloc(command_size);
	data.baudrate = 0;

	if (make_radio_read_transaction(fd, response) == EXIT_FAILURE)
		return data;

	data.baudrate = (baud_rate_t) response[header_size + 0];
	data.parity = (parity_t) response[header_size + 1];
	data.frequencie = uchartofreq(response[header_size + 2],
			response[header_size + 3], response[header_size + 4]);
	data.rf_factor = (rf_factor_t) response[header_size + 5];
	data.mode = (radio_mode_t) response[header_size + 6];
	data.rf_bw = (rf_bw_t) response[header_size + 7];
	data.id = (response[header_size + 8] << 8) & response[header_size + 9];
	data.net_id = response[header_size + 10];
	data.rf_power = (rf_power_t) response[header_size + 11];

	return data;
}

unsigned char touchar(int in, int index) {
	int mask, aux;

	mask = 0xFF << index * 8;
	aux = in & mask;
	aux >>= index * 8;
	aux &= 0xFF;

	return (unsigned char) aux;
}

unsigned char * freqtouchar(float frequencie) {
	unsigned char *ret;
	int aux, cont;

	ret = (unsigned char *) malloc(3);
	aux = (int) ((float) frequencie * 1E6 / 61.035);

	for (cont = 0; cont < 3; ++cont)
		ret[cont] = touchar(aux, cont);

	return ret;
}

unsigned char * make_radio_write_command(radio_data_t data) {
	unsigned char aux[data_size];

	aux[0] = (unsigned char) data.baudrate;
	aux[1] = (unsigned char) data.parity;
	aux[2] = freqtouchar(data.frequencie)[0];
	aux[3] = freqtouchar(data.frequencie)[1];
	aux[4] = freqtouchar(data.frequencie)[2];
	aux[5] = (unsigned char) data.rf_factor;
	aux[6] = (unsigned char) data.mode;
	aux[7] = (unsigned char) data.rf_bw;
	aux[8] = ((data.id & 0xFF00) >> 8) & 0xFF;
	aux[9] = data.id & 0xFF;
	aux[10] = data.net_id;
	aux[11] = (unsigned char) data.rf_power;

	return make_radio_request(cmd_write, aux, data_size);
}

int make_radio_write_transaction(int fd, radio_data_t data) {
	unsigned char *request, *response;
	int cont;

	response = malloc(command_size);
	request = make_radio_write_command(data);

	if (make_serial_transaction(fd, request, response,
	command_size) == EXIT_FAILURE)
		return EXIT_FAILURE;

	for (cont = 0; cont < 8; ++cont)
		if ((cont == 5 && request[cont] != 0)
				|| (cont != 5 && response[cont] != request[cont]))
			return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int write_radio_baudrate(int fd, baud_rate_t baudrate) {
	radio_data_t data;

	data = get_radio_data(fd);

	if (data.baudrate == 0)
		return EXIT_FAILURE;

	data.baudrate = baudrate;

	return make_radio_write_transaction(fd, data);
}

int write_radio_parity(int fd, parity_t parity) {
	radio_data_t data;

	data = get_radio_data(fd);

	if (data.baudrate == 0)
		return EXIT_FAILURE;

	data.parity = parity;

	return make_radio_write_transaction(fd, data);
}

int write_radio_frequencie(int fd, float frequencie) {
	radio_data_t data;

	data = get_radio_data(fd);

	if (data.baudrate == 0)
		return EXIT_FAILURE;

	data.frequencie = frequencie;

	return make_radio_write_transaction(fd, data);
}

int write_radio_rf_factor(int fd, rf_factor_t rf_factor) {
	radio_data_t data;

	data = get_radio_data(fd);

	if (data.baudrate == 0)
		return EXIT_FAILURE;

	data.rf_factor = rf_factor;

	return make_radio_write_transaction(fd, data);
}

int write_radio_mode(int fd, radio_mode_t mode) {
	radio_data_t data;

	data = get_radio_data(fd);

	if (data.baudrate == 0)
		return EXIT_FAILURE;

	data.mode = mode;

	return make_radio_write_transaction(fd, data);
}

int write_radio_rf_bw(int fd, rf_bw_t rf_bw) {
	radio_data_t data;

	data = get_radio_data(fd);

	if (data.baudrate == 0)
		return EXIT_FAILURE;

	data.rf_bw = rf_bw;

	return make_radio_write_transaction(fd, data);
}

int write_radio_id(int fd, unsigned short id) {
	radio_data_t data;

	data = get_radio_data(fd);

	if (data.baudrate == 0)
		return EXIT_FAILURE;

	data.id = id;

	return make_radio_write_transaction(fd, data);
}

int write_radio_net_id(int fd, unsigned char net_id) {
	radio_data_t data;

	data = get_radio_data(fd);

	if (data.baudrate == 0)
		return EXIT_FAILURE;

	data.net_id = net_id;

	return make_radio_write_transaction(fd, data);
}

int write_radio_rf_power(int fd, rf_power_t power) {
	radio_data_t data;

	data = get_radio_data(fd);

	if (data.baudrate == 0)
		return EXIT_FAILURE;

	data.rf_power = power;

	return make_radio_write_transaction(fd, data);
}
