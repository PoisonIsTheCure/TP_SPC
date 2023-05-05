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
volatile uint32_t globaltime = 0; // global time
volatile uint32_t interval_rand = 600; // intervale choisi random
volatile uint32_t interval_user = 300; // intervale controler par utilisateur
volatile uint8_t buttonA_clicked = 0; // button bleu
volatile uint8_t buttonB_clicked = 0; // is white button
volatile uint8_t etat_couleur = 0; // etat a 0 -> touts couleurs allumer
// loin -> bleu +/- 100ms
// 

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
	// RCC.AHB1ENR |= 0x04;	  // On initialise la clock de GPIOC (RM page 141)
	SET_BIT(RCC.AHB1ENR, 3);
	GPIOC.MODER &= ~(3U << (2 * 13)); // set pin PC13 to input mode
	GPIOC.PUPDR &= ~(3U << (2 * 13)); // disable pull-up and pull-down resistors
}

void init_white_button()
{
	RCC.AHB1ENR |= 0x2;			 // on initialise la clock de GPIOB (RM page 141)
	GPIOB.MODER &= 0xFFFCFFFF;	 // on met le moder en input mode (change l octet qui comprend le n8 en 00)
	GPIOB.OTYPER &= ~(0x1 << 8); // on met le 8eme bit de output type a 0 (c.a.d en push-pull mode)/ on decide de modifier le 8
	GPIOB.OSPEEDR |= 0x03 << 16; // on met le 16-17ime bits a 11 , donc high speed mode
	GPIOB.PUPDR &= 0xFFFCFFFF;	 // pull-up / pull-down / on fait un and avec des 1 en modifaint seulement le port 8, RMp189
}

int is_white_button_pressed()
{
	return ((GPIOB.IDR >> 8) & 0x01) == 0; // quand le bouton est pressé, sa valeur est a 0
}

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

void toggle_red_leds(){
	if (globaltime % interval_rand == 0){
		TOGGLE_BIT(GPIOA.ODR,4);
		TOGGLE_BIT(GPIOA.ODR,5);
		TOGGLE_BIT(GPIOA.ODR,6);
		TOGGLE_BIT(GPIOA.ODR,7);
	}
}

