/*
 * RF1279.h
 *
 *  Created on: Sep 8, 2015
 *      Author: cuki
 */

#ifndef RF1279_H_
#define RF1279_H_

#define data_size 		12
#define command_size	23
#define header_size		8

typedef enum command_yy_enum {
	cmd_write = 1, cmd_read, cmd_standard, cmd_central, cmd_node
} command_yy_t;

typedef enum command_xx_enum {
	cmd_xx_response = 0x00, cmd_xx_sending = 0x80
} command_xx_t;

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
	mode_standard, mode_low_power, mode_sleep
} radio_mode_t;

typedef enum rf_bw_enum {
	bw_625k = 6, bw_125k, bw_256k, bw_512k
} rf_bw_t;

typedef enum rf_power_enum {
	p_4dBm = 1, p_7dBm, p_10dBm, p_13dBm, p_14dBm, p_17dBm, p_20dBm
} rf_power_t;

typedef struct radio_data_str {
	baud_rate_t baudrate;
	parity_t parity;
	float frequencie;
	rf_factor_t rf_factor;
	radio_mode_t mode;
	rf_bw_t rf_bw;
	unsigned short id;
	unsigned char net_id;
	rf_power_t rf_power;
} radio_data_t;

extern unsigned char crc(unsigned char *data, unsigned char size);
extern unsigned char * make_radio_request(command_yy_t cmd_type,
		unsigned char *data, int size);
extern unsigned char * make_radio_read_command(int size);
extern int make_radio_read_transaction(int fd, unsigned char * response);
extern float uchartofreq(unsigned char freq_hl, unsigned char freq_lh,
		unsigned char freq_ll);
extern radio_data_t get_radio_data(int fd);
extern unsigned char touchar(int in, int index);
extern unsigned char * freqtouchar(float frequencie);
extern unsigned char * make_radio_write_command(radio_data_t data);
extern int make_radio_write_transaction(int fd, radio_data_t data);
extern int write_radio_baudrate(int fd, baud_rate_t baudrate);
extern int write_radio_parity(int fd, parity_t parity);
extern int write_radio_frequencie(int fd, float frequencie);
extern int write_radio_rf_factor(int fd, rf_factor_t rf_factor);
extern int write_radio_mode(int fd, radio_mode_t mode);
extern int write_radio_rf_bw(int fd, rf_bw_t rf_bw);
extern int write_radio_id(int fd, unsigned short id);
extern int write_radio_net_id(int fd, unsigned char net_id);
extern int write_radio_rf_power(int fd, rf_power_t power);

#endif /* RF1279_H_ */
