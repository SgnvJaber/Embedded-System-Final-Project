#ifndef USART1_H_
#define USART1_H_

/******************************************************************************
 Includes
 ******************************************************************************/
#include "types.h"

/******************************************************************************
 USART1_commandReceived
 Description:
 Returns TRUE if a '\n'-terminated command was received since the previous
 call.
 Return value: TRUE if a command was received, FALSE if not.
 ******************************************************************************/
BOOL USART1_commandReceived(void);

/******************************************************************************
 USART1_getCommand
 Description:
 Copies the current command in the USART1 RX buffer.
 The '\n' at the end of the received command is replaced with a '\0'.
 Input:
 p_command: Address to which to the command should be copied.
 ******************************************************************************/
void USART1_getCommand(char *p_command);

/******************************************************************************
 USART1_init
 Description: Initializes USART1 on GPIOs PA9 and PA10,
 with a baud rate of 115200.
 ******************************************************************************/
void USART1_init(void);

/******************************************************************************
 USART1_print
 Description: Prints a '\0'-terminated sequence of characters.
 Input:
 p_data: The sequence to print to the ESP8266 modem.
 ******************************************************************************/
void USART1_print(const char *p_data);
#endif /* USART1_H_ */
