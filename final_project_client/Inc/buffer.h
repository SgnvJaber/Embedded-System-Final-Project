#ifndef BUFFER_H_
#define BUFFER_H_
/******************************************************************************
 Includes
 ******************************************************************************/
#include "types.h"
/******************************************************************************
 Definitions
 ******************************************************************************/
#define BUFFER_SIZE 128
/******************************************************************************
 Structure
 ******************************************************************************/
//Cyclic Buffer
typedef struct {
	uint32_t array[BUFFER_SIZE];
	uint32_t *p_write;
	uint32_t *p_read;
	uint32_t *p_end;
} CYCLIC_BUFFER;
/******************************************************************************
 CB_init
 Description: Initializes a CYCLIC_BUFFER
 Input:
 CYCLIC_BUFFER pointer: a CYCLIC_BUFFER buffer to be initialized
 ******************************************************************************/
void CB_init(CYCLIC_BUFFER *buffer);
/******************************************************************************
 CB_write
 Description: write to an empty slot in the Buffer
 Input:
 data: The data to write.
 CYCLIC_BUFFER pointer: a CYCLIC_BUFFER  to be initialized
 ******************************************************************************/
void CB_write(uint32_t data, CYCLIC_BUFFER *buffer);
/******************************************************************************
 CB_read
 Description: read data which *p_read is pointing at.
 Input:
 CYCLIC_BUFFER: a CYCLIC_BUFFER to read from.
 ******************************************************************************/
uint32_t CB_read(CYCLIC_BUFFER *buffer);
/******************************************************************************
 CB_isEmpty
 Description: check if the cyclic buffer is empty
 CYCLIC_BUFFER: check if a given  CYCLIC_BUFFER is empty.
 ******************************************************************************/
BOOL CB_isEmpty(CYCLIC_BUFFER *buffer);
/*****************************************************************************/
#endif /* BUFFER_H_ */
