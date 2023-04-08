void init(void), toggle_LED(void), uart_send(char), deinit(void);
void uart_init(void), uart_deinit(void), sw_reset(void);
char wait(unsigned);	// while waiting, do background jobs
char flash(char);	// write a character to flash memory
                        // return true file upload complete
char read_button(void);

#define APPWORD *((int*)0x8200)	// start of application in data space
                                // it is 0xFFFF (-1) when flash is blank

static char upload_done;

__attribute__((section(".ctors"), naked)) int boot(void) {
  asm volatile("clr r1");	// compiler expects r1 to be zero
}

__attribute__((section(".init9"), naked)) int main(void) {
  init();
  wait(200);			// time for hardware to be steady
  if ((APPWORD == -1) || !read_button()) {	// check if skip bootloader
    uart_init();
    while (!upload_done && wait(200))		// check upload status
      toggle_LED(); 				// and blinks LED
    sw_reset();
  }
  deinit();			// restore to reset state
  asm volatile("jmp 0x200");	// jump to application
}

void uart_received(char c) {	// process character received from PC
  uart_send(c);			// echo it back to PC
  upload_done = flash(c);	// write character to flash
}				// and report upload status
