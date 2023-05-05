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
// DEFINES
//
//*************************************************************************************//

#define RAND_NUM_MIN 50
#define RAND_NUM_MAX 800
#define DUREE_PARTIE 10000
#define DUREE_PARTIE_SEC DUREE_PARTIE / 1000

//*************************************************************************************//

//*************************************************************************************//
// MACROS
//
// to modify bits
//*************************************************************************************//

#define SET_BIT(REG, BIT) ((REG) |= (1 << (BIT)))
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(1 << (BIT)))
#define TOGGLE_BIT(REG, BIT) ((REG) ^= (1 << (BIT)))
#define READ_BIT(REG, BIT) ((REG) & (1 << (BIT)))

//*************************************************************************************//

//*************************************************************************************//
// VARIABLES GLOBALES
//
//*************************************************************************************//

static volatile char c = 0;
volatile uint64_t timecount = 0;
volatile uint16_t random_interval_red_leds = 0;
volatile uint16_t random_interval_user = 0;
volatile uint16_t nb_blue_button_pressed = 0;
volatile uint16_t debut_partie = 0;
volatile uint16_t fin_partie = 0;
volatile uint8_t chronometre = DUREE_PARTIE_SEC;
volatile uint8_t etat_couleur = 0; // etat a 0 -> touts couleurs allumer

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
// lED RGB
//
// PA 8 à 10 (3 leds de différentes couleurs)
//*************************************************************************************//

//*************************************************************************************//

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

void change_couleur_RGB()
{
	if (timecount % random_interval_user == 0)
	{
		srand(timecount);
		// int alea = rand() % 3;
		//  Clearing colors bits
		//CLEAR_BIT(GPIOA.ODR, 8); // set red at 0
		///CLEAR_BIT(GPIOA.ODR, 9); // set
		//CLEAR_BIT(GPIOA.ODR, 10);
		switch (etat_couleur)
		{
		case 0:
			TOGGLE_BIT(GPIOA.ODR, 8); // RED
			return;
		case 1:
			TOGGLE_BIT(GPIOA.ODR, 9); // GREEN
			return;
		case 2:
			TOGGLE_BIT(GPIOA.ODR, 10); // BLUE
		case 3:
			TOGGLE_BIT(GPIOA.ODR, 8);	// RED
			TOGGLE_BIT(GPIOA.ODR, 9);	// GREEN
			TOGGLE_BIT(GPIOA.ODR, 10); // BLUE
			return;
		}
	}
	return;
}

int abs(int val)
{
	if (val < 0)
		return -val;
	return val;
}

void verifie_etat_couleur()
{
	int val = abs(random_interval_red_leds - random_interval_user);
	if (val > 100)
	{
		etat_couleur = 0;
	}
	if (val < 100 && val > 50)
	{
		etat_couleur = 1;
	}
	if (val < 50)
	{
		etat_couleur = 3;
	}
}

//*************************************************************************************//
// BUZZER
//
//*************************************************************************************//

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

void turn_buzzer_on()
{
	// GPIOB.ODR |= 0x00000100;
	SET_BIT(GPIOB.ODR, 9);
}

void turn_buzzer_off()
{ // on éteint toutes les leds (de 4 à 7)
	// GPIOB.ODR &= 0xFFFFFDFF;
	CLEAR_BIT(GPIOB.ODR, 9);
}

void toggle_buzzer(int freq)
{
	if (debut_partie != 0 && (timecount % freq == 0))
	{
		TOGGLE_BIT(GPIOB.ODR, 9);
	}
}

void lancer_buzzer()
{
	if (5 < chronometre && chronometre <= 10)
	{
		toggle_buzzer(1000);
	}
	else if (3 < chronometre && chronometre <= 5)
	{
		toggle_buzzer(500);
	}
	else if (1 < chronometre && chronometre <= 3)
	{
		toggle_buzzer(250);
	}
	else if (chronometre == 1)
	{
		toggle_buzzer(150);
	}
	else if (chronometre == 0)
	{
		toggle_buzzer(1);
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

int get_random_for_red_leds()
{
	return (rand() % RAND_NUM_MAX + 1) + RAND_NUM_MIN;
}

void toggle_red_leds()
{
	if (timecount % random_interval_red_leds == 0)
	{
		TOGGLE_BIT(GPIOA.ODR, 4);
		TOGGLE_BIT(GPIOA.ODR, 5);
		TOGGLE_BIT(GPIOA.ODR, 6);
		TOGGLE_BIT(GPIOA.ODR, 7);
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
	timecount++;

	toggle_red_leds();
	lancer_buzzer();
	verifie_etat_couleur();
	change_couleur_RGB();

	if (timecount == debut_partie + DUREE_PARTIE)
	{
		fin_partie = 1;
	}

	if (debut_partie != 0 && (timecount % 1000 == 0))
	{
		if (chronometre == 1)
		{
			printf("%d", chronometre);
		}
		else
		{
			printf("%d,", chronometre);
		}
		chronometre -= 1;
	}
}

//*************************************************************************************//

//*************************************************************************************//
// FONCTIONS JEUX
//
//*************************************************************************************//

void reset_game()
{
	nb_blue_button_pressed = 0;
	debut_partie = 0;
	fin_partie = 0;
	chronometre = DUREE_PARTIE_SEC;
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
	printf("----------------------------------------------\r\n");
	printf("Press the blue button twice to start\r\n");

	turn_red_leds_off();

	systick_init(1000); // une fois chaque millisecond

	init_red_leds();

	init_RGB();

	init_buzzer();

	init_white_button();
	init_blue_button();

	while (1)
	{

		if (is_blue_button_pressed())
		{
			nb_blue_button_pressed += 1;

			while (is_blue_button_pressed())
				;
		}

		if (nb_blue_button_pressed == 2)
		{

			srand(timecount); // On commence par initialiser le générateur de nombre pseudo-aléatoires.

			debut_partie = timecount;

			random_interval_red_leds = get_random_for_red_leds();

			random_interval_user = 500;

			printf("Random interval = %d\r\n", random_interval_red_leds);

			while (!fin_partie)
			{
			}

			printf("\r\nPartie terminée !\r\n");
			printf("----------------------------------------------\r\n");
			printf("Press the blue button twice to start\r\n");
			reset_game();
		}
	}

	return EXIT_SUCCESS;
}

//*************************************************************************************//