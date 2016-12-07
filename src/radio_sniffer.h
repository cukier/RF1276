/*
 * radio_sniffer.h
 *
 *  Created on: 7 de dez de 2016
 *      Author: cuki
 */

#ifndef SRC_RADIO_SNIFFER_H_
#define SRC_RADIO_SNIFFER_H_

#include "RF1276.h"

baud_rate_t discover_radio(int fd, radio_data_t *data);
int sniff(char *porta);

#endif /* SRC_RADIO_SNIFFER_H_ */
