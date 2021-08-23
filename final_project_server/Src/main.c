/*****************************************************************************/
/* Server-Side main file, with interrupts                                         */
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
	WATCHDOG_init();
	BUTTON_init();
	LED_init();
	TIMER2_init();
	USART2_init();
	USART1_init();
	EVENT_QUEUE_init();
	TERMINAL_init();

	while (1) {

		if (EVENT_QUEUE_isEmpty() == FALSE) {
			uint32_t event = EVENT_QUEUE_read();
			//print("%lu\n",(unsigned long)EVENT_QUEUE_read());
			if (event == EVENT_BUTTON_PRESSED) {

				BUTTON_switch();

			}
			WATCHDOG_pet();
			if (event == EVENT_USART1_MESSAGE) {
				USART1_getCommand(USART1_response);
				if (strstr(USART1_response, "switch")) {
					LED_switch();
				}
				USART2_print(USART1_response);
			}
			WATCHDOG_pet();
			if (event == EVENT_USART2_MESSAGE) {
				TERMINAL_handleCommand();
			}
			WATCHDOG_pet();

		}

		if (TIMER2_expired()) {
			SCHEDULER_handle();
		}
		WATCHDOG_pet();
		LED_status();

	}

}
