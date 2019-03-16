/* przyk³ad 2.2 "leds2.c" */
/* 8 diod LED prz³¹czonych do portu D */
/* ATmega 1MHz */

#define F_CPU 1000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


/*
1. Kalibracja uk³adu
2. Obroty w prawo, obroty w lewo
3. Chwycenie d³oni po podaniu impulsu zewnêtrznego 1
4. Obrót d³oni po podaniu impulsu zewnêtrznego 2
5. Ponowna kalibracja po okreœlonym czasie


*/

//Definicje sterowania dla portu C Atmega32, 0 - PC0, 1 - PC1, 2 - PC2, 3 - PC3
/*
  PC0 - Uzwojenie 1
  PC1 - Uzwojenie 2
  PC2 - Uzwojenie 3
  PC3 - Uzwojenie 4
  
  PC4 - Uzwojenie 1
  PC5 - Uzwojenie 2
  PC6 - Uzwojenie 3
  PC7 - Uzwojenie 4
  
*/
//Tablica dla silnika pierwszego K1

#define K1 0x20  // Krok pierwszy, Uzwojenie 4 ok
#define K2 0x21  // Krok drugi, Uzwojenie 4, 3 ok
#define K3 0x01  // Krok trzeci,Uzwojenie 3 ok
#define K4 0x03  // Krok czwarty, Uzwojenie 3, 2 ok
#define K5 0x02  // Krok pi¹ty, Uzwojenie 2 ok
#define K6 0x06  // Krok szósty, Uzwojenie 2, 1 ok
#define K7 0x04  // Krok siódmy, Uzwojenie 1
#define K8 0x24  // Krok ósmy, Uzwojenie 1,4

//Tablica dla silnika drugiego K2

#define KD1 0x40  // Krok pierwszy, Uzwojenie 4
#define KD2 0xC0  // Krok drugi, Uzwojenie 4, 3
#define KD3 0x80  // Krok trzeci,Uzwojenie 3
#define KD4 0x90  // Krok czwarty, Uzwojenie 3, 2
#define KD5 0x10  // Krok pi¹ty, Uzwojenie 2
#define KD6 0x18  // Krok szósty, Uzwojenie 2, 1
#define KD7 0x08  // Krok siódmy, Uzwojenie 1
#define KD8 0x48  // Krok ósmy, Uzwojenie 1,4

#define KD11 0x68  // Krok ósmy, Uzwojenie 4
#define KD12 0x29  // Krok ósmy, Uzwojenie 4,3
#define KD13 0x19  // Krok ósmy, Uzwojenie 3
#define KD14 0x13  // Krok ósmy, Uzwojenie 3,2  
#define KD15 0x92  // Krok ósmy, Uzwojenie 2
#define KD16 0x86  // Krok ósmy, Uzwojenie 2,1
#define KD17 0xC4  // Krok ósmy, Uzwojenie 1
#define KD18 0x64  // Krok ósmy, Uzwojenie 1,4


//s1 PRAWO LEWO
//s2 OBROT PRAWO

#define KR_S1 224 //Polo¿enie krañcowe silnik pierwszy (obroty prawo, lewo) 224
#define KR_S2 916 //Polo¿enie krañcowe silnik drugi (rêka) 916
#define KR_K_S1 190 //Polo¿enie krañcowe silnik pierwszy - druga strona (obroty prawo, lewo) 214
#define KR_K_S2 800 //Polo¿enie krañcowe silnik drugi - druga strona (rêka) 902

#define K_S1_Pelny 160 // Pe³ny obrót na ile pozwala ograniczenie mechaniczne - obroty prawo,lewo 204
#define K_S2_Pelny 700 // Pe³ny obrót na ile pozwala ograniczenie mechaniczne - rêka 870

#define K_S1_Plowa 80 // Polowa  - obroty prawo,lewo 102
#define K_S2_Polowa 350 // Pe³ny obrót na ile pozwala ograniczenie mechaniczne - rêka 436


