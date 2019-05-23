#include "stm32f0xx.h"


//legs for modbus: PD2 - RTS, RXD - PC11, TXD - PC10

void init_usart() {
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;	//USART1 Clock ON
	USART1->CR1  |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE; // USART1 ON, TX ON, RX
	USART1->CR2 |= USART_CR2_ABREN; // abrmode = 0

	USART1->CR1 |= USART_CR1_RTOIE;
	USART1->CR2 |= USART_CR2_RTOEN;
	USART1->CR3 |= USART_CR3_DEM; // RS 485 driver enable
	USART1->BRR 	= 0xEA6; // set some baudrate

    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN;

	RCC->APB2ENR  	|= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN; 	// GPIOA Clock ON. Alter function clock ON


}

int main(void)
{
	while(1 == 1) {

	}
}
