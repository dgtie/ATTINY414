#include <avr/io.h>

char get_key(void);
void control_off(void);

namespace
{

char bits, count;
const char *ptr;

void next(void) {
  if (*ptr == (char)0x80) {
    if (get_key()) ptr -= (unsigned)ptr[1];
    else return control_off();
  }
  if (!(bits = *ptr++)) control_off();
  else {
    if (bits & 1) {
      count = (bits & 0x7f) >> 1;
      count --;
      bits &= 0x80;
    }
  }
}

} // anonymous

void control_off(void) {
  TCB0.CTRLA = 0;
  ptr = 0;
}

void ir_send(const char *p) {
  if (ptr) return;
  bits = count = 0;
  unsigned c = *p++;
  TCB0.CCMPL = c - 1;
  TCB0.CCMPH = 30;
  TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc;
  TCB0.CTRLB = TCB_CNTMODE_PWM8_gc;
  TCA0.SINGLE.PER = c * *p++;
  TCB0.CTRLA = TCB_ENABLE_bm;
  ptr = p;
}

void control_isr(void) {
  if (!ptr) return;
  if (bits & 0x80) TCB0.CTRLB |= TCB_CCMPEN_bm;
  else TCB0.CTRLB &= ~TCB_CCMPEN_bm;
  if (count) count--;
  else {
    if (bits & 0x3f) bits <<= 1;
    else next();
  }
}

bool ir_sending(void) { return ptr; }