//Tablica sterowania dla silnika pierwszego pod³aczonego do portu C

int tster_polkr_prawo_lewo[8] = {K8, K7, K6, K5, K4, K3, K2, K1}; // Metoda pó³krokowa
int tster_polkr_gora_dol[8] = {KD1, KD2, KD3, KD4, KD5, KD6, KD7, KD8}; // Metoda pó³krokowa
int tster_polkr_gora_dol_oba[8] = {KD18,KD17,KD16,KD15,KD14,KD13,KD12,KD11}; //{KD11,KD12,KD13,KD14,KD15,KD16,KD17,KD18}; 
	
int tster_pelnokr_prawo_lewo[4] = {K7,K5,K3,K1};
int tster_pelnokr_gora_dol[4] = {KD1,KD3,KD5,KD7};
int tster_pelnokr_gora_dol_oba[4] = {KD17,KD15,KD13,KD11};
	
int tster_pelnokr_gracja[4]={K7+KD7, K5+KD5, K3+KD3, K1+KD1};
int w=0;	
int i=0;	

//int krok=0; //Zapamiêtanie katualnej wartoœci kroku - indeks kroku od 0 - 7


//Deklaracja funkcji

	
	
void gora_prawo_polkr(int t_ster[], int kr, unsigned char ster);
void dol_lewo_polkr(int t_ster[], int kr, unsigned char ster)	;
void gora_prawo_pelnokr(int t_ster[], int kr,unsigned char ster);
void dol_lewo_pelnokr(int t_ster[], int kr, unsigned char ster);
void reka_o();
void cala_reka__wlacz();
void cala_reka__wylacz();
void polowa_reki_wlacz();
void polowa_reki_1_wlacz();
void fack_on();
void kalibracja();

void palec1_on();
void palec1_off();

void palec2_on();
void palec2_off();

void palec3_on();
void palec3_off();

void palec4_on();
void palec4_off();

void palec5_on();
void palec5_off();
void ster_pierwszy();
void ster_drugi();


//Obs³uga przerwañ

ISR(INT0_vect)
{
	cli();
	//gora_prawo_polkr(tster_polkr_prawo_lewo,Kat_90); reka scisnij
	for(int i=0;i<35;i++)
	{
		PORTC |=0xff;
		_delay_ms(1.1);
		PORTC &=0x00;
		_delay_ms(17); //19.4
	}
	
	_delay_ms(2000); // odczekaj 2 skundy
	
	for(int i=0;i<35;i++) // reka otworz
	{
		PORTC |=0xff;
		_delay_ms(2.2);
		PORTC &=0x00;
		_delay_ms(1); //19.4
		
	}
	
	sei();
	
}
/*
ISR(INT1_vect)
{

	
		cli();
		//gora_prawo_polkr(tster_polkr_prawo_lewo,Kat_90);
		for(int i=0;i<35;i++)
		{
			PORTC |=0xff;
			_delay_ms(2.2);
			PORTC &=0x00;
			_delay_ms(1); //19.4
			
		}

		sei();
	
}

*/

int main(void)
{
	DDRB=0xff;
    DDRC = 0xff; //Ustawienie lini PC0-PC7 jako wyjœcia (pierwszy silnik)
	
	
		
		//Globalna obs³uga przerwañ
		
		DDRD=0x00;
		PORTD=0xff;
		GIMSK |= _BV(INT0)| _BV(INT1);
		MCUCR |=  (0<<ISC01)| (0<<ISC00) | (0<<ISC11)| (0<<ISC10);
		sei();
	
		
	
		cala_reka__wylacz(); //Resetowanie palców
		kalibracja(); //Kalibracja reki
	
		

		for(;;)
		{
			for(int z=0;z<=20;z++)
			{
			
			ster_pierwszy();
			_delay_ms(6000);
			ster_drugi();
			_delay_ms(6000);
			
			
			
				if(z==20)
				{
					_delay_ms(10000);
					z=0;
					kalibracja();
				}
			
			}			
			
			
			}			
	
		}


