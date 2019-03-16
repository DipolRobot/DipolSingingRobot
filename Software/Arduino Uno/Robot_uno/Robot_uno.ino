
//#include<time.h>
#define przekaznik 13
#define przekaznikPupy 12
#define przekaznikBioder 11
#define magnes 8
#define magnesPupy 7
#define pilot 10
int stanSilnika = LOW;
int stanSilnikaPupy = LOW;
int stanSilnikaBioder = LOW;
long randNumber;

unsigned long aktualnyCzas = 0;
unsigned long zapamietanyCzas = 0;
unsigned long zapamietanyCzasPupy = 0;
unsigned long zapamietanyCzasBioder = 0;
unsigned long czasWlaczeniaSilnika = 0;
unsigned long czasWylaczeniaSilnika = 0;
unsigned long czasWlaczeniaSilnikaPupy = 0;
unsigned long czasWylaczeniaSilnikaPupy = 0;
unsigned long czasWlaczeniaSilnikaBioder = 0;
unsigned long czasWylaczeniaSilnikaBioder = 0;
unsigned long czasPilota = 16000; // Ile milisekund po naciśnięciu pilota ma ręka być podniesiona
unsigned long Czas = 0;

void setup() {
  Serial.begin(9600);
  pinMode(pilot, INPUT_PULLUP); //Sygnał pilota jako wejście
  pinMode(przekaznik, OUTPUT); //Przekaźnik włączający silnik ramienia
  pinMode(magnes, INPUT); //czujnik Halla jako wejście
  pinMode(przekaznikPupy, OUTPUT); //Przekaźnik włączający silnik ramienia
  pinMode(magnesPupy, INPUT); //czujnik Halla jako wejście
  pinMode(przekaznikBioder, OUTPUT); //Przekaźnik włączający silnik ramienia
  randomSeed(analogRead(0));
}
void loop() {
  //Pobierz liczbe milisekund od startu
  aktualnyCzas = millis();
  if (digitalRead(pilot) == LOW) {  // Jeśli pilot został naciśnięty i nie upłynął czasPilota

    delay(1);
    if (digitalRead(pilot) == LOW) {

      Czas = aktualnyCzas + czasPilota;
    }
  }
  if (Czas >= aktualnyCzas) {
    if (digitalRead(magnes) == LOW) { //jeśli magnes ręki jest obok czujnika Halla
      czasWlaczeniaSilnika = 0;
      czasWylaczeniaSilnika = 6000;
    }
    else {
      czasWlaczeniaSilnika = 6000;
      czasWylaczeniaSilnika = 0;
    }
    if (digitalRead(magnesPupy) == LOW) { //jeśli magnes pupy jest obok czujnika Halla
      czasWlaczeniaSilnikaPupy = 6000;
      czasWylaczeniaSilnikaPupy = 0;
    }
    else {
      czasWlaczeniaSilnikaPupy = 0;
      czasWylaczeniaSilnikaPupy = 6000;
    }
    czasWlaczeniaSilnikaBioder = 0;
    czasWylaczeniaSilnikaBioder = 6000;
  }
  if (stanSilnika == LOW) {
    if (aktualnyCzas - zapamietanyCzas >= czasWylaczeniaSilnika) {
      czasWlaczeniaSilnika = random(800) + 400; //tu wpisać czasy silnika ręki dla pracy ciągłej
      Serial.println(czasWlaczeniaSilnika);
      zapamietanyCzas = aktualnyCzas;
      //zmieniamy stan silnika na przeciwny
      stanSilnika = HIGH; //silnik włączony
      digitalWrite(przekaznik, stanSilnika);
    }
  } else {
    if (aktualnyCzas - zapamietanyCzas >= czasWlaczeniaSilnika) {
      czasWylaczeniaSilnika = random(4000) + 8000; //tu wpisać czasy silnika ręki dla pracy ciągłej
      zapamietanyCzas = aktualnyCzas;
      //zmieniamy stan silnika na przeciwny
      stanSilnika = LOW; //stan silnika wyłączony
      digitalWrite(przekaznik, stanSilnika);
    }
  }
  if (stanSilnikaPupy == LOW) {
    if (aktualnyCzas - zapamietanyCzasPupy >= czasWylaczeniaSilnikaPupy) {
      czasWlaczeniaSilnikaPupy = 9000; //tu wpisać czasy silnika pupy dla pracy ciągłej
      zapamietanyCzasPupy = aktualnyCzas;
      //zmieniamy stan silnika pupy na przeciwny
      stanSilnikaPupy = HIGH; //silnik włączony
      digitalWrite(przekaznikPupy, stanSilnikaPupy);
    }
  } else {
    if (aktualnyCzas - zapamietanyCzasPupy >= czasWlaczeniaSilnikaPupy) {
      czasWylaczeniaSilnikaPupy = 500; //tu wpisać czasy silnika pupy dla pracy ciągłej
      zapamietanyCzasPupy = aktualnyCzas;
      //zmieniamy stan silnika na przeciwny
      stanSilnikaPupy = LOW; //stan silnika wyłączony
      digitalWrite(przekaznikPupy, stanSilnikaPupy);
    }
  }
  if (stanSilnikaBioder == LOW) {
    if (aktualnyCzas - zapamietanyCzasBioder >= czasWylaczeniaSilnikaBioder) {
      czasWlaczeniaSilnikaBioder = 10000; //tu wpisać czasy silnika bioder dla pracy ciągłej
      zapamietanyCzasBioder = aktualnyCzas;
      //zmieniamy stan silnika bioder na przeciwny
      stanSilnikaBioder = HIGH; //silnik bioder włączony
      digitalWrite(przekaznikBioder, stanSilnikaBioder);
    }
  } else {
    if (aktualnyCzas - zapamietanyCzasBioder >= czasWlaczeniaSilnikaBioder) {
      czasWylaczeniaSilnikaBioder = 500; //tu wpisać czasy silnika bioder dla pracy ciągłej
      zapamietanyCzasBioder = aktualnyCzas;
      //zmieniamy stan silnika Bioder na przeciwny
      stanSilnikaBioder = LOW; //stan silnika bioder wyłączony
      digitalWrite(przekaznikBioder, stanSilnikaBioder);
    }
    Serial.println(digitalRead(magnesPupy));
  }

}

