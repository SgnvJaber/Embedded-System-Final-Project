/*****************************************************************************/
/* Client-Side main file, with interrupts                                         */
/* All references to the reference manual are to STM's RM0316, revision 8    */
/* Students:Saed Jaber,ID:208480632, Ameer Jaber:318534831 */

/*****************************************************************************/
#include "stm32f303xe.h"
#include "button.h"
#include "watchdog.h"
#include "led.h"
#include "scheduler.h"
#include "terminal.h"
#include "timer2.h"
#include "types.h"
#include "usart1.h"
#include "usart2.h"
#include "buffer.h"
#include "event_queue.h"
#include <string.h>

static char USART1_response[TERMINAL_SIZE_OF_COMMAND_BUFFER];

int main(void) {
	//Initialize Watchdog to reset the system if it gets stuck
	WATCHDOG_init();
	//Initialize the blue button on the NUCLEO-F303RE
	BUTTON_init();
	//Initialize the LED functions
	LED_init();
	//Initialize the TIMER functions
	TIMER2_init();
	//Initialize USART2 to transmit and receive commands from PC
	USART2_init();
	//Initialize USART2 to transmit and receive commands from ESP8266(modem)
	USART1_init();
	//Initialize an event queue buffer to handle interrupt events.
	EVENT_QUEUE_init();
	//Initialize the terminal to handle commands
	TERMINAL_init();
	while (1) {
		//Checking if the Queue contains an event
		if (EVENT_QUEUE_isEmpty() == FALSE) {
			//Storing the event into variable
			uint32_t event = EVENT_QUEUE_read();
			//If the button was pressed
			if (event == EVENT_BUTTON_PRESSED) {
				//Send through modem to the client to switch it's button
				BUTTON_switch();
			}
			//Reseting Watchdog timer
			WATCHDOG_pet();
			if (event == EVENT_USART1_MESSAGE) {
				USART1_getCommand(USART1_response);
				//If the command received contains the word "switch"
				if (strstr(USART1_response, "switch")) {
					//switch led state
					LED_switch();
				}
				USART2_print(USART1_response);
			}
			//Reseting Watchdog timer
			WATCHDOG_pet();
			if (event == EVENT_USART2_MESSAGE) {
				TERMINAL_handleCommand();
			}
			//Reseting Watchdog timer
			WATCHDOG_pet();

		}
		if (TIMER2_expired()) {
			SCHEDULER_handle();
		}
		//Reseting Watchdog timer
		WATCHDOG_pet();
		LED_status();

	}

}
