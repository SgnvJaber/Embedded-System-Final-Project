#ifndef TYPES_H_
#define TYPES_H_

/******************************************************************************
 Definitions
 ******************************************************************************/
typedef enum {
	FALSE, TRUE
} BOOL;

typedef enum {
	EVENT_USART2_MESSAGE, EVENT_BUTTON_PRESSED, EVENT_USART1_MESSAGE

} EVENT;

typedef enum {
	OFF, ON, BLINKY
} STATE;

typedef enum {
	HANDLING_INPUT, GETTING_WIFI_NAME, GETTING_WIFI_PASSWORD
} MODE;

#endif /* TYPES_H_ */
