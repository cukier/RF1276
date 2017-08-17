/*
 * main.c
 *
 *  Created on: 28 de nov de 2016
 *      Author: cuki
 */

#include "serial.h"
#include "RF1276.h"
#include "radio_sniffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
	char message[8] = { 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0xC5, 0xCD };

	if (argc == 1) {
		fprintf(stderr, "Definir uma porta de comunicacao\n");
		return -1;
	}

	sniff(argv[1], message, 8);

	return 0;
}
