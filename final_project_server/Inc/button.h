#ifndef BUTTON_H_
#define BUTTON_H_
/******************************************************************************
 BUTTON_init
 Description: Initializes the button GPIO (PC13) and its interrupt.
 ******************************************************************************/
void BUTTON_init(void);
/******************************************************************************
 BUTTON_switch
 Description: send through ESP8266 modem the command to switch LED
 ******************************************************************************/
void BUTTON_switch(void);
#endif /* BUTTON_H_ */
