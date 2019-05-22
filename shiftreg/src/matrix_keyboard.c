/*
 * matrix_keyboard.c
 *
 *  Created on: Apr 17, 2019
 *      Author: valmit
 */


#include "matrix_keyboard.h"





void init_keyboard(void) {
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN;


	GPIOA->MODER &= ~(GPIO_MODER_MODER4 | GPIO_MODER_MODER5);
	GPIOA->MODER |= GPIO_MODER_MODER15_0;
	GPIOC->MODER |= GPIO_MODER_MODER12_0;


	GPIOA->MODER &= ~GPIO_MODER_MODER0;


	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1 | GPIO_PUPDR_PUPDR15_1;
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR12_1;

}





uint8_t question() {
	uint8_t res = 0;
	GPIOA->BSRR = GPIO_BSRR_BR_15;
	GPIOC->BSRR = GPIO_BSRR_BR_12;

	GPIOA->BSRR = GPIO_BSRR_BS_15;

	__NOP();
	__NOP();
	__NOP();
	if ((GPIOA->IDR & GPIO_IDR_4) == GPIO_IDR_4) {
		res |= 1 << 0;
	}
	if ((GPIOA->IDR & GPIO_IDR_5) == GPIO_IDR_5) {
		res |= 1 << 1;
	}
	__NOP();
	__NOP();

	GPIOA->BSRR = GPIO_BSRR_BR_15;
	GPIOC->BSRR = GPIO_BSRR_BS_12;
	__NOP();
	__NOP();
	__NOP();
	if ((GPIOA->IDR & GPIO_IDR_4) == GPIO_IDR_4) {
		res |= 1 << 2;
	}

	if ((GPIOA->IDR & GPIO_IDR_5) == GPIO_IDR_5) {
		res |= 1 << 3;
	}
	__NOP();
	__NOP();
	GPIOA->BSRR = GPIO_BSRR_BR_15;
	GPIOC->BSRR = GPIO_BSRR_BR_12;
	return res;
}



