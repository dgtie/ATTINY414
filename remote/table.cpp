#include "table.h"

extern const char device;
void ir_send(const char*);

void ir_send(char key) {
  static char buffer[99];
  const Device *devices = (const Device*)(0x8ec8);
  if ((int)devices[device].func == -1) return;
  char c = devices[device].command[key];
  if (c == devices[device].null) return;
  (*devices[device].func)(buffer, devices[device].address, c);
  ir_send(buffer);
}
