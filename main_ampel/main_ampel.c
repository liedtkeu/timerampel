//***************************************************************************************
// Ampeltimer
// Version: 1.0
// Hardware: Controller ATMega8
// Autor: Liedtke Uwe
// Date: April 2020
//***************************************************************************************
// Verwertung der Hardware aus dem Projekt "APT Ampel"
// Timer 
// 
//***************************************************************************************
// Librery declaration
#include "main_ampel.h"
//***************************************************************************************


//=======================================================================================
//=======================================================================================
int main (void)
{
// Variables Initialization


	// Functions Initialization
	
PORTs_init();						// Init der Ein und Ausgabeports
variable_init();
TIMER_init();						
//set_sleep_mode (SLEEP_MODE_IDLE);	// setzt den Sleepmodus
sei(); // Zuschalten des globalen Interrupts
stunde = eeprom_read_byte(&stunde_E);  // DATEN aus dem EPROM werden zurückgeschrieben
minute = eeprom_read_byte(&minute_E);  // DATEN aus dem EPROM werden zurückgeschrieben
if (minute >= 59)
	{
	minute = 59; // Begrenzt den Minutenzaehler auf 60
	}

while(1)							//Loop forever
	{
		
	gruene_LED();				// Ausgabe der aktuellen Minuten über die grüne LED
	
	/*Aktion langer Tastendruck*/

	if(qbi(TLANG,FLAGS))
		{
		cbi(TLANG,FLAGS);  // Merker rucksetzen 
		tzahler = 6;  // hier wir verhindert das der kurze Tastendruck aktiviert wird
		// timer löschen	
		stunde = 0;  // Timer zurücksetzen 
		minute = 0;  // Timer zurücksetzen
		zeit2 = 4;  // alle grünen Lampen an
		}
	
	/* Aktion kurzer Tastendruck */ 
	
	if(qbi(TKURZ,FLAGS))
		{	
		cbi(TKURZ,FLAGS); // merker rucksetzen
		nummer = 60; // Timer für extra Programm wird gesetzt
		}

	
	/* Erkennung kurzer Tastendruck */ 
	
		if(tzahler >= 1  && tzahler <= 4)
		{
			if(qbi(TAKTIV,FLAGS) && !qbi(TKURZ,FLAGS))
				{
				sbi(TMERK,FLAGS);   // 	Taster wurde kurz gedrückt
				}
			else
				{
				if(qbi(TMERK,FLAGS) && !qbi(TAKTIV,FLAGS))
					{
					cbi(TMERK,FLAGS);	
					sbi(TKURZ,FLAGS);
					}	
				}		
		}
	
	
	/* Erkennung langer Tastendruck */ 
	
	if(tzahler > 20)
		{
		
		if(qbi(TAKTIV,FLAGS) && !qbi(TLANG,FLAGS))
			{
			cbi(TMERK,FLAGS);		//		Merker für kurzen Tastendruck zurücksetzen
			sbi(TLANG,FLAGS);		//      Merker für den langen Tastendruck setzen
			}
		}
	
	/* Programm Ablauf geht hier los  */
	
	if (zeit1 >= 13)  // Counter für den DEMO Mode läuft bis 13 und wird dann ruckgesetzt
		{
		zeit1 = 0;	  // rucksetzen auf 0
		}
	
	if (!qbi(SEPROM,FLAGS) && (minute == 1)) // Einmal pro Stunde Datensicherung
		{
		eeprom_write_byte(&stunde_E,stunde); // schreiben der Variable stunde in den Eprom
		eeprom_write_byte(&minute_E,minute); // schreiben der Variable minute in den Eprom
		sbi(SEPROM,FLAGS);					 // Verriegelung > sonst wird der Eprom kaputt geschrieben 
		}
	
	if (!qbi(SEPROM,FLAGS) && (minute == 10)) // Einmal pro Stunde Datensicherung
		{
		eeprom_write_byte(&minute_E,minute); // schreiben der Variable minute in den Eprom
		sbi(SEPROM,FLAGS);					 // Verriegelung > sonst wird der Eprom kaputt geschrieben
		}
		
	if (!qbi(SEPROM,FLAGS) && (minute == 20)) // Einmal pro Stunde Datensicherung
		{
		eeprom_write_byte(&minute_E,minute); // schreiben der Variable minute in den Eprom
		sbi(SEPROM,FLAGS);					 // Verriegelung > sonst wird der Eprom kaputt geschrieben
		}
	if (!qbi(SEPROM,FLAGS) && (minute == 30)) // Einmal pro Stunde Datensicherung
		{
		eeprom_write_byte(&minute_E,minute); // schreiben der Variable minute in den Eprom
		sbi(SEPROM,FLAGS);					 // Verriegelung > sonst wird der Eprom kaputt geschrieben
		}
	if (!qbi(SEPROM,FLAGS) && (minute == 40)) // Einmal pro Stunde Datensicherung
		{
		eeprom_write_byte(&minute_E,minute); // schreiben der Variable minute in den Eprom
		sbi(SEPROM,FLAGS);					 // Verriegelung > sonst wird der Eprom kaputt geschrieben
		}
	if (!qbi(SEPROM,FLAGS) && (minute == 50)) // Einmal pro Stunde Datensicherung
		{
		eeprom_write_byte(&minute_E,minute); // schreiben der Variable minute in den Eprom
		sbi(SEPROM,FLAGS);					 // Verriegelung > sonst wird der Eprom kaputt geschrieben
		}			
		
	if (qbi(SEPROM,FLAGS) && (minute == 5)) // Rücksetzen des FLAGS
		{
		cbi(SEPROM,FLAGS);					// Verriegelung zurücksetzen.
		}
	if (qbi(SEPROM,FLAGS) && (minute == 15)) // Rücksetzen des FLAGS
		{
		cbi(SEPROM,FLAGS);					// Verriegelung zurücksetzen.
		}	
	if (qbi(SEPROM,FLAGS) && (minute == 25)) // Rücksetzen des FLAGS
		{
		cbi(SEPROM,FLAGS);					// Verriegelung zurücksetzen.
		}
	if (qbi(SEPROM,FLAGS) && (minute == 35)) // Rücksetzen des FLAGS
		{
		cbi(SEPROM,FLAGS);					// Verriegelung zurücksetzen.
		}
	if (qbi(SEPROM,FLAGS) && (minute == 45)) // Rücksetzen des FLAGS
		{
		cbi(SEPROM,FLAGS);					// Verriegelung zurücksetzen.
		}	
	if (qbi(SEPROM,FLAGS) && (minute == 55)) // Rücksetzen des FLAGS
		{
		cbi(SEPROM,FLAGS);					// Verriegelung zurücksetzen.
		
		}
		
		
		
			
	if(nummer)
		{
		ampel(zeit1);		// bei aktiven DEMO mode wird alle sekunde die LED Ampel weiter geschaltet 
		}
	else
		{					// Hauptprogramm !!
			if(stunde == 5)
				{
				zeit2 = 1;
				}
			if(stunde == 14)
				{
				zeit2 = 2;
				}
			if(stunde == 21)
				{
				zeit2 = 3;
				}
			if(stunde == 28)
				{
				zeit2 = 4;
				}
			if(stunde == 35)
				{
				zeit2 = 5;
				}
			if(stunde == 42)
				{
				zeit2 = 6;
				}
			if(stunde == 49)
				{
				zeit2 = 7;
				}
			if(stunde == 56)
				{
				zeit2 = 8;
				}
			if(stunde == 63)
				{
				zeit2 = 9;
				}
			if(stunde == 70)
				{
				zeit2 = 10;
				}
			if(stunde == 77)
				{
				zeit2 = 11;
				}
			if(stunde >= 84)
				{
				zeit2 = 12;
				}
			if(qbi(sekundenw,FLAGS) && (stunde < 6))
				{
				ampel(1);	
				}
			else
				{
				ampel(zeit2);	
				}	
		}
	}
		
}		
		
	
	
		
		


