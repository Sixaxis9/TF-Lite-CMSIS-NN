// Use main.cpp
// Start project converted following the procedure on Microchip website
// https://microchipsupport.force.com/s/article/C---Support-for-Atmel-START-C 


#include <atmel_start.h>

void USART_0_(void);

extern struct usart_sync_descriptor USART_0;
struct io_descriptor *io;

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	USART_0_();
	
	/* Replace with your application code */
	while (1) {
		io_write(io, (uint8_t *)"Toggle LED!\n", 12);
		gpio_toggle_pin_level(LED);
		delay_ms(100);
	}
}

void USART_0_(void)
{
	usart_sync_get_io_descriptor(&USART_0, &io);
	usart_sync_enable(&USART_0);
}