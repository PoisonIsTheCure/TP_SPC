#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "sys/cm4.h"
#include "sys/devices.h"
#include "sys/init.h"
#include "sys/clock.h"

static volatile char c = 0;

// My variables
volatile uint32_t timecount = 0;
volatile uint32_t interval = 100;

// MACROS to modify bits
#define SET_BIT(REG, BIT) ((REG) |= (1 << (BIT)))
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(1 << (BIT)))
#define TOGGLE_BIT(REG, BIT) ((REG) ^= (1 << (BIT)))
#define READ_BIT(REG, BIT) ((REG) & (1 << (BIT)))


void init_LD2()
{
	/* on positionne ce qu'il faut dans les différents
	   registres concernés */
	RCC.AHB1ENR |= 0x01;								   // on allume gpioA (init clock)
	GPIOA.MODER = (GPIOA.MODER & 0xFFFFF3FF) | 0x00000400; // ici on met le moder de 5pin bit a 1
	// GPIO.MODER c'est le register ou on precise si c'est input 00 ou output 01
	GPIOA.OTYPER &= ~(0x1 << 5); // on met toujour a 0 , push-pull
	GPIOA.OSPEEDR |= 0x03 << 10; // always on high speed
								 // GPIOA.PUPDR &= 0xFFFFF3FF; // on met a 11
}

/*
UMp23
Bouton Bleu = PC13
cherche a modifier tout les registre utile avec RM a partir de la p187
*/
void init_PB()
{
	/* GPIOC.MODER = ... */
	RCC.AHB1ENR |= 0x04;	  // On initialise la clock de GPIOC (RM page 141)
	GPIOC.MODER = 0xF3FFFFFF; // On met le moder en input mode (change l octet qui comprend le n13 en 00)
	// GPIOC.OTYPER &= ~(0x1<<13); // on met le 13eme bit de output type a 0 (c.a.d en push-pull mode)/ on decide de modifier le 13
	// GPIOA.OSPEEDR |= 0x03<<26;  // on met le 26-27ime bits a 11 , donc high speed mode
	GPIOC.PUPDR &= 0xF3FFFFFF; // Mettre en pull-up / pull-down / on fait un and avec des 1 en modifaint seulement le port 13, RMp189
}

void tempo_500ms()
{
	volatile uint32_t duree;
	/* estimation, suppose que le compilateur n'optimise pas trop... */
	for (duree = 0; duree < 5600000; duree++)
	{
		;
	}
}

void change_couleur()
{
	srand(timecount);
	int alea = rand() % 3;
	// Clearing colors bits
	CLEAR_BIT(GPIOA.ODR,8);
	CLEAR_BIT(GPIOA.ODR,9);
	CLEAR_BIT(GPIOA.ODR,10);
	switch (alea)
	{
	case 0:
		SET_BIT(GPIOA.ODR,8);
		return;
	case 1:
		SET_BIT(GPIOA.ODR,9);
		return;
	case 2:
		SET_BIT(GPIOA.ODR,10);
		return;
	}
	return;
}

void init_USART()
{
	GPIOA.MODER = (GPIOA.MODER & 0xFFFFFF0F) | 0x000000A0;
	GPIOA.AFRL = (GPIOA.AFRL & 0xFFFF00FF) | 0x00007700;
	USART2.BRR = get_APB1CLK() / 9600;
	USART2.CR3 = 0;
	USART2.CR2 = 0;
}

void _putc(const char c)
{
	while ((USART2.SR & 0x80) == 0)
		;
	USART2.DR = c;
}

// affiche un entier sur 2 digits
void _puts(const char *c)
{
	int len = strlen(c);
	for (int i = 0; i < len; i++)
	{
		_putc(c[i]);
	}
}
// affiche le carac entré au clavier
char _getc()
{
	/* À compléter */
	while ((USART2.SR & 0x20) == 0)
		;
	char c = USART2.DR;
	return c;
}

/* Initialisation du timer système (systick) */
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
		change_couleur();
		//GPIOA.ODR ^= 0x00000700; // flipping the 5th bit
								 // if it's on it turns of , if its of , it turns on.
}