void gruene_LED(void)
{
if (second == 1)
	{
	tminute = minute; // Minutenwert wird übernommen 	
	}		
if (second == 30)
	{
	if(minute < 10)
		{
		tminute = minute; // Minutenwert wird übernommen 	
		}	
	}	
if(second & 1)
	{ // gerade Zahl
	if(tminute)
		{
		sbi(LED_ON,LPORT3);	// blinkt solange der Minutenzaehler größer Null
		if(!qbi(ZMODUS,FLAGS))
			{
			tminute--;     // temporäre Minuten Zähler wird zurückgezählt
			sbi(ZMODUS,FLAGS); // Verriegelung damit pro Sekunde nur einmal zurückgezählt wird
			}
		}	
	}
else 
	{ // ungerade Zahl 
	cbi(LED_ON,LPORT3);
	cbi(ZMODUS,FLAGS);
	}
}
//=======================================================================================
// INIT der Ein und Ausgabeports
// 0 zu Eingang und 1 zu Ausgang
//=======================================================================================
void PORTs_init(void)
{

// PORT0 = Temperatursensor
// PORT1 = Schaltausgang für Heizung

// PORT6 = LEDC Rot
// PORT7 = LEDD Rot
	DDRB = 0b11000000;
	PORTB =0b00000000;


// PORT C
// PORT0 = LEDA Grün
// PORT1 = LEDB Grün
// PORT2 = 
// PORT3 = 
// PORT4 = LEDA Rot
// PORT5 = LEDB Rot

	DDRC =  0b00110011;
	PORTC = 0b00000000;

// PORT0 = grüne LED 
// PORT3 = Taster 
// PORT4 = Stromversorgung Sensor
// PORT5 = LEDC Grün
// PORT6 = LEDD Grün


	DDRD =  0b01100001;
	PORTD = 0b00001000;
}


