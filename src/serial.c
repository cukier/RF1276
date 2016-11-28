#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <stdlib.h>

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
	default:
	case 9600:
		speed = B9600;
		break;
	case 19200:
		speed = B19200;
		break;
	}

	if ((tcgetattr(fd, &options) == -1) || (cfsetispeed(&options, speed) == -1)
			|| (cfsetospeed(&options, speed) == -1))
		return -1;

	options.c_cflag |= (CLOCAL | CREAD);	//**************************//
	options.c_cflag &= ~PARENB;				//							//
	options.c_cflag &= ~CSTOPB;				//		No parity (8N1):	//
	options.c_cflag &= ~CSIZE;				//							//
	options.c_cflag |= CS8;					//**************************//
	options.c_cflag &= ~CRTSCTS;			//disable hardware flow control
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); //raw input
	options.c_iflag &= ~(IXON | IXOFF | IXANY); //disable software flow control

	if (tcsetattr(fd, TCSANOW, &options) == -1)
		return -1;

	return 0;
}

int serial_transaction(int fd, unsigned char *request, unsigned char *response,
		int req_size, int res_size) {
	int n;

	write(fd, request, req_size);
	tcflush(fd, TCIOFLUSH);
	usleep(300000);
	n = read(fd, response, res_size);

	return n;
}

int serial_close(int fd) {
	return close(fd);
}
