#ifndef _serial_h
#define _serial_h

int serial_makeraw(int fd);
int serial_set_raw(int fd);
int serial_set_baudrate(int fd, int baudrate);
int serial_stopbits(int fd, int stopbits);
int serial_set_min_time(int fd, int vmin, int vtime);
int serial_clear(int fd);
int serial_set_TIOCM(int fd, int tiocm, int val);

#endif
