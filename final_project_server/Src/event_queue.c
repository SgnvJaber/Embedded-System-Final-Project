#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "stm32f303xe.h"
#include "types.h"
#include "event_queue.h"
#include "buffer.h"
//This buffer is used to handle and store commands.
static CYCLIC_BUFFER EVENT_QUEUE;
//A function to initializes an empty EVENT_QUEUE,
//by using the CYCLIC_BUFFER initializer  function.
void EVENT_QUEUE_init(void) {
	CB_init(&EVENT_QUEUE);
}

//A function to write to data to EVENT_QUEUE,
//by using the CYCLIC_BUFFER write function.
void EVENT_QUEUE_write(uint32_t data) {
	CB_write(data, &EVENT_QUEUE);
}

//A function to read data from an EVENT_QUEUE,
//by using the CYCLIC_BUFFER read function.
uint32_t EVENT_QUEUE_read(void) {
	uint32_t value = CB_read(&EVENT_QUEUE);
	return value;
}
//A function to check if an EVENT_QUEUE is empty,
//by using the CYCLIC_BUFFER isEmpty function.
BOOL EVENT_QUEUE_isEmpty(void) {
	return CB_isEmpty(&EVENT_QUEUE);
}
