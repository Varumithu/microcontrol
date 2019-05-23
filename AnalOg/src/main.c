#include "stm32f0xx.h"


// Sck - PB13
// MDSI PB15
// SS PA8

#include "graphic.h"

volatile uint8_t led[8]= {
													0b00000000,
													0b00000000,
													0b00000000,
													0b00000000,
													0b00000000,
													0b00000000,
													0b00000000,
													0b00000000,
													};
volatile uint8_t line_counter = 0;

volatile uint16_t counter = 0;


void systick_init()
{
	SysTick->LOAD = 8e8/1e4 - 1;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
}

void init(void) {
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN;
//	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;


	GPIOA->MODER &= ~(GPIO_MODER_MODER4 | GPIO_MODER_MODER5);
	GPIOA->MODER |= GPIO_MODER_MODER15_0;
	GPIOC->MODER |= GPIO_MODER_MODER12_0;

	GPIOA->MODER &= ~GPIO_MODER_MODER0;

	GPIOA->PUPDR |= GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1;


	//leds
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN; //activation intefaces for lights and buttons



	GPIOC->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7 |GPIO_MODER_MODER8 | GPIO_MODER_MODER9 | GPIO_MODER_MODER12 ); //clear mask
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER12_0; //set bits of 6 7 8 9 legs
}




void anal_init()
{
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	GPIOA->MODER |= GPIO_MODER_MODER1_0 | GPIO_MODER_MODER1_1;

	ADC1->CHSELR |= ADC_CHSELR_CHSEL1;
	ADC1->CFGR1 |= ADC_CFGR1_ALIGN| ADC_CFGR1_RES_0| ADC_CFGR1_RES_1 |ADC_CFGR1_CONT;
	ADC1->CFGR2 |= ADC_CFGR2_CKMODE_1;
	ADC1->SMPR  = ADC_SMPR_SMP;

	//kalibrovka
	if ((ADC1->CR & ADC_CR_ADEN) != 0) /* (1) */
	{
	ADC1->CR |= ADC_CR_ADDIS; /* (2) */
	}
	while ((ADC1->CR & ADC_CR_ADEN) != 0)
	{
	/* For robust implementation, add here time-out management */
	}
	ADC1->CFGR1 &= ~ADC_CFGR1_DMAEN; /* (3) */
	ADC1->CR |= ADC_CR_ADCAL; /* (4) */
	while ((ADC1->CR & ADC_CR_ADCAL) != 0) /* (5) */
	{
	/* For robust implementation, add here time-out management */
	}
	//zapusk
	if ((ADC1->ISR & ADC_ISR_ADRDY) != 0) /* (1) */
	{
	ADC1->ISR &= ~ADC_ISR_ADRDY; /* (2) */
	}
	ADC1->CR |= ADC_CR_ADEN; /* (3) */
	while ((ADC1->ISR & ADC_ISR_ADRDY) == 0) /* (4) */
	{
	/* For robust implementation, add here time-out management */
	}

	ADC1->CR = ADC_CR_ADSTART;
}

void time_init(){
		//SysTick_Config(8000000/1000);
	  SysTick->LOAD = 8000000/1000 - 1;
    SysTick->VAL = 8000000/1000 - 1;
	  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk |
                   SysTick_CTRL_ENABLE_Msk;
}




uint16_t Spi_Write_Data(uint16_t data){


	SPI2->DR = data;

	//while((SPI2->SR & SPI_SR_BSY));
	//GPIOA->BSRR = GPIO_BSRR_BS_8;
	//GPIOA->BRR = GPIO_BSRR_BS_8;
  return data;
}



void draw(void){
	  uint8_t data = led[line_counter];
		Spi_Write_Data(1 << (line_counter) | data << (8));
		line_counter++;
		line_counter %= 8;
		//Spi_Write_Data(0b1000000011000000);
}


//
//void SPI2_IRQHandler(void){
//
//	GPIOA->BSRR = GPIO_BSRR_BS_8;
//	GPIOA->BSRR = GPIO_BSRR_BR_8;
//	draw();
//	volatile uint16_t trash = SPI2->DR;
//}



void update_screen(void)
{
	for (int i = 7;i > 0;i--)
	{
		led[i]=led[i-1];
	}
	set_buffer(led);
}


void SysTick_Handler(void)
{
		counter++;
}

int main(void){
	init();
	time_init();
//	SPI_init();
//	Spi_Write_Data(0);
	init_graphic();
	set_buffer(led);
	draw_buffer();
	anal_init();
  while (1){
		if(ADC1->ISR & ADC_ISR_EOC)
		{
				uint8_t data = ADC1->DR;
				uint8_t pos = data/32;
				led[0] = 1 << pos;
				if (counter > 2e2)
				{
					counter = 0;
					update_screen();
				}
		}


	}


	return 0;
}
