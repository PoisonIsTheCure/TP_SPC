//*************************************************************************************//
// INCLUDES
//
//*************************************************************************************//

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "sys/cm4.h"
#include "sys/devices.h"
#include "sys/init.h"
#include "sys/clock.h"
#include "stdlib.h"

//*************************************************************************************//

//*************************************************************************************//
// VARIABLES GLOBALES
//
//*************************************************************************************//

static volatile char c = 0;
volatile uint64_t timecount = 0;
volatile uint64_t interval = 500;

//*************************************************************************************//

//*************************************************************************************//
// FONCTION TEMPO
//
// 500ms
//*************************************************************************************//

void tempo_500ms()
{
	volatile uint32_t duree;
	/* estimation, suppose que le compilateur n'optimise pas trop... */
	for (duree = 0; duree < 5600000; duree++)
	{
		;
	}
}

//*************************************************************************************//

//*************************************************************************************//
// 4 LEDS ROUGES SHIELD
//
// PA 4 à 7 (4 leds rouge shield)
//*************************************************************************************//

void init_red_led4()
{ // on initialise la led 4 (PA4)
	GPIOA.MODER = (GPIOA.MODER & 0xFFFFFCFF) | 0x00000100;
	GPIOA.OTYPER &= ~(0x1 << 4);
	GPIOA.OSPEEDR |= 0x03 << 8;
	GPIOA.PUPDR &= 0xFFFFFCFF;
}

void init_red_led5()
{ // on initialise la led 5 (PA5)
	GPIOA.MODER = (GPIOA.MODER & 0xFFFFF3FF) | 0x00000400;
	GPIOA.OTYPER &= ~(0x1 << 5);
	GPIOA.OSPEEDR |= 0x03 << 10;
	GPIOA.PUPDR &= 0xFFFFF3FF;
}

void init_red_led6()
{ // on initialise la led 6 (PA6)
	GPIOA.MODER = (GPIOA.MODER & 0xFFFF3FFF) | 0x00004000;
	GPIOA.OTYPER &= ~(0x1 << 6);
	GPIOA.OSPEEDR |= 0x03 << 12;
	GPIOA.PUPDR &= 0xFFFF3FFF;
}

void init_red_led7()
{ // on initialise la led 7 (PA7)
	GPIOA.MODER = (GPIOA.MODER & 0xFFFFCFFF) | 0x00001000;
	GPIOA.OTYPER &= ~(0x1 << 7);
	GPIOA.OSPEEDR |= 0x03 << 14;
	GPIOA.PUPDR &= 0xFFFFCFFF;
}

void init_red_leds()
{						 // on initialise toutes les leds (de 4 à 7)
	RCC.AHB1ENR |= 0x01; // on initialise la clock de GPIOA (RM page 141)
	init_red_led4();	 // on initialise la led 4 PA4
	init_red_led5();	 // on initialise la led 5 PA5
	init_red_led6();	 // on initialise la led 6 PA6
	init_red_led7();	 // on initialise la led 7 PA7
}

void turn_red_leds_on()
{ // on allume toutes les leds (de 4 à 7)
	GPIOA.ODR |= 0x000000F0;
}

void turn_red_leds_off()
{ // on éteint toutes les leds (de 4 à 7)
	GPIOA.ODR &= 0xFFFFFF0F;
}

void clignote_2sec()
{
	for (int i = 0; i < 2; i++)
	{
		turn_red_leds_on();
		tempo_500ms();
		turn_red_leds_off();
		tempo_500ms();
	}
}

//*************************************************************************************//

//*************************************************************************************//
// BOUTON BLEU NUCLEO
//
// GPIO C patte 13
// no pull-up, no pull-down
//*************************************************************************************//

void init_blue_button()
{
	RCC.AHB1ENR |= 0x4;			  // on initialise la clock de GPIOC (RM page 141)
	GPIOC.MODER &= 0xF3FFFFFF;	  // on met le moder en input mode (change l octet qui comprend le n13 en 00)
	GPIOC.OTYPER &= ~(0x1 << 13); // on met le 13eme bit de output type a 0 (c.a.d en push-pull mode)/ on decide de modifier le 13
	GPIOC.OSPEEDR |= 0x03 << 26;  // on met le 26-27ime bits a 11 , donc high speed mode
	GPIOC.PUPDR &= 0xF3FFFFF;	  // pull-up / pull-down / on fait un and avec des 1 en modifaint seulement le port 13, RMp189
}

uint8_t is_blue_button_pressed()
{
	return ((GPIOC.IDR >> 13) & 0x01) == 0; // quand le bouton est pressé, sa valeur est a 0
}

//*************************************************************************************//

