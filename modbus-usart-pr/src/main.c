#include "stm32f0xx.h"


//legs for modbus: PD2 - RTS, RXD - PC11, TXD - PC10


char usart_data_recieved = 0;

void init_usart() {
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;	//USART3 Clock ON

    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIODEN |
    				RCC_AHBENR_GPIOCEN;

    GPIOD->MODER |= GPIO_MODER_MODER2;
    GPIOC->MODER |= GPIO_MODER_MODER11 | GPIO_MODER_MODER10;

//	RCC->APB2ENR  	|= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;

	USART3->CR1  |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE; // USART1 ON, TX ON, RX


	USART3->CR1 |= USART_CR1_RTOIE;
	USART3->CR2 |= USART_CR2_RTOEN;
	USART3->CR3 |= USART_CR3_DEM; // RS 485 driver enable
	USART3->BRR 	= 0xEA6; // set some baudrate
	USART3->RTOR = 42304; // modbus RTU timeout placeholder
	// GPIOA Clock ON. Alter function clock ON

	NVIC_EnableIRQ (USART3_4_IRQn); // i don't understand what 4 means



}


void USART3_4_IRQHandler() {

	usart_data_recieved = 1;
	USART3->ICR = USART_ICR_RTOCF; //clear interrupt request
}


void transmit_data (uint16_t data) {
	if ((USART3->ISR & USART_ISR_TXE) == USART_ISR_TXE) {
		USART3->TDR = data;
	}
}

void slave_message_act(uint16_t message) {

}

void master_message_act(uint16_t message) {

}


int main(void){

uint16_t recieved = 0;

	while(1 == 1) {
		if (usart_data_recieved == 1) {
			//TODO do something with recieved data
			recieved = USART3->RDR;
		}
	}
}
