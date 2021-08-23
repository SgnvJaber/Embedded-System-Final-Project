#include <string.h>
#include "terminal.h"
#include "led.h"
#include "scheduler.h"
#include "usart1.h"
#include "usart2.h"

static char A_COMMAND_RX[TERMINAL_SIZE_OF_COMMAND_BUFFER];
static char A_COMMAND_TX[TERMINAL_SIZE_OF_COMMAND_BUFFER];
static char Wifi_name[TERMINAL_SIZE_OF_COMMAND_BUFFER];
static char Wifi_password[TERMINAL_SIZE_OF_COMMAND_BUFFER];
static char IP_address[TERMINAL_SIZE_OF_COMMAND_BUFFER];

MODE mode = HANDLING_INPUT;

void TERMINAL_init(void) {
	USART2_print("\nWelcome to the Client side!\n");
	USART2_print("\nInitillizing...\n");
	USART1_print("ATE0\r\n");
	USART2_print("Setting CWMODE\n");
	USART1_print("AT+CWMODE=1\r\n");
	USART2_print("Setting CIPMUX\n");
	USART1_print("AT+CIPMUX=0\r\n");
	USART2_print("Client\n");
}

void TERMINAL_handleCommand(void) {
	USART2_getCommand(A_COMMAND_RX);
	strcat(A_COMMAND_RX, "\r\n");

	if (strcmp(A_COMMAND_RX, "config\r\n") == 0) {
		USART2_print("Please Enter Wifi name:\n");
		mode = GETTING_WIFI_NAME;
		return;
	}

	if (mode == HANDLING_INPUT) {
		if (strcmp(A_COMMAND_RX, "watchdog\r\n") == 0) {
			SCHEDULER_loop();
		} else if (strcmp(A_COMMAND_RX, "show\r\n") == 0) {
			USART1_print("AT+CIFSR\r\n");

		} else if (strcmp(A_COMMAND_RX, "disconnect\r\n") == 0) {
			USART1_print("AT+CWQAP\r\n");

		}

		else if (strcmp(A_COMMAND_RX, "connect\r\n") == 0) {
			USART2_print("Please Enter Server IP Address:\n");
			USART2_print(
					"(You can find it by typing \"show\" in the server side)\n");

			mode = GETTING_IP_ADDRESS;
			return;
		} else {
			USART1_print(A_COMMAND_RX);
		}
	}
	if (mode == GETTING_WIFI_NAME) {
		A_COMMAND_RX[strlen(A_COMMAND_RX) - 2] = '\0';
		(void) strcpy(Wifi_name, A_COMMAND_RX);
		USART2_print("Please Enter Wifi password:\n");
		mode = GETTING_WIFI_PASSWORD;
		return;
	}
	if (mode == GETTING_WIFI_PASSWORD) {
		A_COMMAND_RX[strlen(A_COMMAND_RX) - 2] = '\0';
		(void) strcpy(Wifi_password, A_COMMAND_RX);
		strcat(A_COMMAND_TX, "AT+CWJAP=");
		strcat(A_COMMAND_TX, "\"");
		strcat(A_COMMAND_TX, Wifi_name);
		strcat(A_COMMAND_TX, "\"");
		strcat(A_COMMAND_TX, ",");
		strcat(A_COMMAND_TX, "\"");
		strcat(A_COMMAND_TX, Wifi_password);
		strcat(A_COMMAND_TX, "\"");
		strcat(A_COMMAND_TX, "\r\n");
		USART2_print("\nProcessing data...please wait!\n\n");
		USART1_print(A_COMMAND_TX);
		A_COMMAND_TX[0] = '\0';
		mode = HANDLING_INPUT;
		return;
	}
	if (mode == GETTING_IP_ADDRESS) {

		A_COMMAND_RX[strlen(A_COMMAND_RX) - 2] = '\0';
		(void) strcpy(IP_address, A_COMMAND_RX);
		mode = CONNECT_TO_IP_ADDRESS;
	}
	if (mode == CONNECT_TO_IP_ADDRESS) {
		print("Server IP Address: is [%s] \n", IP_address);
		strcat(A_COMMAND_TX, "AT+CIPSTART=");
		strcat(A_COMMAND_TX, "\"");
		strcat(A_COMMAND_TX, "TCP");
		strcat(A_COMMAND_TX, "\"");
		strcat(A_COMMAND_TX, ",");
		strcat(A_COMMAND_TX, "\"");
		strcat(A_COMMAND_TX, IP_address);
		strcat(A_COMMAND_TX, "\"");
		strcat(A_COMMAND_TX, ",");
		strcat(A_COMMAND_TX, "80");
		strcat(A_COMMAND_TX, "\r\n");
		USART2_print("\nProcessing data...please wait!\n\n");
		USART1_print(A_COMMAND_TX);
		A_COMMAND_TX[0] = '\0';
		mode = HANDLING_INPUT;
		return;
	}
}