void gora_prawo_polkr(int t_ster[], int kr, unsigned char ster)
{
	for(int j=0;j<=kr;j++)
	{
		for(int i=0;i<=7;i++)
		{
		
			PORTB =t_ster[i];
			_delay_ms(4);
			
		}
		if(ster>0)
		{
			for(int u=0;u<=j;u++)
			{
				_delay_ms(0.05);
			}
		}
	}
	PORTB=0x00;
}	

void dol_lewo_polkr(int t_ster[], int kr, unsigned char ster)
{
	
	for(int j=0;j<=kr;j++)
	{
		for(int i=7;i>=0;i--)
		{
			
			PORTB =t_ster[i];
			_delay_ms(4);
		if(ster>0)
		{
			for(int u=0;u<=j;u++)
			{
				_delay_ms(0.05);
			}
		}			
			
		}
		
		
	}
	PORTB=0x00;
}



void gora_prawo_pelnokr(int t_ster[], int kr,unsigned char ster)
{
	for(int j=0;j<=kr;j++)
	{
		for(int i=0;i<=3;i++)
		{
			
			PORTB =t_ster[i];
			_delay_ms(4);
			
		}
		if(ster>0)
		{
			for(int u=0;u<=j;u++)
			{
				_delay_ms(0.05);
			}
		}
	}
	PORTB=0x00;
}

void dol_lewo_pelnokr(int t_ster[], int kr,unsigned char ster)
{
	for(int j=0;j<=kr;j++)
	{
		for(int i=3;i>=0;i--)
		{
			
			PORTB =t_ster[i];
			_delay_ms(4);
			
			
		}
		if(ster>0)
		{
			for(int u=0;u<=j;u++)
			{
				_delay_ms(0.05);
			}
		}
	}
	PORTB=0x00;
}



/*Sterowanie palcami*/

void cala_reka__wlacz()
{
	
	for(int i=0;i<35;i++)
	{
		PORTC |=0xff;
		_delay_ms(1.1);
		PORTC &=0x00;
		_delay_ms(17); //19.4
	}

	
}

void cala_reka__wylacz()
{
	
	for(int i=0;i<35;i++)
	{
		PORTC |=0xff;
		_delay_ms(2.2);
		PORTC &=0x00;
		_delay_ms(17); //19.4
	}
	
	
}

void palec1_on()
{
	for(int i=0;i<35;i++)
	{
		PORTC |=0x01;
		_delay_ms(1.1);
		PORTC &=0xfe;
		_delay_ms(17); //19.4
	}
}	

void palec2_on()
{	
	for(int i=0;i<35;i++)
	{
		PORTC |=0x02;
		_delay_ms(1.1);
		PORTC &=0xfd;
		_delay_ms(17); //19.4
	}
}	

void palec3_on()
{	
	for(int i=0;i<35;i++)
	{
		PORTC |=0x04;
		_delay_ms(1.1);
		PORTC &=0xfb;
		_delay_ms(17); //19.4
	}
}	

void palec4_on()
{
	for(int i=0;i<35;i++)
	{
		PORTC |=0x08;
		_delay_ms(1.1);
		PORTC &=0xf7;
		_delay_ms(17); //19.4
	}
}

void palec5_on()
{	
	for(int i=0;i<35;i++)
	{
		PORTC |=0x10;
		_delay_ms(1.1);
		PORTC &=0xef;
		_delay_ms(17); //19.4
	}
}

//

void palec1_off()
{
	for(int i=0;i<35;i++)
	{
		PORTC |=0x01;
		_delay_ms(2.2);
		PORTC &=0xfe;
		_delay_ms(17); //19.4
	}
}

void palec2_off()
{
	for(int i=0;i<35;i++)
	{
		PORTC |=0x02;
		_delay_ms(2.2);
		PORTC &=0xfd;
		_delay_ms(17); //19.4
	}
}

