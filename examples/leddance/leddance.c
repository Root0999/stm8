/*
//BLINK Example for STM8S003F3

//In this example, I assume there are 8 LED's available.
//sorted by use in dance routine:
//PC3 -> PC4 -> PC5 -> PC6 -> PC7 -> PD1 -> PD2 -> PD3

//Remember that PD1 is the SWIM pin, so if we have an LED between this pin
//and ground, we can not flash the chip! so unconnect the LED from PD1 prior
//to flashing!
*/

#include "stm8s003.h"
#include "clock.h"
#include "gpio.h"
#include "dance.h"

#define DELAY_RESET_VALUE	20;
static int blink_delay;

unsigned char pinsC = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 
			| GPIO_PIN_6 | GPIO_PIN_7;
unsigned char pinsD = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;

/**
*@brief Port D init routine
*/
void init_gpio() {

	//set LED pins
	GPIO_Config_Pin(GPIO_PORT_C, pinsC , PIN_MODE_OUTPUT_PP_FAST);
	GPIO_Config_Pin(GPIO_PORT_D, pinsD , PIN_MODE_OUTPUT_PP_FAST);
}

/**
*@brief Main function.
*/
int main() {
	CLK_Init(CLK_SRC_HSI, 0x00, 0x00);
	init_gpio();

	//create LED structs

	//led 1 is our HEAD
	d_head = DNC_Gen_Struct(GPIO_PORT_C, GPIO_PIN_3,
		0 /* NO PREV */);
	
	//led 2
	d_head->next = DNC_Gen_Struct(GPIO_PORT_C, GPIO_PIN_4,
		d_head /* PREV = LED1 */);

	//led 3
	d_head->next->next = DNC_Gen_Struct(GPIO_PORT_C, GPIO_PIN_5,
		d_head->next /* PREV = LED2 */);
	
	//led 4
	d_head->next->next->next = DNC_Gen_Struct(GPIO_PORT_C, GPIO_PIN_6,
		d_head->next->next /* PREV = LED3 */);

	//led 5
	d_head->next->next->next->next = DNC_Gen_Struct(GPIO_PORT_C, GPIO_PIN_7,
		d_head->next->next->next /* PREV = LED4 */);

	//led 6
	d_head->next->next->next->next->next = DNC_Gen_Struct(GPIO_PORT_D,
	 GPIO_PIN_1, d_head->next->next->next->next /* PREV = LED5 */);

	//led 7
	d_head->next->next->next->next->next->next = DNC_Gen_Struct(GPIO_PORT_D,
		GPIO_PIN_2, d_head->next->next->next->next->next /* PREV = LED6 */);

	//led 8 is out TAIL
	d_tail = DNC_Gen_Struct(GPIO_PORT_D, GPIO_PIN_3,
	d_head->next->next->next->next->next->next /* PREV = LED7 */);

	//final fix for connections
	//assign LED7's next to be LED8, the tail!
	d_head->next->next->next->next->next->next->next = d_tail;
	d_tail->next = 0;
	d_head->prev = 0;
	
	//main loop
	while(1) {
		//PERFORM DANCE!
		blink_delay = DELAY_RESET_VALUE;
		
		DNC_Toggling(d_head, d_tail, DANCE_DIR_FORWARD, 0, &blink_delay, 10, 0);
		DNC_Toggling(d_tail, d_head, DANCE_DIR_BACKWARD, 0, &blink_delay, -5, 0);

		DNC_On(d_head, d_tail, DANCE_DIR_FORWARD, 0, &blink_delay, 10, 0);
		DNC_Off(d_tail, d_head, DANCE_DIR_BACKWARD, 0, &blink_delay, -5, 0);

		DNC_AllOnOff(d_head, 10, &blink_delay, 2);
		DNC_AllOffOn(d_head, 12, &blink_delay, -4);
		
		DNC_Toggling(d_head, d_tail, DANCE_DIR_FORWARD, 0, &blink_delay, 10, 0);
		DNC_Toggling(d_tail, d_head, DANCE_DIR_BACKWARD, 0, &blink_delay, -5, 0);

		DNC_On(d_head, d_tail, DANCE_DIR_FORWARD, 0, &blink_delay, 10, 0);
		DNC_Off(d_tail, d_head, DANCE_DIR_BACKWARD, 0, &blink_delay, -5, 0);

		CLK_Delay_ms(1000);			
	}
}