#include <avr/interrupt.h>

bool uart_print(const char*);

namespace
{

unsigned mark;
char line[] = "000 000\r\n";

void toString(char *p, unsigned u) {
  unsigned q = u / 10;
  unsigned r = u - q * 10;
  p[2] = r + '0';
  u = q / 10;
  r = q - u * 10;
  p[1] = r + '0';
  p[0] = u + '0';
}

void print(unsigned space) {
  if (mark) {
    toString(line, mark / 40);
    toString(&line[4], space / 40);
    uart_print(line);
  }
}

} //anonymous

void learn_init(void) {
  mark = 0;
  TCB0.CTRLA = TCB_CLKSEL_CLKDIV2_gc;	// 0.6 us
  TCB0.CTRLB = TCB_CNTMODE_INT_gc;	// overflow interrupt
  TCB0.CCMP = 65535;			// 39 ms
  TCB0.CNT = 0;
  PORTA.INTFLAGS = PIN4_bm;
  cli();
  PORTA.PIN4CTRL = PORT_ISC_BOTHEDGES_gc;
  TCB0.INTCTRL = TCB_CAPT_bm;
  TCB0.CTRLA |= TCB_ENABLE_bm;
  sei();
}

void learn_deinit(void) {
  PORTA.PIN4CTRL = TCB0.INTCTRL = 0;
  TCB0.CTRLA &= ~TCB_ENABLE_bm;
}

void learn_isr(void) {
  if (PORTA.INTFLAGS & PIN4_bm) {
    unsigned cnt = TCB0.CNT;
    TCB0.CNT = 0;
    PORTA.INTFLAGS = PIN4_bm;
    if (PORTA.IN & PIN4_bm) mark = cnt;
    else print(cnt);
  }
}

ISR(TCB0_INT_vect) {
  TCB0.INTFLAGS = TCB_CAPT_bm;
  print(0);
  mark = 0;
}

