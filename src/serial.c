#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>

#ifndef TIME_OUT
#define TIME_OUT 3000000UL
#endif

int serial_open_port(char *porta) {
	int fd;

	fd = open(porta, O_RDWR | O_NOCTTY | O_NDELAY);

	if (fd == -1)
		fprintf(stderr, "Problemas ao abrir a porta \"%s\" : %s\n", porta,
				strerror(errno));

	return fd;
}

int serial_set_port(int baud_rate, int fd) {
	struct termios options;
	speed_t speed;

	switch (baud_rate) {
	case 1200:
		speed = B1200;
		break;
	case 2400:
		speed = B2400;
		break;
	case 4800:
		speed = B4800;
		break;
	default:
	case 9600:
		speed = B9600;
		break;
	case 19200:
		speed = B19200;
		break;
	case 38400:
		speed = B38400;
		break;
	case 57600:
		speed = B57600;
		break;
	case 115200:
		speed = B115200;
		break;
	}

	if ((tcgetattr(fd, &options) == -1) || (cfsetispeed(&options, speed) == -1)
			|| (cfsetospeed(&options, speed) == -1))
		return -1;

	options.c_cflag |= (CLOCAL | CREAD);		//**************************//
	options.c_cflag &= ~PARENB;					//							//
	options.c_cflag &= ~CSTOPB;					//		No parity (8N1):	//
	options.c_cflag &= ~CSIZE;					//							//
	options.c_cflag |= CS8;						//**************************//
	options.c_cflag &= ~CRTSCTS;				//disable hardware flow control
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //raw input
	options.c_iflag &= ~(IXON | IXOFF | IXANY); //disable software flow control

	if (tcsetattr(fd, TCSANOW, &options) == -1)
		return -1;

	return 0;
}

int serial_transaction(int fd, uint8_t *request, uint8_t *response,
		uint16_t req_size, uint16_t res_size) {
	int n;

	write(fd, request, req_size);
	tcflush(fd, TCIOFLUSH);
	usleep(TIME_OUT);
	n = read(fd, response, res_size);

	return n;
}

int serial_close(int fd) {
	return close(fd);
}
