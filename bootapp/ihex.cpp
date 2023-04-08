#include <stdio.h>

void set_row_address(unsigned), save_row_data(unsigned char), dump_rows(void);

namespace
{

unsigned char data[20];
unsigned size, address, extended;

int hex(unsigned char c) {
  if (c < '0') return -1;
  if (c < ':') return c - '0';
  if (c < 'A') return -1;
  if (c < 'G') return c - '7';
  if (c < 'a') return -1;
  if (c < 'g') return c - 'W';
  return -1;
}

int hex2(unsigned char *p) {
  int i = hex(p[1]);
  if (i != -1) i += hex(p[0]) * 16;
  return i;
}

} // anonymous

unsigned verify_hex(unsigned char *buf) {
  int cs, temp, i;
  if (buf[0] != ':') return 0;
  size = hex2(&buf[1]);
  if (size < 0) return 0;
  cs = size;
  for (i = 0; i < size + 4; i++) {
    temp = hex2(&buf[(i << 1) + 3]);
    if (temp == -1) return 0;
    cs += data[i] = temp;
  }
  cs &= 255;
  if (cs) {
    buf[(size << 1) + 11] = 0;
    printf("%s expect cs = %02x\n", buf, (data[size + 3] - cs) & 255);
    return 0;
  }
  if (data[2] == 4) extended = (data[3] << 8) | data[4];
  address = (extended << 16) | (data[0] << 8) | data[1];
  if (!data[2]) {
    set_row_address(address);
    for (int j = 0; j < size; j++) save_row_data(data[j + 3]);
  }
  i = (i << 1) + 3;
  return buf[i] < 14 ? i : 0;
}
