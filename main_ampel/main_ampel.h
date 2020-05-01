// Librery declaration
#define F_CPU 1000000UL

#include <stdint.h>
#include <avr/interrupt.h>			// alt WINAVR
#include <util/delay.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <avr/eeprom.h> 	


// **************************************************************************************
// Constants and  Macros definitions

// Macros definitions
#define sbi(TEST, PORT) PORT |= (1 << TEST)
#define cbi(TEST, PORT) PORT &= ~(1 << TEST)
#define qbi(TEST, PORT) ( (PORT & (0x01 << TEST))>>TEST )
#define LOW(x)  ((x) & 0xFF)				/* Makro zum Lowbyte Zugriff eines Word */
#define HIGH(x)   (((x) >> 8) & 0xFF)		/* Makro zum Highbyte Zugriff eines Word */



// Constants definitions
#define false 0
#define true 1
#define uchar unsigned char
#define uint unsigned int
#define bit uchar
#define idata
#define code
#define CLEAR 0

#define F_TIMER      256L							// Timer1 ISR-Frequenz
#define T1_PRESCALER 1								// Hardware-Vorteiler für Timer 1,
													// muss mit der Konfiguratiom übereinstimmen!
#define OCRx_RELOAD ((F_CPU / (T1_PRESCALER * F_TIMER)) -1) // Periodendauer pro Interrupt /Timertakte
													// -1, weil die Timerperiode OCR1x +1 ist
#define REST         (F_CPU % (T1_PRESCALER * F_TIMER))     // Divisionsrest
#if (OCRx_RELOAD+REST) > 65535
#error Ueberlauf in Timer1! F_TIMER oder T1_PRESCALER erhöhen.
#endif

#define LEDA_G				0		// LED A GRÜN PORTC
#define LEDA_R				4		// LED A ROT  PORTC
#define LEDB_G				1		// LED B GRÜN PORTC
#define LEDB_R				5		// LED B ROT  PORTC
#define LEDC_G				5		// LED C GRÜN PORTD
#define LEDC_R				6		// LED C ROT  PORTB
#define LEDD_G				6		// LED D GRÜN PORTD
#define LEDD_R				7		// LED D ROT  PORTB
#define TSENS1				0		// Anschluss Temperatursensor
#define POWERS				1		//  Versorgung Tempsensor PORTB
#define TASTER				3		// Taster
#define LED_ON				0		// Status LED auf Board (grün) PORTD

//FLAGS
#define BLINKON				0		// 1. FLAG für das Blinken der Ampel
#define sekundenw			1		// Sekundenwechsel
#define TAKTIV				2		// Taster Aktiv / Passiv
#define TLANG				3		// Taster wurde lang gedrückt
#define TKURZ				4		// Taster wurde kurz gedrückt
#define TMERK				5		// Merker für Tastendruck KURZ 
#define ZMODUS				6		// Verhindert das schnelle runterlaufen von Variable tminute
#define SEPROM				7		// Datensicherung in EPROM


#define LPORT1				PORTB	// LED PORT1
#define LPORT2				PORTC	// LED PORT2
#define LPORT3				PORTD	// LED PORT3
#define TEMP_P				PORTC	// Digitale Sensoren 
#define TEMP_D				DDRC	// Datenrichtungsregister für Temperatursensor 

// Global Variables


uint8_t volatile tzahler; 			// erkennt langeren tastendruck
uint8_t volatile zeit1;				// Stundentimer 
uint8_t volatile zeit2;				// Minutentimer 
uint8_t volatile second;			// Sekundenzaehler
uint8_t	volatile minute;			// Minutenzaehler
uint8_t	volatile tminute;			// Minutenzaehler, wird aber fuer die LED Anzeige runter gezaehlt.
uint8_t	volatile stunde;			// Stundenzahler	
uint8_t volatile nummer;		    // Fuer den DEMO Mode 
uint8_t volatile FLAGS;				// Die FLAGS halt, siehe oben
static uint16_t DEFEKT EEMEM;		// Dieser Bereich des Eproms ist defekt
static uint8_t minute_E EEMEM;		// Speichern der aktuellen Minute in den Eprom  
static uint8_t stunde_E EEMEM;		// Speichern der aktuellen Stunde in den Eprom


//=======================================================================================
// Funktionen in der Datei main_th.c
void gruene_LED(void);										// gruene LED gibt Minute Zaehler aus 
void PORTs_init(void);										// Portinit
void variable_init(void);
void TIMER_init(void);
void ampel(int stufe);										//vereinfachtes schalten der Ampel
