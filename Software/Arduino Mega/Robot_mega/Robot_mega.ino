
#include <LedControl.h>  //  BIBLIOTEKA LED MATRIX
#include <LiquidCrystal.h> //  BIBLIOTEKA WYSWIETLACZA LCD

LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //DEFINICJA MAGISTRALI DANYCH LCD

LedControl lc=LedControl(12,11,10,1); //


//define ILOSC_PRZYCISKOW 8
//int g_wersja_mp3[ILOSC_PRZYCISKOW+1];

const char g_jezyki[12][12] = {"Polski.... ","Angielski. ","Chinski... ","Niemiecki. ","Slonski... ","Kaszubski. ","Rosyjski.. ","Wloski.....","Rumunski.. ","Czeski.... ","Slowacki...","Portugalski"};
const char emp[1][5] = {"mp3 "};
static int facet=0, laska=0, cisza=0, piosenka1=0, piosenka2=0;
 
void setup() {
////
 lc.shutdown(0,false);// turn off power saving, enables display
 lc.setIntensity(0,3);// sets brightness (0~15 possible values)
 lc.clearDisplay(0);// clear screen
////


 LedControl lc=LedControl(2,5,4,1); //
 //JAK POLACZYC ARDUINO Z MP3 TRIGGER
 //     Ardu ---  Mp3Trigger
 //  0   RX  ---  TX
 //  1   TX  ---  RX
 //     GND  ---  GND
 lc.shutdown(0,false);// turn off power saving, enables display
 lc.setIntensity(0,1);// sets brightness (0~15 possible values)
 lc.clearDisplay(0);// clear screen

 lcd.begin(16, 2);    //DEKLARACJA TYPU LCD
 lcd.setCursor(0, 0); //POZYCJA KURSORA
 lcd.print("    R O B O T");  //TEKST NA LCD
 lcd.setCursor(0, 1);
 lcd.print("   D I P O L A");

 //INICJALIZACJA PORTU
 Serial.begin(38400); //MP3 TRIGGER WYMAGA PREDKOSCI PORTU: 38400
 Serial1.begin(38400);

 //DEFINICJA PRZYCISKOW ODPOWIEDZIALNYCH ZA DANY JEZYK (NR PINU NA PLYTCE)
 pinMode(46, INPUT_PULLUP); // JEZYK PLUS
 pinMode(47, INPUT_PULLUP); // JEZYK MINUS

 //DEFINICJA PRZYCISKOW NA PILOCIE (NR PINU NA PLYTCE)
 pinMode(30, INPUT_PULLUP); // MP3_0
 pinMode(31, INPUT_PULLUP); // MP3_1
 pinMode(32, INPUT_PULLUP); // MP3_2
 pinMode(33, INPUT_PULLUP); // MP3_3
 pinMode(34, INPUT_PULLUP); // MP3_4
 pinMode(35, INPUT_PULLUP); // MP3_5
 pinMode(36, INPUT_PULLUP); // MP3_6
 pinMode(37, INPUT_PULLUP); // MP3_7
 pinMode(38, INPUT_PULLUP); // MP3_8
 pinMode(39, INPUT_PULLUP); // MP3_9


}

int sprawdz_jezyk() {
 static int jezyk=0, jezyk_przycisk=0;


 if (digitalRead(46) == LOW) {
   if(jezyk_przycisk != 1)
     jezyk = (jezyk+1)%6; //po znaku % wpisac ilosc jezykow
   jezyk_przycisk=1;
   lc.clearDisplay(0);// clear screen
    LedControl lc=LedControl(12,11,10,1); //
   lc.setIntensity(0,3);// sets brightness (0~15 possible values)
   lc.clearDisplay(0);// clear screen
   lc.shutdown(0,true);
   lc.shutdown(0,false);
   delay(100);
 }else if (digitalRead(47) == LOW){ 
   if(jezyk_przycisk != 2){
     jezyk = jezyk-1;
     if(jezyk < 0)
       jezyk = 5; // tu wpisac ilosc jezykow minus 1
   }
   jezyk_przycisk=2;
   lc.clearDisplay(0);// clear screen
    LedControl lc=LedControl(12,11,10,1); //
   lc.setIntensity(0,3);// sets brightness (0~15 possible values)
   lc.clearDisplay(0);// clear screen
   lc.shutdown(0,true);
   lc.shutdown(0,false);
   delay(100);
    }else if (digitalRead(30) == LOW){ 
   {
     jezyk = 0;
    
   }
   jezyk_przycisk=2;
   lc.clearDisplay(0);// clear screen
    LedControl lc=LedControl(12,11,10,1); //
   lc.setIntensity(0,3);// sets brightness (0~15 possible values)
   lc.clearDisplay(0);// clear screen
   lc.shutdown(0,true);
   lc.shutdown(0,false);
   delay(100);
 }else{
   jezyk_przycisk=0;
 }
 
 return jezyk;
}

