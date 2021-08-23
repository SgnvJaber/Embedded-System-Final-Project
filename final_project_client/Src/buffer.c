#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "stm32f303xe.h"
#include "types.h"
#include "buffer.h"

// This buffer is used to handle commands.
// A function to to initializes an empty CYCLIC_BUFFER
void CB_init(CYCLIC_BUFFER *buffer) {
	//Initialize p_read and p_write to the start of the array.
	buffer->p_read = buffer->array;
	buffer->p_write = buffer->array;
	// p_end set to point at the last character of the buffer,
	buffer->p_end = buffer->array + BUFFER_SIZE;
}

// A function that return the current data that p_read is pointing at
uint32_t CB_read(CYCLIC_BUFFER *buffer) {
	uint32_t data = *(buffer->p_read);
	buffer->p_read++;
	if (buffer->p_read == buffer->p_end) {
		//Return p_read to the start of the array.
		buffer->p_read = buffer->array;
	}
	return data;
}
// A function to write data to CYCLIC_BUFFER,
// The data will be written the the slot p_write is pointing at.
void CB_write(uint32_t data, CYCLIC_BUFFER *buffer) {
	*(buffer->p_write) = data;
	buffer->p_write++;
	// If p_write reached the end of the array,return it to the start of the array
	if (buffer->p_write == buffer->p_end) {
		//Return p_write to the start of the array.
		buffer->p_write = buffer->array;

	}
}
// A function to check if a given CYCLIC_BUFFER is empty
BOOL CB_isEmpty(CYCLIC_BUFFER *buffer) {
	// if p_read is pointing at the same slot as p_write,
	// then the array is empty,otherwise it is not.
	return buffer->p_read == buffer->p_write;
}
