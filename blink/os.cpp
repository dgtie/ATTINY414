#include <avr/interrupt.h>

void poll(unsigned char);

void init(void) {
  PORTA.DIRSET = PIN7_bm;
  PORTA.PIN3CTRL = PORT_PULLUPEN_bm;
  TCA0.SINGLE.PER = 1866;
  TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
  TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
#ifndef BOOT_APP
  _PROTECTED_WRITE(CPUINT.CTRLA, (CPUINT.CTRLA | CPUINT_IVSEL_bm));
#endif
  sei();
}

namespace
{

volatile unsigned char tick;

void loop(unsigned char t) {
  static unsigned char tick;
  if (tick != t) poll(tick = t);
}

} //anonymous

ISR(TCA0_OVF_vect) {
  TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm; ++tick;
}

bool wait(unsigned t) {
  unsigned char c;
  for (loop(c = tick); t--; c = tick) while (c == tick) loop(tick);
  return true; 
}

void toggle_LED(void) { PORTA.OUTTGL = PIN7_bm; }

bool read_button(void) { return PORTA.IN & PIN3_bm; }

void sw_reset(void) {
  _PROTECTED_WRITE(RSTCTRL.SWRR, RSTCTRL_SWRE_bm);
}