void palec3_off()
{
	for(int i=0;i<35;i++)
	{
		PORTC |=0x04;
		_delay_ms(2.2);
		PORTC &=0xfb;
		_delay_ms(17); //19.4
	}
}

void palec4_off()
{
	for(int i=0;i<35;i++)
	{
		PORTC |=0x08;
		_delay_ms(2.2);
		PORTC &=0xf7;
		_delay_ms(17); //19.4
	}
}

void palec5_off()
{
	for(int i=0;i<35;i++)
	{
		PORTC |=0x10;
		_delay_ms(2.2);
		PORTC &=0xef;
		_delay_ms(17); //19.4
	}
}

void polowa_reki_wlacz()
{
	for(int i=0;i<10;i++)
	{
		PORTC |=0xff;
		_delay_ms(1.1);
		PORTC &=0x00;
		_delay_ms(17); //19.4
	}
	
	_delay_ms(1000);
	
	for(int i=0;i<25;i++)
	{
		PORTC |=0xff;
		_delay_ms(1.1);
		PORTC &=0x00;
		_delay_ms(17); //19.4
	}
	
		//_delay_ms(2000);
		//cala_reka__wylacz();
}

void polowa_reki_1_wlacz()
{
	
	for(int i=0;i<5;i++)
	{
		PORTC |=0x10;
		_delay_ms(1.1);
		PORTC &=0xef;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<10;i++)
	{
		PORTC |=0x08;
		_delay_ms(1.1);
		PORTC &=0xf7;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<15;i++)
	{
		PORTC |=0x04;
		_delay_ms(1.1);
		PORTC &=0xfb;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<20;i++)
	{
		PORTC |=0x02;
		_delay_ms(1.1);
		PORTC &=0xfd;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<25;i++)
	{
		PORTC |=0x01;
		_delay_ms(1.1);
		PORTC &=0xfe;
		_delay_ms(17); //19.4
	}
	
	
	_delay_ms(1000);
	
	for(int i=0;i<30;i++)
	{
		PORTC |=0xff;
		_delay_ms(1.1);
		PORTC &=0x00;
		_delay_ms(17); //19.4
	}
	
	
	
	
}
void fack_on()
{
	for(int i=0;i<35;i++)
	{
		PORTC |=0x01;
		_delay_ms(1);
		PORTC &=0xfe;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<35;i++)
	{
		PORTC |=0x02;
		_delay_ms(1);
		PORTC &=0xfd;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<35;i++)
	{
		PORTC |=0x08;
		_delay_ms(1);
		PORTC &=0xf7;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<35;i++)
	{
		PORTC |=0x10;
		_delay_ms(1);
		PORTC &=0xef;
		_delay_ms(17); //19.4
	}
}

void kalibracja()
{
	gora_prawo_pelnokr(tster_pelnokr_gora_dol_oba,400,0);
	dol_lewo_pelnokr(tster_pelnokr_gora_dol_oba,150,0);
	gora_prawo_pelnokr(tster_pelnokr_prawo_lewo,916,0);
	dol_lewo_pelnokr(tster_pelnokr_prawo_lewo,70,0);//380
	dol_lewo_pelnokr(tster_pelnokr_prawo_lewo,100,0); //lewo

}

void reka_o()
{	
	
	for(int i=0;i<35;i++)
	{
		PORTC |=0x02;
		_delay_ms(1.1);
		PORTC &=0xfd;
		_delay_ms(17); //19.4
	}
	
	
	for(int i=0;i<7;i++)
	{
		PORTC |=0x04;
		_delay_ms(1.1);
		PORTC &=0xfb;
		_delay_ms(17); //19.4
	}
	for(int i=0;i<4;i++)
	{
		PORTC |=0x08;
		_delay_ms(1.1);
		PORTC &=0xf7;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<2;i++)
	{
		PORTC |=0x10;
		_delay_ms(1.1);
		PORTC &=0xef;
		_delay_ms(17); //19.4
	}
	
}