// /* Fonction non bloquante envoyant une chaîne par l'UART */
// int _async_puts(const char *s)
//
// 	/* Cette fonction doit utiliser un traitant d'interruption
// 	 * pour gérer l'envoi de la chaîne s (qui doit rester
// 	 * valide pendant tout l'envoi). Elle doit donc être
// 	 * non bloquante (pas d'attente active non plus) et
// 	 * renvoyer 0.
// 	 *
// 	 * Si une chaîne est déjà en cours d'envoi, cette
// 	 * fonction doit renvoyer 1 (et ignorer la nouvelle
// 	 * chaîne).
// 	 *
// 	 * Si s est NULL, le code de retour permet de savoir
// 	 * si une chaîne est encore en cours d'envoi ou si
// 	 * une nouvelle chaîne peut être envoyée.
// 	 */
// 	/* À compléter */
// }

int button_pressed()
{
	return ((GPIOC.IDR >> 13) & 0x01) == 0; // apparement ,quand le bouton est poussé, sa valeur est a 0
}

void turn_led_on()
{
	GPIOA.ODR |= 0x00000020;
}

void turn_led_off()
{
	GPIOA.ODR &= ~(0x1 << 5);
}

void clignote_2sec()
{
	for (int i = 0; i < 2; i++)
	{
		turn_led_on();
		tempo_500ms();
		turn_led_off();
		tempo_500ms();
	}
}

/* PARTIE CARTE MERE */

void init_RED_PA8()
{
	/* on positionne ce qu'il faut dans les différents
	   registres concernés */
	RCC.AHB1ENR |= 0x01; // on allume gpioA (init clock)
	GPIOA.MODER = (GPIOA.MODER & 0xFFFCFFFF) | 0x00010000;
	GPIOA.OTYPER &= ~(0x1 << 8);  // on met toujour a 0 , push-pull
	GPIOA.OSPEEDR |= 0x03 << 16;  // always on high speed
	GPIOA.PUPDR &= ~(0x03 << 16); // on met a 11
}

void init_RED_PA9()
{
	/* on positionne ce qu'il faut dans les différents
	   registres concernés */
	RCC.AHB1ENR |= 0x01; // on allume gpioA (init clock)
	GPIOA.MODER = (GPIOA.MODER & 0xFFF3FFFF) | 0x00040000;
	GPIOA.OTYPER &= ~(0x1 << 9);  // on met toujour a 0 , push-pull
	GPIOA.OSPEEDR |= 0x03 << 18;  // always on high speed
	GPIOA.PUPDR &= ~(0x03 << 18); // on met a 11
}

void init_RED_PA10()
{
	/* on positionne ce qu'il faut dans les différents
	   registres concernés */
	RCC.AHB1ENR |= 0x01; // on allume gpioA (init clock)
	GPIOA.MODER = (GPIOA.MODER & 0xFFCFFFFF) | 0x00100000;
	GPIOA.OTYPER &= ~(0x1 << 10); // on met toujour a 0 , push-pull
	GPIOA.OSPEEDR |= 0x03 << 20;  // always on high speed
	GPIOA.PUPDR &= ~(0x03 << 20); // on met a 11
}


int main()
{

	printf("\e[2J\e[1;1H\r\n");
	printf("\e[01;32m*** Welcome to Nucleo F446 ! ***\e[00m\r\n");

	printf("\e[01;31m\t%08lx-%08lx-%08lx\e[00m\r\n",
		   U_ID[0], U_ID[1], U_ID[2]);
	printf("SYSCLK = %9lu Hz\r\n", get_SYSCLK());
	printf("AHBCLK = %9lu Hz\r\n", get_AHBCLK());
	printf("APB1CLK= %9lu Hz\r\n", get_APB1CLK());
	printf("APB2CLK= %9lu Hz\r\n", get_APB2CLK());
	printf("\r\n");

	// init_LD2();
	init_PB();
	init_RED_PA8();
	init_RED_PA9();
	init_RED_PA10();
	systick_init(1000); // une fois chaque seconde

	while (1)
	{
		if (button_pressed()){
			interval-=10;
		}
	}

	return 0;
}
