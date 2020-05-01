//***************************************************************************************
// Librery declaration
#include "main_ampel.h"
//***************************************************************************************
//=======================================================================================
// Interrupt f�r Timer 2
//=======================================================================================
ISR(TIMER2_OVF_vect)				// Interrupt Timer 2
{
//nicht genutzt 
}


//=======================================================================================
// Interrupt f�r Timer 1
// wird als Wecker f�r den Sleepmodus benutzt
//=======================================================================================
ISR(TIMER1_OVF_vect)				// Interrupt Timer 1
{
// nicht genutzt		
}


ISR(TIMER1_COMPA_vect) 
{
static uint8_t prescaler=(uint8_t)F_TIMER;

if( --prescaler == 0 ) 
		{
		prescaler = (uint8_t)F_TIMER;
		second++;   // sekunden werden hochgezaehlt
		zeit1++;    // Timer f�r das DEMO (Kurztaste) wird hochgezaehlt 
		if(nummer)
				{
				nummer--;  // Timer f�r das DEMO wird im Hauptprogramm gesetzt und l�uft dann zur�ck
				} 
		  
		if(qbi(sekundenw,FLAGS))  // f�r das Blinken der LED's notwendig
			{
			cbi(sekundenw,FLAGS);
			}
		else
			{
			sbi(sekundenw,FLAGS); 	
			}	
		if( second == 60 )			// eine Minute vorueber 
			{
			second = 0;				// eine Minute vorbei
			minute++;				// Minuten hochzahlen 
			}	
		if( minute >= 59 )			// eine Stunde vorueber
			{
			minute = 0;				// eine Stunde vorbei
			stunde++;
			}
		}	                                    
		OCR1A = OCRx_RELOAD + REST;             // Rest behandeln
} 

//=======================================================================================
// Interrupt f�r Timer 0
// Teilung 1024 1M / 1024 / 256 = Aufruf alle 262,144 ms 
// F�r die Abfrage des Tasters 
//=======================================================================================
ISR(TIMER0_OVF_vect)		// Interrupt TIMER 0
{
if(qbi(TASTER,PIND))
	{
	cbi(TAKTIV,FLAGS);						// Taster in Ruhestellung
	if(tzahler)
		{
		tzahler--;							// zaehlt Zahler auf Null
		}
	}
else
	{
	sbi(TAKTIV,FLAGS);						// Taster gedr�ckt
	if(tzahler < 250)
		{
		tzahler++;							// Zur Erkennung eines langen Tastendrucks 
		}	
	}

}

