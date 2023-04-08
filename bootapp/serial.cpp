#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

int port;
struct termios tty;

bool serial_open(const char *device) {
  port = open(device, O_RDWR);
  if (tcgetattr(port, &tty)) return false;
  tty.c_cflag &= ~PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;
  tty.c_cflag &= ~CRTSCTS;
  tty.c_cflag |= CREAD | CLOCAL;
  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO;
  tty.c_lflag &= ~ECHOE;
  tty.c_lflag &= ~ECHONL;
  tty.c_lflag &= ~ISIG;
  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
  tty.c_oflag &= ~OPOST;
  tty.c_oflag &= ~ONLCR;
  tty.c_cc[VTIME] = 0;
  tty.c_cc[VMIN] = 1;
  cfsetispeed(&tty, B115200);
  cfsetospeed(&tty, B115200);
  return tcsetattr(port, TCSANOW, &tty) ? false : true;
}

void serial_send(char c) { write(port, &c, 1); }

char serial_read(void) {
  char c;
  read(port, &c, 1);
  return c;
}

void serial_close(void) { close(port); }

void serial_flush(void) { tcflush(port, TCIFLUSH); }