void ster_pierwszy()
{
	
	dol_lewo_pelnokr(tster_pelnokr_gora_dol_oba,230,0); //dol +230
	_delay_ms(2000);
	polowa_reki_wlacz();
	_delay_ms(2000);
	cala_reka__wylacz();
	_delay_ms(2000);
	gora_prawo_polkr(tster_polkr_gora_dol,100,0); //gora 230-100 = 130 , lewo -100
	_delay_ms(2000);
	reka_o();
	_delay_ms(2000);
	dol_lewo_pelnokr(tster_pelnokr_prawo_lewo,150,0);// lewo -100 - 150 = -250
	//_delay_ms(2000);
	gora_prawo_pelnokr(tster_pelnokr_prawo_lewo,150,0); //prawo -250 +150 = -100
	//_delay_ms(2000);
	cala_reka__wylacz();
	gora_prawo_pelnokr(tster_pelnokr_prawo_lewo,100,0);// prawo -100+100 = 0
	_delay_ms(2000);
	gora_prawo_pelnokr(tster_pelnokr_gora_dol_oba,130,0);// gora -130 = 0
	_delay_ms(8000);
	
	// pozycja poczatkowa gora dol 0, prawo lewo 0
	
	dol_lewo_pelnokr(tster_pelnokr_gora_dol_oba,120,0); // dol +120
	_delay_ms(2000);
	dol_lewo_pelnokr(tster_pelnokr_prawo_lewo,250,0);//lewo -250
	_delay_ms(2000);
	polowa_reki_wlacz();
	_delay_ms(2000);
	gora_prawo_pelnokr(tster_pelnokr_prawo_lewo,250,0); //prawo -250 +250=0
	_delay_ms(2000);
	cala_reka__wylacz();
	_delay_ms(2000);
	gora_prawo_pelnokr(tster_pelnokr_gora_dol_oba,120,0);//gora+120 -120 = 0
	_delay_ms(2000);
	
	// pozycja poczatkowa gora dol 0, prawo lewo 0
	
	dol_lewo_pelnokr(tster_pelnokr_gora_dol_oba,160,0);
	_delay_ms(2000);
	gora_prawo_pelnokr(tster_pelnokr_prawo_lewo,15,0); //prawo -250 +250=0
	dol_lewo_pelnokr(tster_pelnokr_prawo_lewo,215,0);
	gora_prawo_pelnokr(tster_pelnokr_prawo_lewo,200,0); //prawo -250 +250=0
	gora_prawo_pelnokr(tster_pelnokr_gora_dol_oba,160,0);
	_delay_ms(8000);
	
	// pozycja poczatkowa gora dol 0, prawo lewo 0
	
	dol_lewo_pelnokr(tster_pelnokr_gora_dol_oba,160,0);
	_delay_ms(2000);
	dol_lewo_pelnokr(tster_pelnokr_prawo_lewo,440,0);
	_delay_ms(2000);
	polowa_reki_1_wlacz();
	gora_prawo_pelnokr(tster_pelnokr_gora_dol_oba,160,0);
	cala_reka__wylacz();
	_delay_ms(2000);
	
	// pozycja poczatkowa gora dol 0, prawo lewo 0
	
	dol_lewo_pelnokr(tster_pelnokr_gora_dol_oba,160,0);
	_delay_ms(2000);
	gora_prawo_pelnokr(tster_pelnokr_prawo_lewo,440,0);
	_delay_ms(2000);
	gora_prawo_pelnokr(tster_pelnokr_gora_dol_oba,160,0);
	_delay_ms(2000);
	
	
}


