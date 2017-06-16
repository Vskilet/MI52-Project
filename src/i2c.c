#include "i2c.h"

#define TX_LOCK 1
#define RX_LOCK 2

uint32_t nb_read;

struct I2C_Device i2c_device = {
	.instance = I2C1, .state = 0,
};

void I2C_Init(){
	GPIO_TypeDef * gpiob = GPIOB;
	RCC_TypeDef * rcc = RCC;
	I2C_TypeDef * i2c = I2C1;

	/*-------------- clock enable --------------- */
	/* Enable GPIOB clock */
	rcc->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	/* Enable I2Cx clock */
	rcc->APB1ENR |= RCC_APB1ENR_I2C1EN;
	/* Reset I2C1 clock */
  rcc->APB1RSTR |=  RCC_APB1RSTR_I2C1RST;
	rcc->APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;

	/*-------------- Pin muxing configuration --------------- */
	/*AF4 on PB8 and PB9 */
	gpiob->AFR[1] &= ~(0xFF); /* clear the 8 bits */
	gpiob->AFR[1] |= (4 << 4) | 4; /* set alternate function AF4 */
	/* Configure alternate function for PB8 and PB9*/
	gpiob->MODER &= ~GPIO_MODER_MODER8;
	gpiob->MODER |= GPIO_MODER_MODER8_1;
	gpiob->MODER &= ~GPIO_MODER_MODER9;
	gpiob->MODER |= GPIO_MODER_MODER9_1;
	/* OTYPER type register */
	gpiob->OTYPER |= (1 << 8) | (1<<9);

	/*-------------- I2C parameters configuration --------------- */
	i2c->CR1 &= ~I2C_CR1_SMBUS; // (optionnal) i2c mode
	i2c->CR2 |= (42 & I2C_CR2_FREQ); //mask to be sure not modified anything else
																// for the value see set_clk.c
	i2c->CCR &= ~I2C_CCR_FS; // (optionnal) Set as standard mode (not fast)
	i2c->CCR |= (210 & I2C_CCR_CCR); // I2C control clock

	i2c->TRISE &= ~I2C_TRISE_TRISE; // Time rise
	i2c->TRISE |=  (43 & I2C_TRISE_TRISE);

	i2c->CR1 |= I2C_CR1_PE; // enable peripherical
}


/*
 * Start
 * slave address
 * write
 * Adresse registers SUB
 * ACK
 * Start
 * slave address
 * received Temperature
 *
 */
uint32_t I2C_Communication(I2C_TypeDef * i2c, uint8_t * data, uint32_t len){
	printf("Start communication\r\n");
	i2c->CR1 |= I2C_CR1_START;
	while(!(i2c->SR1 & I2C_SR1_SB));
	printf("Reset SB flag\r\n");
	i2c->DR = 0x3C; //0b10111110; //would like to write an adress

	while (!(i2c->SR1 & I2C_SR1_ADDR)); //wait ADDR is set
	if (i2c->SR1 && i2c->SR2){	// clear ADDR flag
		__asm("nop");
		printf("Reset ADDR Flag\r\n");
	}

	//while (!(i2c->SR1 & I2C_SR1_TXE));
	i2c->DR = 0x28;
	//while (!(i2c->SR1 & I2C_SR1_BTF));

	i2c->CR1 |= I2C_CR1_START;
	while(!(i2c->SR1 & I2C_SR1_SB));
	i2c->DR = 0x3D; //0b10111111; //would like to read an adress

	while (!(i2c->SR1 & I2C_SR1_ADDR)); //wait ADDR is set
	if (i2c->SR1 && i2c->SR2){	// clear ADDR flag
		__asm("nop");
		printf("Reset ADDR Flag\r\n");
	}

	//i2c->CR1 &= ~I2C_CR1_ACK; // ACK low
	i2c->CR1 |= I2C_CR1_POS; // POS high
	//while(!(i2c->SR1 & I2C_SR1_BTF));

	*data++ = i2c->DR;
	nb_read++;

	i2c->CR1 |= I2C_CR1_STOP;	//Set STOP high
	return nb_read;
}