void init_buzzer()
{
	RCC.AHB1ENR |= 0x2; // on initialise la clock de GPIOB
						// GPIOB.MODER &= 0xFFF3FFFF;     // on met le moder en input mode (change 1 octet qui comprend le n9 en 00)
	SET_BIT(GPIOB.MODER, 2 * 9);
	CLEAR_BIT(GPIOB.MODER, (2 * 9) + 1);
	// GPIOB.OTYPER &= ~(0x1 << 9); // on met le 9eme bit de output type a 0 (c.a.d en push-pull mode)/ on decide de modifier le 9
	CLEAR_BIT(GPIOB.OTYPER, 9);
	GPIOB.OSPEEDR |= 0x03 << 18; // on met le 18-19ime bits a 11 , donc high speed mode
	GPIOB.PUPDR &= 0xFFF3FFFF;	 // pull-up / pull-down / on fait un and avec des 1 en modifaint seulement le port 9
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

void turn_buzzer_on()
{
	// GPIOB.ODR |= 0x00000100;
	SET_BIT(GPIOB.ODR, 9);
}

void turn_buzzer_off()
{	// on éteint toutes les leds (de 4 à 7)
	// GPIOB.ODR &= 0xFFFFFDFF;
	CLEAR_BIT(GPIOB.ODR, 9);
}

void buzzer()
{
	turn_buzzer_on();
	tempo_500ms();
	turn_buzzer_off();
	tempo_500ms();
}

void toggle_buzzer()
{
	if (globaltime % interval_user == 0)
		TOGGLE_BIT(GPIOB.ODR, 9);
}

void change_couleur_RGB()
{
	if (globaltime % interval_user == 0)
	{
		srand(globaltime);
		int alea = rand() % 3;
		// Clearing colors bits
		CLEAR_BIT(GPIOA.ODR, 8); // set red at 0
		CLEAR_BIT(GPIOA.ODR, 9); // set
		CLEAR_BIT(GPIOA.ODR, 10);
		switch (etat_couleur)
		{
		case 0:
			SET_BIT(GPIOA.ODR, 8); // RED
			return;
		case 1:
			SET_BIT(GPIOA.ODR, 9);// GREEN
			return;
		case 2:
			SET_BIT(GPIOA.ODR, 10);// BLUE
		case 3:
			SET_BIT(GPIOA.ODR, 8); // RED
			SET_BIT(GPIOA.ODR, 9); // GREEN
			SET_BIT(GPIOA.ODR, 10);// BLUE
			return;
		}
	}
	return;
}

void reinitialiser_interval_user(){
	if (interval_user == 0 || interval_user > 1000 )
		interval_user = 1000; // resetting the intervalA
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
	globaltime++;

	// toggle_buzzer();
	change_couleur_RGB();
	reinitialiser_interval_user(); // verrifie les bornes de l'intervale

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

int buttonA_pressed()
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

void toggle_led2(){
	TOGGLE_BIT(GPIOA.ODR,5);
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

void init_GREEN_PA9()
{
	/* on positionne ce qu'il faut dans les différents
	   registres concernés */
	RCC.AHB1ENR |= 0x01; // on allume gpioA (init clock)
	GPIOA.MODER = (GPIOA.MODER & 0xFFF3FFFF) | 0x00040000;
	GPIOA.OTYPER &= ~(0x1 << 9);  // on met toujour a 0 , push-pull
	GPIOA.OSPEEDR |= 0x03 << 18;  // always on high speed
	GPIOA.PUPDR &= ~(0x03 << 18); // on met a 11
}

void init_BLUE_PA10()
{
	/* on positionne ce qu'il faut dans les différents
	   registres concernés */
	RCC.AHB1ENR |= 0x01; // on allume gpioA (init clock)
	GPIOA.MODER = (GPIOA.MODER & 0xFFCFFFFF) | 0x00100000;
	GPIOA.OTYPER &= ~(0x1 << 10); // on met toujour a 0 , push-pull
	GPIOA.OSPEEDR |= 0x03 << 20;  // always on high speed
	GPIOA.PUPDR &= ~(0x03 << 20); // on met a 11
}

void init_RGB()
{
	init_RED_PA8();
	init_GREEN_PA9();
	init_BLUE_PA10();
}

/**
 * @brief Cette fonction contient decremente l'intervale a la suite d'une click
 * du boutton
 * Remarque : il faut appuyer puis enlever sa main pour faire la decrementation
 * La fonction doit etre utiliser dans un boucle while Infinie
 *
 * @param value the value to add (+/-)
 */
void on_buttonA_click(int value)
{
	if (buttonA_pressed())
	{
		if (!buttonA_clicked)
		{
			interval_user += value;
			buttonA_clicked = 1;
		}
	}
	else
	{
		buttonA_clicked = 0;
	}
}

/**
 * @brief Cette fonction contient decremente l'intervale a la suite d'une click
 * du boutton
 * Remarque : il faut appuyer puis enlever sa main pour faire la decrementation
 * La fonction doit etre utiliser dans un boucle while Infinie
 *
 * @param value the value to add (+/-)
 */
void on_buttonB_click(int value)
{
	if (is_white_button_pressed())
	{
		if (!buttonB_clicked)
		{
			interval_user += value;
			buttonB_clicked = 1;
		}
	}
	else
	{
		buttonB_clicked = 0;
	}
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

	init_LD2();
	init_RGB();
	init_PB();
	init_buzzer();
	init_white_button();

	systick_init(1000); // une fois chaque seconde

	while (1)
	{
		on_buttonA_click(+10);
		on_buttonB_click(-10);
		// buzzer();
	}

	return 0;
}