void ster_drugi()
{
	
	dol_lewo_pelnokr(tster_pelnokr_gora_dol_oba,150,0); // (-150 gd)
	dol_lewo_pelnokr(tster_pelnokr_prawo_lewo, 440,0);//  [lewo -440 pl]
	
	
	//
	_delay_ms(2000);
	
	for(int i=0;i<5;i++)
	{
		PORTC |=0x10;
		_delay_ms(1.1);
		PORTC &=0xef;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<10;i++)
	{
		PORTC |=0x08;
		_delay_ms(1.1);
		PORTC &=0xf7;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<15;i++)
	{
		PORTC |=0x04;
		_delay_ms(1.1);
		PORTC &=0xfb;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<20;i++)
	{
		PORTC |=0x02;
		_delay_ms(1.1);
		PORTC &=0xfd;
		_delay_ms(17); //19.4
	}
	
	cala_reka__wylacz();
	_delay_ms(1000);
	//
	
	for(int i=0;i<20;i++)
	{
		PORTC |=0x02;
		_delay_ms(1.1);
		PORTC &=0xfd;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<15;i++)
	{
		PORTC |=0x04;
		_delay_ms(1.1);
		PORTC &=0xfb;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<10;i++)
	{
		PORTC |=0x08;
		_delay_ms(1.1);
		PORTC &=0xf7;
		_delay_ms(17); //19.4
	}
	
	
	for(int i=0;i<5;i++)
	{
		PORTC |=0x10;
		_delay_ms(1.1);
		PORTC &=0xef;
		_delay_ms(17); //19.4
	}
	
	cala_reka__wylacz();
	
	_delay_ms(2000);
	
	gora_prawo_pelnokr(tster_pelnokr_prawo_lewo,300,0);// -440 + 300 = [-140 pl]

	dol_lewo_pelnokr(tster_pelnokr_prawo_lewo,300,0);// -140 - 300 = [-440 pl]
	
	polowa_reki_1_wlacz();
	
	gora_prawo_pelnokr(tster_pelnokr_gora_dol_oba,75,0);// -150+75 = (-75)

	cala_reka__wylacz();
	gora_prawo_pelnokr(tster_pelnokr_gora_dol_oba,75,0);// -75 + 75 = (0 gd)
	_delay_ms(2000);
	reka_o();
	_delay_ms(2000);
	
	dol_lewo_pelnokr(tster_pelnokr_gora_dol,150,0);// -0-150 = (-150gd), -440 + 150 = [-290 pl]
	gora_prawo_pelnokr(tster_pelnokr_prawo_lewo,150,0); //-290 + 150 = [-140]
	
	dol_lewo_pelnokr(tster_pelnokr_prawo_lewo,300,0); //-140-300 = [-440]
	cala_reka__wylacz();
	_delay_ms(2000);
	gora_prawo_pelnokr(tster_pelnokr_prawo_lewo,440,0);// -440 + 440 = 0 pl
	
	_delay_ms(2000);
	gora_prawo_pelnokr(tster_pelnokr_gora_dol_oba,150,0);// -150 + 150 = 0
	_delay_ms(2000);
	
	for(int i=0;i<5;i++)
	{
		PORTC |=0x10;
		_delay_ms(1.1);
		PORTC &=0xef;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<10;i++)
	{
		PORTC |=0x08;
		_delay_ms(1.1);
		PORTC &=0xf7;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<15;i++)
	{
		PORTC |=0x04;
		_delay_ms(1.1);
		PORTC &=0xfb;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<20;i++)
	{
		PORTC |=0x02;
		_delay_ms(1.1);
		PORTC &=0xfd;
		_delay_ms(17); //19.4
	}
	
	cala_reka__wylacz();
	_delay_ms(1000);
	for(int i=0;i<5;i++)
	{
		PORTC |=0x10;
		_delay_ms(1.1);
		PORTC &=0xef;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<10;i++)
	{
		PORTC |=0x08;
		_delay_ms(1.1);
		PORTC &=0xf7;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<15;i++)
	{
		PORTC |=0x04;
		_delay_ms(1.1);
		PORTC &=0xfb;
		_delay_ms(17); //19.4
	}
	
	for(int i=0;i<20;i++)
	{
		PORTC |=0x02;
		_delay_ms(1.1);
		PORTC &=0xfd;
		_delay_ms(17); //19.4
	}
	cala_reka__wylacz();
	_delay_ms(2000);
	
}