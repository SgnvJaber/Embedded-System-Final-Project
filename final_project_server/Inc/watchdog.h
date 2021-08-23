#ifndef WATCHDOG_H_
#define WATCHDOG_H_
/******************************************************************************
 WATCHDOG_init
 Description: Initializes the independent Watchdog (IWDG) peripheral.
 ******************************************************************************/
void WATCHDOG_init(void);

/******************************************************************************
 WATCHDOG_pet
 Description:Refresh the counter value
 ******************************************************************************/
void WATCHDOG_pet(void);

#endif /* WATCHDOG_H_ */
