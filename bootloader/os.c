#include <avr/io.h>

void uart_received(char);

void init(void) {
  PORTA.DIRSET = PIN7_bm;		// PA7: LED
  PORTA.PIN3CTRL = PORT_PULLUPEN_bm;	// PA3: test for boot
  TCA0.SINGLE.PER = 1866;		// 560us
  TCA0.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;
}

void uart_init(void) {
  PORTB.DIRSET = PIN2_bm;		// PB2: UART-TX
  USART0.BAUD = 116;			// 115200
  USART0.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
  USART0.RXDATAL;			// clear input buffer
}

void deinit(void) {
  PORTB.DIRCLR = PIN2_bm;
  TCA0.SINGLE.CTRLA = PORTA.PIN3CTRL = 0;
  PORTA.DIRCLR = PORTA.OUTCLR = PIN7_bm;
}

static char tick;

static void isrs(void) {	// polling interrupt service
  if (USART0.STATUS & USART_RXCIF_bm) uart_received(USART0.RXDATAL);
  if (TCA0.SINGLE.INTFLAGS & TCA_SINGLE_OVF_bm) {
    tick++;
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
  }
}

char wait(unsigned t) {
  char c = tick;
  for (isrs(); t--; c = tick)	// wait for t == 0
    while (c == tick) isrs();	// service interrupt while waiting
  return 1;
}

void toggle_LED(void) { PORTA.OUTTGL = PIN7_bm; }

void uart_send(char c) { USART0.TXDATAL = c; }

char read_button(void) { return PORTA.IN & PIN3_bm; }

void sw_reset(void) {
  _PROTECTED_WRITE(RSTCTRL.SWRR, RSTCTRL_SWRE_bm);
}
