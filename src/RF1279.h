/*
 * RF1279.h
 *
 *  Created on: Sep 8, 2015
 *      Author: cuki
 */

#ifndef RF1279_H_
#define RF1279_H_

#define data_size 12

typedef enum command_yy_enum {
	cmd_write = 1, cmd_read, cmd_standard, cmd_central, cmd_node
} command_yy_t;

typedef enum baud_rate_enum {
	b1200bps = 1, b2400bps, b4800bps, b9600bps, b19200bps, b38400bps, b57600bps
} baud_rate_t;

typedef enum parity_enum {
	no_parity, odd_parity, even_parity
} parity_t;

typedef enum rf_factor_enum {
	rf_128 = 7, rf_256, rf_512, rf_1024, rf_2048, rf_4096
} rf_factor_t;

typedef enum mode_enum {
	standard, low_power, sleep
} mode_t;

typedef enum rf_bw_enum {
	bw_625k = 6, bw_125k, bw_256k, bw_512k
} rf_bw_t;

typedef enum rf_power_enum {
	p_4dBm = 1, p_7dBm, p_10dBm, p_13dBm, p_14dBm, p_17dBm, p_20dBm
} rf_power_t;

typedef struct data_str {
	baud_rate_t baudrate;
	parity_t parity;
	unsigned char float_hl;
	unsigned char float_lh;
	unsigned char float_ll;
	rf_factor_t rf_factor;
	mode_t mode;
	rf_bw_t rf_bw;
	unsigned char id_hi;
	unsigned char id_lo;
	unsigned char net_id;
	rf_power_t rf_power;
} data_str_t;

typedef struct read_command_str {
	unsigned short sync_word;
	unsigned short id_code;
	unsigned char header;
	unsigned char command_xx;
	unsigned char command_yy;
	unsigned char length;
	unsigned char data[data_size];
	unsigned char cs;
	unsigned short end_code;
} read_command_str_t;

typedef union read_command_un {
	read_command_str_t str;
	unsigned char raw[sizeof(read_command_str_t) - 1];
} read_t;

unsigned char crc(unsigned char *data, unsigned char size);
int make_radio_read_request(unsigned char *request);

#endif /* RF1279_H_ */