void variable_init(void)
{
OSCCAL = 0x9F;						// Offset Kalibrierung RC Oszilator  
FLAGS = 0; 							// auf 0
}


//=======================================================================================
// Timer INIT
//=======================================================================================
void TIMER_init(void)
{

/***************************************TIMER 0******************************************/ 
TCCR0 = 0x05;			// Teilung 1024 1M / 1024 / 256 = 262,144ms 
sbi(TOIE0,TIMSK);		// Timer0 Overflow Interrupt Enable

/***************************************TIMER 1******************************************/ 
TCCR1B = (1<<WGM12) | (1<<CS10);
OCR1A = OCRx_RELOAD;
// OCR1A Interrupt freigeben, wird für Timer CTC mode benutzt
sbi(OCIE1A,TIMSK);

/***************************************TIMER 2******************************************/
//nicht genutzt
}

//schaltet die Ampel entsprechend der Übergabe 
// 12 Stufen sind möglich
void ampel(int stufe)
{
if (stufe == 0)
	{
	cbi(LEDA_R,LPORT2);				// alle Lampen aus
	cbi(LEDA_G,LPORT2);
	cbi(LEDB_R,LPORT2);
	cbi(LEDB_G,LPORT2);
	cbi(LEDC_R,LPORT1);			
	cbi(LEDC_G,LPORT3);				
	cbi(LEDD_R,LPORT1);
	cbi(LEDD_G,LPORT3);		
	}

if (stufe == 1)
	{
	cbi(LEDA_R,LPORT2);				// A GRÜN AN 
	sbi(LEDA_G,LPORT2);
	cbi(LEDB_R,LPORT2);
	cbi(LEDB_G,LPORT2);
	cbi(LEDC_R,LPORT1);
	cbi(LEDC_G,LPORT3);
	cbi(LEDD_R,LPORT1);
	cbi(LEDD_G,LPORT3);
		
	}
if (stufe == 2)
	{
	cbi(LEDA_R,LPORT2);				// A + B GRÜN AN 
	sbi(LEDA_G,LPORT2);
	cbi(LEDB_R,LPORT2);
	sbi(LEDB_G,LPORT2);
	cbi(LEDC_R,LPORT1);
	cbi(LEDC_G,LPORT3);
	cbi(LEDD_R,LPORT1);
	cbi(LEDD_G,LPORT3);
	}
if (stufe == 3)
	{
	cbi(LEDA_R,LPORT2);				// A + B + C GRüN an 
	sbi(LEDA_G,LPORT2);
	cbi(LEDB_R,LPORT2);
	sbi(LEDB_G,LPORT2);
	cbi(LEDC_R,LPORT1);
	sbi(LEDC_G,LPORT3);
	cbi(LEDD_R,LPORT1);
	cbi(LEDD_G,LPORT3);
	}
if (stufe == 4)
	{
	cbi(LEDA_R,LPORT2);				// A + B + C + D Grün an 
	sbi(LEDA_G,LPORT2);
	cbi(LEDB_R,LPORT2);
	sbi(LEDB_G,LPORT2);
	cbi(LEDC_R,LPORT1);
	sbi(LEDC_G,LPORT3);
	cbi(LEDD_R,LPORT1);
	sbi(LEDD_G,LPORT3);
	}
if (stufe == 5)
	{
	sbi(LEDA_R,LPORT2);				// nur A Grün u ROT an
	sbi(LEDA_G,LPORT2);
	cbi(LEDB_R,LPORT2);
	cbi(LEDB_G,LPORT2);
	cbi(LEDC_R,LPORT1);
	cbi(LEDC_G,LPORT3);
	cbi(LEDD_R,LPORT1);
	cbi(LEDD_G,LPORT3);	
	}
if (stufe == 6)
	{
	sbi(LEDA_R,LPORT2);				// A + B Grün u ROT an
	sbi(LEDA_G,LPORT2);
	sbi(LEDB_R,LPORT2);
	sbi(LEDB_G,LPORT2);
	cbi(LEDC_R,LPORT1);
	cbi(LEDC_G,LPORT3);
	cbi(LEDD_R,LPORT1);
	cbi(LEDD_G,LPORT3);	
	}
if (stufe == 7)
	{
	sbi(LEDA_R,LPORT2);				// A + B + C Grün u ROT an
	sbi(LEDA_G,LPORT2);
	sbi(LEDB_R,LPORT2);
	sbi(LEDB_G,LPORT2);
	sbi(LEDC_R,LPORT1);
	sbi(LEDC_G,LPORT3);
	cbi(LEDD_R,LPORT1);
	cbi(LEDD_G,LPORT3);
	}
if (stufe == 8)
	{
	sbi(LEDA_R,LPORT2);				// A + B + C + D Grün u ROT an
	sbi(LEDA_G,LPORT2);
	sbi(LEDB_R,LPORT2);
	sbi(LEDB_G,LPORT2);
	sbi(LEDC_R,LPORT1);
	sbi(LEDC_G,LPORT3);
	sbi(LEDD_R,LPORT1);
	sbi(LEDD_G,LPORT3);	
	}
if (stufe == 9)
	{
	sbi(LEDA_R,LPORT2);				// A ROT AN
	cbi(LEDA_G,LPORT2);
	cbi(LEDB_R,LPORT2);
	cbi(LEDB_G,LPORT2);
	cbi(LEDC_R,LPORT1);
	cbi(LEDC_G,LPORT3);
	cbi(LEDD_R,LPORT1);
	cbi(LEDD_G,LPORT3);	
	}
if (stufe == 10)
	{
	sbi(LEDA_R,LPORT2);				// A + B ROT AN
	cbi(LEDA_G,LPORT2);
	sbi(LEDB_R,LPORT2);
	cbi(LEDB_G,LPORT2);
	cbi(LEDC_R,LPORT1);
	cbi(LEDC_G,LPORT3);
	cbi(LEDD_R,LPORT1);
	cbi(LEDD_G,LPORT3);
	}
if (stufe == 11)
	{
	sbi(LEDA_R,LPORT2);				// A + B + C ROT AN
	cbi(LEDA_G,LPORT2);
	sbi(LEDB_R,LPORT2);
	cbi(LEDB_G,LPORT2);
	sbi(LEDC_R,LPORT1);
	cbi(LEDC_G,LPORT3);
	cbi(LEDD_R,LPORT1);
	cbi(LEDD_G,LPORT3);
	}
if (stufe == 12)
	{
	sbi(LEDA_R,LPORT2);				// A + B + C + D ROT AN
	cbi(LEDA_G,LPORT2);
	sbi(LEDB_R,LPORT2);
	cbi(LEDB_G,LPORT2);
	sbi(LEDC_R,LPORT1);
	cbi(LEDC_G,LPORT3);
	sbi(LEDD_R,LPORT1);
	cbi(LEDD_G,LPORT3);
	}
}

