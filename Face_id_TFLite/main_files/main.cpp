/*
 * TFLite_SAMD51.cpp
 *
 * Created: 7/16/2020 7:26:21 PM
 * Author : Marco
 */ 

#include "atmel_start.h"
#include <string.h>
#include "TFLite_inference.h"
#include "tensorflow/lite/micro/debug_log.h"

void USART_0_(void);

extern struct usart_sync_descriptor USART_0;
struct io_descriptor *io;

extern float fArray[1200];
extern int8_t iArray[1200];

/* DWT (Data Watchpoint and Trace) registers, only exists on ARM Cortex with a DWT unit */
  #define KIN1_DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))
    /*!< DWT Control register */
  #define KIN1_DWT_CYCCNTENA_BIT       (1UL<<0)
    /*!< CYCCNTENA bit in DWT_CONTROL register */
  #define KIN1_DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))
    /*!< DWT Cycle Counter register */
  #define KIN1_DEMCR                   (*((volatile uint32_t*)0xE000EDFC))
    /*!< DEMCR: Debug Exception and Monitor Control Register */
  #define KIN1_TRCENA_BIT              (1UL<<24)
    /*!< Trace enable bit in DEMCR register */

#define KIN1_InitCycleCounter() \
  KIN1_DEMCR |= KIN1_TRCENA_BIT
  /*!< TRCENA: Enable trace and debug block DEMCR (Debug Exception and Monitor Control Register */
 
#define KIN1_ResetCycleCounter() \
  KIN1_DWT_CYCCNT = 0
  /*!< Reset cycle counter */
 
#define KIN1_EnableCycleCounter() \
  KIN1_DWT_CONTROL |= KIN1_DWT_CYCCNTENA_BIT
  /*!< Enable cycle counter */
 
#define KIN1_DisableCycleCounter() \
  KIN1_DWT_CONTROL &= ~KIN1_DWT_CYCCNTENA_BIT
  /*!< Disable cycle counter */
 
#define KIN1_GetCycleCounter() \
  KIN1_DWT_CYCCNT
  /*!< Read cycle counter register */

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	USART_0_();
	//DebugLog("Hello");
	setup_inference();
	
	uint8_t image_serial[1200];
	
	/* Replace with your application code */
	while (1) {
		
		
		io_read(io, image_serial, 1200);
		//io_write(io, image_serial, 1200);
		
		// instantiate float array    
for(int i = 0; i < 1200; ++i) {
    //fArray[i] = (float)(((float)image_serial[i])/255.0);
    iArray[i] = (int8_t) (image_serial[i] >> 1);
}

uint32_t cycles; /* number of cycles */
 
KIN1_InitCycleCounter(); /* enable DWT hardware */
KIN1_ResetCycleCounter(); /* reset cycle counter */
KIN1_EnableCycleCounter(); /* start counting */
int8_t *output = run_inference(); /* call function and count cycles */
cycles = KIN1_GetCycleCounter(); /* get cycle counter */
KIN1_DisableCycleCounter(); /* disable counting if not used any more */

		
		  uint8_t best_output = 0;
		  int8_t accuracy = -128;
		  
		  for (int i = 0; i < 43; i++)
		  {
			  if (accuracy < output[i])
			  {
				  accuracy = output[i];
				  best_output = i;
			  }
		  }
		  
		  
		uint8_t accuracy_1[4];

		memcpy(accuracy_1, &accuracy, 4);

		
		uint8_t cicle_count[4] = {0x00, 0x00, 0x00, 0x20};

		  io_write(io, &best_output, 1);
		  io_write(io, accuracy_1, 4);		  
		  io_write(io, (uint8_t *) &cycles, 4);
		
		gpio_toggle_pin_level(LED);
		//io_write(io, (uint8_t *)"Toggle LED!\n", 12);
		//delay_ms(100);
	}
}

void USART_0_(void)
{
	usart_sync_get_io_descriptor(&USART_0, &io);
	usart_sync_enable(&USART_0);
}

extern "C"{ void DebugLog(const char *s){
	io_write(io, (uint8_t *) s, strlen(s));
}
	
}