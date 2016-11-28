/*
 * RF1276.h
 *
 *  Created on: Sep 8, 2015
 *      Author: cuki
 */

#ifndef RF1276_H_
#define RF1276_H_

#include <stdint.h>

typedef enum command_yy_enum {
	CMD_WRITE = 1, CMD_READ, CMD_STANDARD, CMD_CENTRAL, CMD_NODE
} command_yy_t;

typedef enum command_xx_enum {
	CMD_XX_RESPONSE = 0x00, CMD_XX_SENDING = 0x80
} command_xx_t;

typedef enum baud_rate_enum {
	B1200BPS = 1, B2400BPS, B4800BPS, B9600BPS, B19200BPS, B38400BPS, B57600BPS
} baud_rate_t;

typedef enum parity_enum {
	NO_PARITY, ODD_PARITY, EVEN_PARITY
} parity_t;

typedef enum rf_factor_enum {
	RF_128 = 7, RF_256, RF_512, RF_1024, RF_2048, RF_4096
} rf_factor_t;

typedef enum mode_enum {
	MODE_STANDARD, MODE_LOW_POWER, MODE_SLEEP
} radio_mode_t;

typedef enum rf_bw_enum {
	BW_62_5K = 6, BW_125K, BW_250K, BW_500K
} rf_bw_t;

typedef enum rf_power_enum {
	P_4DBM = 1, P_7DBM, P_10DBM, P_13DBM, P_14DBM, P_17DBM, P_20DBM
} rf_power_t;

typedef struct radio_data_str {
	baud_rate_t baudrate;
	parity_t parity;
	float frequencie;
	rf_factor_t rf_factor;
	radio_mode_t mode;
	rf_bw_t rf_bw;
	uint16_t id;
	uint8_t net_id;
	rf_power_t rf_power;
} radio_data_t;

extern int RF1276_get_radio_data(int fd, radio_data_t *data);
extern int RF1276_write_radio_baudrate(int fd, baud_rate_t baudrate);
extern int RF1276_write_radio_parity(int fd, parity_t parity);
extern int RF1276_write_radio_frequencie(int fd, float frequencie);
extern int RF1276_write_radio_rf_factor(int fd, rf_factor_t rf_factor);
extern int RF1276_write_radio_mode(int fd, radio_mode_t mode);
extern int RF1276_write_radio_rf_bw(int fd, rf_bw_t rf_bw);
extern int RF1276_write_radio_id(int fd, uint16_t id);
extern int RF1276_write_radio_net_id(int fd, uint8_t net_id);
extern int RF1276_write_radio_rf_power(int fd, rf_power_t power);

#endif /* RF1279_H_ */