//*************************************************************************************//
// BOUTON BLANC SHIELD
//
// GPIO B patte 8
// no pull-up, no pull-down
//*************************************************************************************//

void init_white_button()
{
	RCC.AHB1ENR |= 0x2;			 // on initialise la clock de GPIOB (RM page 141)
	GPIOB.MODER &= 0xFFFCFFFF;	 // on met le moder en input mode (change l octet qui comprend le n8 en 00)
	GPIOB.OTYPER &= ~(0x1 << 8); // on met le 8eme bit de output type a 0 (c.a.d en push-pull mode)/ on decide de modifier le 8
	GPIOB.OSPEEDR |= 0x03 << 16; // on met le 16-17ime bits a 11 , donc high speed mode
	GPIOB.PUPDR &= 0xFFFCFFFF;	 // pull-up / pull-down / on fait un and avec des 1 en modifaint seulement le port 8, RMp189
}

uint8_t is_white_button_pressed()
{
	return ((GPIOB.IDR >> 8) & 0x01) == 0; // quand le bouton est pressé, sa valeur est a 0
}

//*************************************************************************************//

//*************************************************************************************//
// INIT_USART
//
// Initialisation de l'USART
//*************************************************************************************//

void init_USART()
{
	GPIOA.MODER = (GPIOA.MODER & 0xFFFFFF0F) | 0x000000A0;
	GPIOA.AFRL = (GPIOA.AFRL & 0xFFFF00FF) | 0x00007700;
	USART2.BRR = get_APB1CLK() / 9600;
	USART2.CR3 = 0;
	USART2.CR2 = 0;
}

//*************************************************************************************//

//*************************************************************************************//
// FONCTIONS AFFICHAGES
//
//*************************************************************************************//

void _putc(const char c)
{
	while ((USART2.SR & 0x80) == 0)
		;
	USART2.DR = c;
}

void _puts(const char *c)
{ // affiche un entier sur 2 digits
	int len = strlen(c);
	for (int i = 0; i < len; i++)
	{
		_putc(c[i]);
	}
}

char _getc()
{ // affiche le carac entré au clavier
	while ((USART2.SR & 0x20) == 0)
		;
	char c = USART2.DR;
	return c;
}

//*************************************************************************************//

//*************************************************************************************//
// TIMER SYSTEME SYSTICK
//
//*************************************************************************************//

void systick_init(uint32_t freq)
{
	uint32_t p = get_SYSCLK() / freq;
	SysTick.LOAD = (p - 1) & 0x00FFFFFF;
	SysTick.VAL = 0;
	SysTick.CTRL |= 7;
}

void __attribute__((interrupt)) SysTick_Handler()
{
	/* Le fait de définir cette fonction suffit pour
	 * qu'elle soit utilisée comme traitant,
	 * cf les fichiers de compilation et d'édition de lien
	 * pour plus de détails.
	 */
	// tempo_500ms(); // tempo of 500ms
	timecount++;
	if (timecount % interval == 0)
		GPIOA.ODR ^= 0x00000020; // flipping the 5th bit
								 // if it's on it turns of , if its of , it turns on.
}

//*************************************************************************************//

//*************************************************************************************//
// FONCTION PRINCIPALE
//
//*************************************************************************************//

int main(void)
{

	printf("\e[2J\e[1;1H\r\n");
	printf("\e[01;32m*** Welcome to Nucleo F446 ! ***\e[00m\r\n");

	printf("\e[01;31m\t%08lx-%08lx-%08lx\e[00m\r\n", U_ID[0], U_ID[1], U_ID[2]);
	printf("SYSCLK = %9lu Hz\r\n", get_SYSCLK());
	printf("AHBCLK = %9lu Hz\r\n", get_AHBCLK());
	printf("APB1CLK= %9lu Hz\r\n", get_APB1CLK());
	printf("APB2CLK= %9lu Hz\r\n", get_APB2CLK());
	printf("\r\n");

	// systick_init(1); // une fois chaque millisecond

	init_red_leds();

	init_white_button();
	init_blue_button();

	// while (1) {

	/*if (is_white_button_pressed()) {
		turn_red_leds_on();
	} else {
		turn_red_leds_off();
	}

	if (is_blue_button_pressed()) {
		_puts("b");
	}*/

	// clignote_2sec();

	//}

	while (1)
	{
		if (is_white_button_pressed())
		{
			// if (interval>0) interval -=50;
			turn_red_leds_on();
			while (is_white_button_pressed())
				; // Protection pour que ca sera fait une seule fois
		}
		else
		{
			turn_red_leds_off();
		}
	}

	return EXIT_SUCCESS;
}

//*************************************************************************************//