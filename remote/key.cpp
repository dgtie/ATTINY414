#include <avr/interrupt.h>
#include "key.h"

#define SENSE_PINS (PIN6_bm|PIN3_bm|PIN2_bm|PIN1_bm)

void key_handler(char);
void learn_isr(void);

namespace
{

char debounce, keys;

char read(char k, char d) {
  PORTB.DIRCLR = d;			// release drive pin
  asm volatile("nop");			// time for hw steady
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  if (!(PORTA.IN & PIN3_bm)) k += 64;	// bit 7-6
  if (!(PORTA.IN & PIN2_bm)) k += 16;	// bit 5-4
  if (!(PORTA.IN & PIN1_bm)) k += 4;	// bit 3-2
  if (!(PORTA.IN & PIN6_bm)) k++;	// bit 1-0
  return k;
}

void pinctrl(char c) {
  PORTA.PIN1CTRL = PORTA.PIN2CTRL = PORTA.PIN3CTRL = PORTA.PIN6CTRL = c;
}

} // anonymous

char get_key(void) { return keys; }

void key_init(void) {
  PORTB.DIRSET = PIN1_bm | PIN0_bm;	// drive pins low
  cli();
  PORTA.INTFLAGS = SENSE_PINS;	// clear flags
  pinctrl(PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc);
  sei();
}

void key_scan(void) {		// call it from poll() 
  if (!debounce) return;
  if (!--debounce) {
    char k = read(read(read(0, 0), PIN0_bm), PIN1_bm);
    if (keys != k) key_handler(keys = k);
    SLPCTRL.CTRLA = SLPCTRL_SMODE_PDOWN_gc | SLPCTRL_SEN_bm;
    key_init();			// enable interrupt
  }
}

ISR(PORTA_PORT_vect) {
  if (PORTA.PIN4CTRL == PORT_ISC_BOTHEDGES_gc) learn_isr();
  if (PORTA.INTFLAGS & SENSE_PINS) {
    debounce = 40;		// 40x560us = 22ms
    pinctrl(PORT_PULLUPEN_bm);	// disable interrupt, keep pullup
    SLPCTRL.CTRLA = 0;		// don't sleep
  }
}

char key_index(char c) {
  const char keys[] = { KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6,
                        KEY_7, KEY_8, KEY_9, KEY_POWER, KEY_SHIFT };
  char i;
  for (i = 0; i < sizeof(keys); i++) if (c == keys[i]) break;
  return i;
}
