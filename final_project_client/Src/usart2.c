#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "stm32f303xe.h"
#include "types.h"
#include "buffer.h"
#include "usart2.h"
#include "event_queue.h"

// This buffer is used by the printf-like print function.
static char A_Tx_Buffer[USART2_SIZE_OF_PRINT_BUFFER];
//A BOOL flag to check if a command has been received
static BOOL Command_Received;
//A CYCLIC_BUFFER which will store the USART2 response
static CYCLIC_BUFFER USART2_response;

//A printf-like function to print variables and string
void print(char *p_format, ...) {
	va_list p_variables;
	va_start(p_variables, p_format);
	(void) vsprintf(A_Tx_Buffer, p_format, p_variables);
	USART2_print(A_Tx_Buffer);
}

//A function that return whether or not a command has been received.
BOOL USART2_commandReceived(void) {
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

//A function that copy the USART2 response into an array.
//will be used to send be the USART2 response to main.
void USART2_getCommand(char *p_command) {
	//While the CYCLIC_BUFFER is not empty:
	//Start reading the data and store it into array
	while (!CB_isEmpty(&USART2_response)) {
		*p_command = (char) CB_read(&USART2_response);
		p_command++;
	}
	//If the CYCLIC_BUFFER is empty,
	//close the array using '\0'
	if (CB_isEmpty(&USART2_response) == TRUE) {
		*p_command = '\0';
	}

}

//A function to Initializes USART2 and its registers.

void USART2_init(void) {
	// Enable GPIOA clock (p. 148 in the reference manual).
	RCC->AHBENR |= 0x00020000;
	// Enable USART2 clock.
	RCC->APB1ENR |= 0x00020000;
	// Configure GPIOA pins 2 and 3 as alternate function 7, which is USART2.
	GPIOA->MODER |= 0x000000A0;
	// AFR[0] is the same as AFRL in the reference manual (p. 241),
	// and AFR[1] is the same as AFRH in the reference manual (p. 242).
	GPIOA->AFR[0] |= 0x00007700;
	USART2->BRR = 833;  // 8 MHz / 9600 baud rate.
	// Enable USART2 and its RX and TX functionality.
	// Also enable the RX interrupt.
	USART2->CR1 = 0x0000002D;
	NVIC_EnableIRQ(USART2_IRQn);
	//Initializes an empty CYCLIC_BUFFER

	CB_init(&USART2_response);
}

//A handler to get the response from USART2 RX modem.
void USART2_EXTI26_IRQHandler(void) {
	char rx_byte;
	// Read the received byte into the buffer.
	// This also clears the interrupt request flag.
	rx_byte = USART2->RDR;
	// If the user entered '\n', a whole command has been received.
	if (rx_byte == '\n') {
		Command_Received = TRUE;
		EVENT_QUEUE_write(EVENT_USART2_MESSAGE);
		CB_write('\0', &USART2_response);
		return;
	}
	CB_write(rx_byte, &USART2_response);
}

void USART2_print(const char *p_data) {
	while (*p_data != '\0') {
		USART2->TDR = *p_data;
		p_data++;
		while (!(USART2->ISR & 0x00000080))
			;
	}
}