int sprawdz_przycisk(){ //FUNKCJA SPRAWDZA GUZIK
 int przycisk=1;
 for(int i=31; i<=39; i++){
   if(digitalRead(i) == LOW)
     return przycisk;
   przycisk++;
 }
 return -1;
}

void update_lcd(int jezyk, int przycisk, int mp3, int pokaz_wszystko) {
   lcd.clear();
   //lcd.begin(16, 2); //DEKLARACJA LCD
   lcd.setCursor(0, 0); //USTAWIENIE KURSORA NA LCD
   lcd.print(g_jezyki[jezyk]);
   lcd.print(9+jezyk);
   if(pokaz_wszystko != 0){
     lcd.setCursor(3,1); //LCD NR. MP3
     lcd.print(mp3);
     lcd.setCursor(15,0); //LCD NR. GUZIKA NA PILOCIE
     lcd.print(przycisk);
   }
}

void kolejna_zmiana_led( int jezyk, int mp3 ){
 static int wlaczony[8]={0,0,0,0,0,0,0,0};
 static int poz=0;
 const int led_map[8] = {0,1,2,3,4,5,6};
/* 
 for(int i=0; i<8; i++){
   if(wlaczony[poz] == 0)
     lc.setLed(0,poz,i,true);
   else
     lc.setLed(0,poz,i,false);
 }*/

 if(wlaczony[poz]==0){
   wlaczony[poz]=1;
  
    if(poz!=3){lc.setRow(0, led_map[poz], 255-mp3 );}
    lc.setRow(0, 3, 0);
    //lc.setRow(0, 3, pow ( 2, jezyk)-1);
   //switch on the led in the 3'rd row 8'th column of the device at addr=0 
   lc.setLed(0,3,jezyk,true);
   //Serial.write("wlacz row ");
 }else{
   wlaczony[poz]=0;
   if(poz!=3){lc.setRow(0, led_map[poz], mp3-1);}
   //lc.setRow(0, 3, 256 - pow( 2, jezyk));
   //Serial.write("wylacz row ");
   //switch on the led in the 3'rd row 8'th column of the device at addr=0 
   
   lc.setLed(0,3,jezyk,false);

 }

 //Serial.println(poz,HEX);
 poz = (poz+1)%7;
}

void loop() {



 static int old_jezyk=0, mp3=0, wyswietlany_przycisk=0, old_przycisk=-1;
 static int pierwsza_aktualizacja_lcd=0, przycisk_uzyty=0;
 int jezyk = sprawdz_jezyk();
 int przycisk = sprawdz_przycisk();
 kolejna_zmiana_led( jezyk, mp3 );

 if(jezyk!=old_jezyk || (pierwsza_aktualizacja_lcd == 0 && millis() > 2000 && przycisk_uzyty==0)) {
   pierwsza_aktualizacja_lcd = 1;
   update_lcd(jezyk, wyswietlany_przycisk, mp3, przycisk_uzyty);
   old_jezyk = jezyk;
 }

 if (przycisk != -1 && przycisk != old_przycisk) {
   przycisk_uzyty=1;
   //g_wersja_mp3[przycisk] = (g_wersja_mp3[przycisk]+1)%4;
   if (przycisk==1) {
    mp3=1+20*jezyk;
   } else if (przycisk==2) {
    mp3=2+20*jezyk;
   }else if(przycisk==3) {
    mp3=3+20*jezyk;
   }else if(przycisk==4) {
    mp3=254+cisza%3;
    cisza=cisza+1;
   }else if(przycisk==5) {
    mp3=4+facet%4+20*jezyk;
    facet=facet+1;
   }else if(przycisk==6) {
    mp3=8+laska%4+20*jezyk;
    laska=laska+1;
   }else if(przycisk==7) {
    mp3=12+piosenka1%4+20*jezyk;
    piosenka1=piosenka1+1;
   }else if(przycisk==8) {
    mp3=16+piosenka2%4+20*jezyk;
    piosenka2=piosenka2+1;
   }else if(przycisk==9) {
    mp3=253;
   
   }

  
   update_lcd(jezyk, przycisk, mp3, 1);
   wyswietlany_przycisk = przycisk;
   LedControl lc=LedControl(12,11,10,1); //
   lc.setIntensity(0,3);// sets brightness (0~15 possible values)
   lc.clearDisplay(0);// clear screen
   lc.shutdown(0,true);
   lc.shutdown(0,false);
 
   Serial.print("t");
   Serial.write( mp3 ); //WYSYLANIE NUMERU MP3 DO TRIG.
   
 //lc.shutdown(0,false);// turn off power saving, enables display
   lc.setIntensity(0,5);// sets brightness (0~15 possible values)
   lc.clearDisplay(0);// clear screen
   delay(60);
 }

 old_przycisk = przycisk;
 delay(60);
 
}
