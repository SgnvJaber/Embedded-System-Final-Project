#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "stm32f303xe.h"
#include "types.h"
#include "event_queue.h"
#include "usart1.h"
#include "buffer.h"

//A BOOL flag to check if a command has been received
static BOOL Command_Received;
//A CYCLIC_BUFFER which will store the USART1 response
static CYCLIC_BUFFER USART1_response;

//A function that return whether or not a command has been received.
BOOL USART1_commandReceived(void) {
	if (Command_Received) {
		//If a command has been received return true,
		// and reset the BOOL value to false
		Command_Received = FALSE;
		return TRUE;
	} else {
		//else return false
		return FALSE;
	}
}

//A function that copy the USART1 response into an array.
//will be used to send be the USART1 response to main.
void USART1_getCommand(char *p_command) {
	//While the CYCLIC_BUFFER is not empty:
	//Start reading the data and store it into array
	while (!CB_isEmpty(&USART1_response)) {
		*p_command = (char) CB_read(&USART1_response);
		p_command++;
	}
	//If the CYCLIC_BUFFER is empty,
	//close the array using '\0'
	if (CB_isEmpty(&USART1_response) == TRUE) {
		*p_command = '\0';
	}

}
//A function to Initializes USART1 and its registers.

void USART1_init(void) {
	// Enable GPIOA clock (p. 148 in the reference manual).
	RCC->AHBENR |= 0x00020000;
	// Enable USART1 clock(p. 151 in the reference manual).
	RCC->APB2ENR |= 0x00004000; //Enable bit 14.
	//  0000 0000 0000 0000 0100 0000 0000 0000
	// Configure GPIOA pins 9 and 10 as alternate function 7, which is USART1.
	//(p. 237 in the reference manual).
	//Enabling MODER10 and MODER9 as alternate function.
	GPIOA->MODER |= 0x00280000;	//0000 0000 0010 1000 0000 0000 0000 0000
	//(Need to enable AF7 p.53 in the data-sheet)
	// AFR[0] is the same as AFRL in the reference manual (p. 241),(pins from 0 to 7).
	// AFR[1] is the same as AFRH in the reference manual (p. 242),(pins from 8 to 15).
	GPIOA->AFR[1] |= 0x00000770;	    //Pin 9,Pin 10
	USART1->BRR = 69.44444444444444; // 8 MHz / 115200 baud rate-->8*10^6/115200.
	// Enable USART1 and its TX,RX functionality.
	USART1->CR1 = 0x0000002D;
	NVIC_EnableIRQ(USART1_IRQn);
	//Initializes an empty CYCLIC_BUFFER
	CB_init(&USART1_response);
}

//A handler to get the response from the ESP8266 modem.
void USART1_EXTI25_IRQHandler(void) {
	char rx_byte;
	// Read the received byte into the buffer.
	// This also clears the interrupt request flag.
	rx_byte = USART1->RDR;
	// If the user entered '\n', a whole command has been received.
	if (USART1->RDR == '\n') {
		Command_Received = TRUE;
		EVENT_QUEUE_write(EVENT_USART1_MESSAGE);
		CB_write((uint32_t) rx_byte, &USART1_response);
		return;
	}
	CB_write((uint32_t) rx_byte, &USART1_response);
}

//A function to write commands the ESP8266 modem using USART1
void USART1_print(const char *p_data) {
	while (*p_data != '\0') {
		USART1->TDR = *p_data;
		p_data++;
		while (!(USART1->ISR & 0x00000080))
			// The TXE bit is bit 7 in the register ISR.
			;
	}
}
