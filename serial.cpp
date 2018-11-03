/*
 * serial.cpp
 *
 *  Created on: 22/04/2011
 *      Author: cleoner
 */

#include "serial.h"

int cSerial::initSerial(){
	int c, res;
	struct termios oldtio,newtio;

	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY);

	if (fd <0) {
		return 0;
		//perror(MODEMDEVICE);
	}

	tcgetattr(fd, &oldtio); /* Armazena settings correntes de porta */

	//bzero(&newtio, sizeof(newtio));
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;

	/* set input mode (non-canonical, no echo,...) */
	newtio.c_lflag = 0;

	newtio.c_cc[VTIME]    = 0;   /* Temporizador inter-caracteres não usado */
	newtio.c_cc[VMIN]     = 1;   /* Bloqueando `read' até que 5 caracteres sejam
								    recebidos */

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);

	bthread = new boost::thread(&cSerial::threadSerial,this);
	isInitThread = true;
	return 1;
}

void cSerial::threadSerial(){
	int res;
    printf("thread serial\n");

    while(true){
    	res = read(fd, buffer, 255);   /* Retorna após 5 caracteres terem entrado */
    	get_serial_data(buffer);
	}
}
