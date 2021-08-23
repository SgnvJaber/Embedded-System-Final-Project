#include "led.h"
#include "stm32f303xe.h"
#include "scheduler.h"
#include "usart2.h"
static STATE LED = OFF;
void LED_init(void) {
	// Enable the GPIOA clock.
	RCC->AHBENR |= 0x00020000;
	// Configure PA5 as output
	// (by default it will then be push pull, see p. 237 of the reference manual).
	GPIOA->MODER |= 0x00000400;
}

void LED_off(void) {
	GPIOA->ODR &= ~0x00000020;
}

void LED_on(void) {
	GPIOA->ODR |= 0x00000020;
}

void LED_toggle(void) {
	GPIOA->ODR ^= 0x00000020;
}

//A function to switch LED state.
void LED_switch(void) {
	print("Changing!\n");
	if (LED == BLINKY) {
		LED = OFF;
	} else {
		LED++;
	}

}

//A function that checks LED status.
void LED_status(void) {
	if (LED == OFF) {
		SCHEDULER_stopBlinking();
		LED_off();
	} else if (LED == ON) {
		SCHEDULER_stopBlinking();
		LED_on();
	} else {
		SCHEDULER_blink();
	}

}

