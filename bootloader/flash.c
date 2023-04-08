#include <avr/io.h>

static union {
  struct {
    char header;	// 0x1b
    char *addr;
    char cs;		// xor
  }; 
  long value;
  char bytes[4];
} packet;

static char *get_address(char c) {
  char cs = 0x1b;
  packet.value >>= 8;
  packet.bytes[3] = c;
  if (packet.header != cs) return 0;
  cs ^= packet.bytes[1];
  cs ^= packet.bytes[2];
  return packet.cs == cs ? packet.addr : 0;
}

char flash(char c) {	// write flash and report upload status
  static char *p, blank = 0xff;
  if (p) { 
    *p++ = c;		// write data to row buffer
    blank &= c;		// check for blank
    if ((int)p & 63) return 0;		// end of row?
    if (blank == (char)0xff) return 1;	// blank row -> stop upload
    blank = 0xff;		// restore for next row checking
    _PROTECTED_WRITE_SPM(NVMCTRL_CTRLA, NVMCTRL_CMD_PAGEERASEWRITE_gc);
				// erase flash and write buffer
    return (int)p == 0x9000;	// check available flash
  } else {
    p = get_address(c);
    return 0;
  }
}
