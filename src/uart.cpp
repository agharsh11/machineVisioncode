#include <stdio.h>      // standard input / output functions
#include <stdlib.h>
#include <string.h>     // string function definitions
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions
#include <iostream>
#include "../include/uart.h"
using namespace std;

int checkcontour=0;
char port1c[100] = "/dev/ttyS4";
char port2c[100]= "/dev/ttyS5";



int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

void set_mincount(int fd, int mcount)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error tcgetattr: %s\n", strerror(errno));
        return;
    }

    tty.c_cc[VMIN] = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 5;        /* half second timer */

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
        printf("Error tcsetattr: %s\n", strerror(errno));
}

void uartw(int port,char a){
    int fd1 = open(port1c, O_RDWR | O_NOCTTY | O_SYNC);
    int fd2 = open(port2c, O_RDWR | O_NOCTTY | O_SYNC);

     if (fd1< 0) {
        printf("Error opening %s: %s\n", port1c, strerror(errno));
        return ;
    }

    
    if (fd2 < 0) {
        printf("Error opening %s: %s\n", port2c, strerror(errno));
        return ;
    }
set_interface_attribs(fd1, B115200);
set_interface_attribs(fd2, B115200);
set_mincount(fd1, 0);
set_mincount(fd2, 0);
    if (port==1){
        int w=write(fd1,&a,1);
            if (w != 1) 
                cout<<"Error from write!\n";
    }
    else if(port==2){
        int w=write(fd2,&a,1);
            if (w != 1) 
                cout<<"Error from write!\n";
    }

}
	
char uartr(int port){
    int fd1 = open(port1c, O_RDWR | O_NOCTTY | O_SYNC);
    int fd2 = open(port2c, O_RDWR | O_NOCTTY | O_SYNC);

    if (fd1< 0) {
        printf("Error opening %s: %s\n", port1c, strerror(errno));
        return -1;
    }
    if (fd2 < 0) {
        printf("Error opening %s: %s\n", port2c, strerror(errno));
        return -1;
    }
	set_interface_attribs(fd1, B115200);

set_mincount(fd1, 0);
set_interface_attribs(fd2, B115200);
set_mincount(fd2, 0);


    
	int rdlen;
	char buf1='\0';
        if(port==1)
       	rdlen = read(fd1, &buf1, 1);
       else
        rdlen = read(fd2, &buf1, 1);
	 if (rdlen > 0) {
		 char cu='\0';
            printf("Read %d: \"%c\"\n", 1, buf1);
		 if(buf1>'a'&&buf1<'z')
		 	return buf1;
		 else
			return cu; 
	 }
		else if (rdlen< 0) 
        	    printf("Error from read: %d: %s\n", rdlen, strerror(errno));
}
		


void uartay(int yaw){
    int fd1 = open(port1c, O_RDWR | O_NOCTTY | O_SYNC);
	 if (fd1< 0) {
        printf("Error opening %s: %s\n", port1c, strerror(errno));
        return ;
    }

    int fd2 = open(port2c, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd2 < 0) {
        printf("Error opening %s: %s\n", port2c, strerror(errno));
        return ;
    }
    
set_interface_attribs(fd1, B115200);
set_interface_attribs(fd2, B115200);
set_mincount(fd1, 0);
set_mincount(fd2, 0);
	
   
	char c='A';
		int wlen = write(fd2, &c, 1);
    		if (wlen != 1) 
        		printf("Error from write: %d, %d\n", wlen, errno);
	

	c='Y';
		wlen = write(fd2, &c, 1);
    		if (wlen != 1) 
        		printf("Error from write: %d, %d\n", wlen, errno);
	char cmd[4];
	sprintf(cmd,"%d",yaw);
	int n=strlen(cmd);
	int spot = 0;
	n=strlen(cmd);
	 do {
        wlen= write( fd2, &cmd[spot], 1 );
        spot ++;
    } while (spot<n);
	spot=0;
}

void uartao(int offset){
    int fd1 = open(port1c, O_RDWR | O_NOCTTY | O_SYNC);
    int fd2 = open(port2c, O_RDWR | O_NOCTTY | O_SYNC);
	 if (fd1< 0) {
        printf("Error opening %s: %s\n", port1c, strerror(errno));
        return ;
    }
    
	if (fd2 < 0) {
        printf("Error opening %s: %s\n", port2c, strerror(errno));
        return ;
    }
	
set_interface_attribs(fd1, B115200);
set_interface_attribs(fd2, B115200);
set_mincount(fd1, 0);
set_mincount(fd2, 0);   
	char c='A';
		int wlen = write(fd2, &c, 1);
    		if (wlen != 1) 
        		printf("Error from write: %d, %d\n", wlen, errno);
	

	c='O';
		wlen = write(fd2, &c, 1);
    		if (wlen != 1) 
        		printf("Error from write: %d, %d\n", wlen, errno);
	char cmd[4];
	sprintf(cmd,"%d",offset);
	int n=strlen(cmd);
	int spot = 0;
	n=strlen(cmd);
	 do {
        wlen= write( fd2, &cmd[spot], 1 );
        spot ++;
    } while (spot<n);
	spot=0;
}

void uartad(int deapth){
    int fd1 = open(port1c, O_RDWR | O_NOCTTY | O_SYNC);
    int fd2 = open(port2c, O_RDWR | O_NOCTTY | O_SYNC);
	 if (fd1< 0) {
        printf("Error opening %s: %s\n", port1c, strerror(errno));
        return;
    }
    
	if (fd2 < 0) {
        printf("Error opening %s: %s\n", port2c, strerror(errno));
        return ;
    }
	set_interface_attribs(fd1, B115200);
set_interface_attribs(fd2, B115200);
set_mincount(fd1, 0);
set_mincount(fd2, 0);
   
	char c='A';
		int wlen = write(fd2, &c, 1);
    		if (wlen != 1) 
        		printf("Error from write: %d, %d\n", wlen, errno);
	

	c='D';
		wlen = write(fd2, &c, 1);
    		if (wlen != 1) 
        		printf("Error from write: %d, %d\n", wlen, errno);
	char cmd[4];
	sprintf(cmd,"%d",deapth);
	int n=strlen(cmd);
	int spot = 0;
	n=strlen(cmd);
	 do {
        wlen= write( fd2, &cmd[spot], 1 );
        spot ++;
    } while (spot<n);
	spot=0;
}



	
