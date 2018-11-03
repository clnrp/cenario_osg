/*
 * serial.h
 *
 *  Created on: 22/04/2011
 *      Author: cleoner
 */

//#ifndef SERIAL_H_
//#define SERIAL_H_

#ifndef SERIAL_GLOBAL
#define SERIAL_GLOBAL extern
#else
#define SERIAL_GLOBAL
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>

#define BAUDRATE B115200
#define MODEMDEVICE "/dev/ttyACM0"
#define _POSIX_SOURCE 1 /* Fonte de acordo com POSIX */
#define FALSE 0
#define TRUE 1

class cSerial{

private:
	int fd;
	char buffer[255];
	boost::thread *bthread;
	bool isInitThread;

	void threadSerial();

public:
	int initSerial();

	cSerial(){
		isInitThread=false;
	}

	~cSerial(){
		if(isInitThread)
		    delete bthread;
	}
};

extern void get_serial_data(const char buffer[]);

//#endif /* SERIAL_H_ */
