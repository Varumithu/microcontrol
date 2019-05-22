/*
 * graphic.c
 *
 *  Created on: Apr 17, 2019
 *      Author: valmit
 */


#include "graphic.h"

volatile uint8_t state_buf = 0;

void SPI2_IRQHandler(void) {
    GPIOA->BSRR = GPIO_BSRR_BS_8;
    GPIOA->BSRR = GPIO_BSRR_BR_8;
//    data_flushed = 1;
    volatile uint32_t useless = SPI2->DR;


	draw_buffer();

}



volatile uint8_t buffer[8] = {
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
};

void init_graphic(void) {

    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN;


    GPIOB->MODER |= GPIO_MODER_MODER15_1 | GPIO_MODER_MODER13_1;
    GPIOA->MODER |= GPIO_MODER_MODER8_0;

    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

    GPIOB->AFR[1] |= (0 << 4 * (15 - 8)) | (0 << 4 * (13 - 8));


    SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BR | SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA;
    SPI2->CR2 |= SPI_CR2_DS | SPI_CR2_RXNEIE; // 1111 - 16bit data size

    SPI2->CR1 |= SPI_CR1_SPE;

    NVIC_EnableIRQ (SPI2_IRQn);
}



//uint8_t pic[8] = {0b00000000,
//0b01111110,
//0b00010000,
//0b00010000,
//0b00010000,
//0b00010000,
//0b00010000,
//0b01111110,};


void pixel(uint32_t n, uint8_t leds)
 {
    uint8_t line = 1 << n;
    uint8_t pixel = leds;

    uint16_t data = (pixel << 8 | line);
    SPI2->DR = data;

}

void draw_buffer() {



    uint8_t line = 1 << state_buf;

    uint16_t data = (buffer[state_buf] << 8 | line);
    SPI2->DR = data;

	state_buf = (state_buf + 1) % 8;

}

void set_buffer(uint8_t* new_buffer) { // new_buffer points to array of eight chars
	for (uint8_t i = 0; i < 8; ++i) {
		buffer[i] = new_buffer[i];
	}
}


