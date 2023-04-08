#include <avr/interrupt.h>

void poll(unsigned char);
void control_isr(void);

void init(void) {
  PORTA.DIRSET = PIN7_bm;
  PORTA.PIN3CTRL = PORT_PULLUPEN_bm;
  TCA0.SINGLE.PER = 1866;
  TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
  TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
  PORTB.DIRSET = PIN2_bm;
  USART0.BAUD = 116;
  USART0.CTRLB = USART_TXEN_bm;
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

const char* uart_ptr;

} //anonymous

bool uart_print(const char *p) {
  if (USART0.CTRLA & USART_DREIE_bm) return false;
  uart_ptr = p;
  return (USART0.CTRLA |= USART_DREIE_bm);
}

ISR(USART0_DRE_vect) {
  if (*uart_ptr) USART0.TXDATAL = *uart_ptr++;
  else USART0.CTRLA &= ~USART_DREIE_bm;
}

ISR(TCA0_OVF_vect) {
  control_isr();
  TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm; ++tick;
}

bool wait(unsigned t) {
  unsigned char c;
  for (loop(c = tick); t--; c = tick) while (c == tick) loop(tick);
  return true; 
}

void clear_LED(void) { PORTA.OUTCLR = PIN7_bm; }

void sw_reset(void) {
  _PROTECTED_WRITE(RSTCTRL.SWRR, RSTCTRL_SWRE_bm);
}

bool LED_state(void) { return PORTA.OUT & PIN7_bm; }

void toggle_LED(void) { PORTA.OUTTGL = PIN7_bm; }
