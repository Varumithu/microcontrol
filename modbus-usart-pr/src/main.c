#include "stm32f0xx.h"




void init_usart() {
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;	//USART1 Clock ON
	USART1->CR1  |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE; // USART1 ON, TX ON, RX
}



int main(void)
{
	while(1 == 1) {

	}
}
