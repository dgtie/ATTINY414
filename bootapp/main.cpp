#include <stdio.h>
#include <unistd.h>

bool serial_open(const char*);
void serial_close(void), serial_send(char);
char serial_read(void);
void serial_flush(void);
unsigned pop_row(unsigned, unsigned char*);
unsigned verify_hex(unsigned char*);
unsigned get_start_address(void);

int main(int argc, char* argv[]) {
  FILE *fp = NULL;
  unsigned char buffer[100];
  printf("Bootloader Application for ATTINY414\n");
  printf("-- press switch PA3 at power up to enter boot mode\n");
  printf("   Copyright: (C) 2023 Lams Workshop\n");
  if (argc > 1) {
    if (!serial_open("/dev/ttyACM0")) {
      printf("cannot open serial port /dev/ttyACM0\n");
      return 1;
    }
    serial_close();	// without reopening serial port,
			// the first byte read always zero
    fp = fopen(argv[1], "r");
    if (!fp) {
      printf("cannot open file %s\n", argv[1]);
      return 1;
    }
    while (fgets((char*)buffer, 100, fp)) {
      if (!verify_hex(buffer)) {
        fclose(fp);
        return -1;
      }
    }
    fclose(fp);
    serial_open("/dev/ttyACM0");	// reopen
    unsigned char data[64];
    unsigned a = get_start_address() | 0x8000;
    char cs = 0x1b ^ (a >> 8) ^ (a & 255);
    serial_send(0x1b);
    serial_send(a & 255);
    serial_send(a >> 8);
    serial_send(cs);
    usleep(10000);
    serial_flush();
    a &= 0xfff;
    while ((a = pop_row(a, data))) {
      printf("writing page 0x%04X\n", a); a += 64;
      for (int i = 0; i < 64; i++) {
        serial_send(data[i]);
        char c = serial_read();
        if (c != (char)data[i]) {
          serial_close();
          printf("upload failed send:%02x read:%02x\n", data[i], c);
          return 1;
        }
      }
    }
    for (int i = 0; i < 64; i++) {
      serial_send(0xff);
      if (serial_read() != (char)0xff) {
        serial_close();
        printf("upload may be failed\n");
        return 1;
      }
    }
    printf("upload done\n");
    serial_close();
  }
  return 0;
}
