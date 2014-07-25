/**

Serial port configuration.

**/

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>

#include "serial.h"

/*
  Configure the serial port for non cannonical communication.
  Returns 1 if ok, 0 if FAIL.
*/

int serial_makeraw(int serial_fd) {
  static struct termios settings;

  /* read terminal initial parameters */
  if (tcgetattr(serial_fd, & settings) < 0) {
    /* return 0 if fails */
    return 0;
  }

  /* set serial to raw mode */
  cfmakeraw(& settings);

  /* setting new parms to terminal */
  if (tcsetattr(serial_fd,TCSANOW,&settings) < 0) {
    /* return 0 if fails */
    return 0;
  }

  return 1;
}

/* 
   Set serial to be "raw" mode, i.e., totally binary. It returns error
   code in case of failure, or 0 for success. Failure codes are:

   1    could not get serial atributes
   2    could not set serial atributes
*/
int serial_set_raw(int fd) {
  struct termios term;

  /* get current settings */
  if (tcgetattr(fd, & term) != 0)
    return 1;

  cfmakeraw(&term);               

  term.c_iflag &= ~ ( BRKINT | ICRNL | IGNCR | INLCR | IGNPAR | INPCK |
                      PARMRK | ISTRIP | IXOFF | IXON | IMAXBEL);
  term.c_iflag |= IGNBRK;
  
  /* output mode flags */
  term.c_oflag &= ~ ( OPOST | ONLCR | OCRNL | ONOCR | ONLRET | OFILL |
                      OFDEL | NLDLY | CRDLY | TABDLY | BSDLY | VTDLY |
                      FFDLY);
  
  /* local mode flags */
  term.c_lflag &= ~ ( ECHO | ECHOE | ECHOK | ECHONL | ICANON | IEXTEN |
                      ISIG | TOSTOP );
  term.c_lflag |= NOFLSH;

  /* control mode flags */
  term.c_cflag &= ~ ( HUPCL | CSTOPB | PARENB | CSIZE);
  term.c_cflag |= CLOCAL | CREAD | CS8;

  /* set terminal attributes */
  if (tcsetattr(fd, TCSANOW, &term) != 0)
    return 2;
  
  return 0;
}

int serial_stopbits(int serial_fd, int stopbits){
  static struct termios settings;

  /* read terminal initial parameters */
  if (tcgetattr(serial_fd, & settings) < 0) {
    /* return 0 if fails */
    return 0;
  }

  /* 2 stop bits */
  if (stopbits == 2)
    settings.c_cflag |= CSTOPB;
  else if (stopbits == 1)
    settings.c_cflag &= ~CSTOPB;

  /* setting new parms to terminal */
  if (tcsetattr(serial_fd,TCSANOW,&settings) < 0) {
    /* return 0 if fails */
    return 0;
  }

  return 1;
}

/*
  Set VMIN and VTIME attributes of serial.

   Error codes:
   0    success
   1    could not get serial atributes
   2    could not set serial atributes
*/
int serial_set_min_time(int fd, int vmin, int vtime) {
  struct termios term;

  /* get current settings */
  if (tcgetattr(fd, & term) != 0)
    return 1;

  /* control characters */
  //term.c_cc[VINTR] = 0;
  term.c_cc[VMIN] = vmin;
  //term.c_cc[VQUIT] = 0;
  //term.c_cc[VSUSP] = 0;
  term.c_cc[VTIME] = vtime;
  //term.c_cc[VSTART] = 0;
  //term.c_cc[VSTOP] = 0;
  
  /* set terminal attributes */
  if (tcsetattr(fd, TCSANOW, &term) != 0)
    return 2;
  
  return 0;
}

/* 
   Set serial speed (baudrate). You will need to use B9600, B19200,
   etc. constants for baudrate argument.

   Error codes:
   0    success
   1    could not get serial atributes
   2    could not set serial atributes
   3    could not set input speed
   4    could not set output speed
*/
int serial_set_baudrate(int fd, int baudrate) {
  struct termios term;

  /* get current settings */
  if (tcgetattr(fd, & term) != 0)
    return 1;
  
  /* set speed */
  if (cfsetispeed (&term, baudrate) != 0)
    return 3;
  if (cfsetospeed (&term, baudrate) != 0)
    return 4;

  /* set terminal attributes */
  if (tcsetattr(fd, TCSANOW, &term) != 0)
    return 2;
  
  return 0;
}

/*

Clean the serial buffer;

Returns 0 if ERROR
        1 if success
*/
int serial_clear(int serial_fd) {
  if (tcflush(serial_fd,TCIOFLUSH) < 0)
    return 0;
  else
    return 1;
}

/*
  sets a terminal IO bit accordling to val
  tiocm must be TIOCM_DTR or TIOCM_RTS

  Example:
  to set DTR to 1 : set_TIOCM(fdCom, TIOCM_DTR, 1);
  to clear RTS: set_TIOCM(fdCom, TIOCM_RTS, 0);

  Return 0 on success else non zero error code
*/
int serial_set_TIOCM(int fdCom, int tiocm, int val) {
  int status;

  if (ioctl(fdCom, TIOCMGET, & status) != 0) {
    fprintf(stderr, "ioctl get failed\n");
    return 1;
  }

  if(val)
    status |=  tiocm;
  else
    status &= ~tiocm;

  if (ioctl(fdCom, TIOCMSET, & status) != 0) {
    fprintf(stderr, "ioctl set failed [1]\n");
    return 1;
  }

  if(ioctl(fdCom, TIOCMGET, & status) != 0) {
    fprintf(stderr, "ioctl get failed [2]\n");
    return 1;
  }

  return 0;
}

