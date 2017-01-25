#ifndef UART_H
#define UART_H

char uartr(int a);
void uartw(int port,char a);
void uartay(int yaw);
void uartao(int offset);
void uartad(int deapth);
int set_interface_attribs(int fd, int speed);
void set_mincount(int fd, int mcount);

#endif
