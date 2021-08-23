#include "watchdog.h"
#include "stm32f303xe.h"
#include "types.h"

void WATCHDOG_init(void) {

	//Enable the IWDG by writing 0x0000CCCC in the IWDG_KR register(p. 759 in the reference-manual)
	IWDG->KR = 0x0000CCCC;
	//Enable register access by writing 0x00005555 in the IWDG_KR register(p. 759 in the reference-manual)
	IWDG->KR = 0x00005555;
	//Write the IWDG prescaler by programming IWDG_PR from 0 to 7(p. 759 in the reference-manual)
	IWDG->PR = 7;
	//Write the reload register (IWDG_RLR)(p. 759 in the reference-manual)
	//IWDG->RLR = 0x00000FFF;
	IWDG->RLR = 8000000;
	//Wait for the registers to be updated (IWDG_SR = 0x00000000)(p. 759 in the reference-manual)
	while (IWDG->SR)
		;

}
void WATCHDOG_pet(void) {
	//Refresh the counter value with IWDG_RLR (IWDG_KR = 0x0000AAAA).
	IWDG->KR = 0x0000AAAA;
}

