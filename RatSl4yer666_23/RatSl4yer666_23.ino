/*
 * Board: Arduino Pro tai Arduino Pro Mini
 * Processor: ATmega328P 8MHz 3.3V - toiminee myös muutki mutta käytetään tätä
 */
#include <avr/interrupt.h>
#include <avr/sleep.h>      // lepotila, interrupt
#include <avr/power.h>

#include <SoftwareSerial.h> // XBee ja kamera käyttää 

SoftwareSerial xbee(5, 6);  // RX, TX.  XBee serial pinnit

const int wakePin = 2;      // Interrupt pinni 

int wakeVal = 0;
int wakeState = LOW;

void wakeUpNow() {          // Interrupt
  sleep_disable();          // Poistutaan lepotilasta
  detachInterrupt(0);       // Interrupt väliaikaisesti pois käytöstä
  
} 

void setup() {
  Serial.begin(9600);
  //pinMode(10, OUTPUT);

  pinMode(5, INPUT);        // XBee:n RX ja TX, varmuuden vuoksi erikseen määritellään täällä
  pinMode(6, OUTPUT); 

  // Interrupt
  pinMode(wakePin, INPUT);  
  PCIFR = bit (PCIF1);
  //ADCSRA = 0x00;  // ADC pois

  //GIMSK &= ~_BV(INT0); // disable hardware INT0
  //MCUCR &= ~(_BV(ISC01) | _BV(ISC00)); // set hardware INT0 active on low LEVEL

  // XBee
  xbee.begin( 115200 );
}  

void sleepNow() {    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here 
    noInterrupts();
    sleep_enable();          // enables the sleep bit in the mcucr register 
    attachInterrupt(0, wakeUpNow, RISING); //attachInterrupt(0, wakeUpNow, RISING); // use interrupt 0 (pin 2) and run function
    EIFR = bit (INTF0); // ilman tätä menee interruptit pieleen, herää samantein ku menee nukkuun // clear any outstanding interrupt 0

    // Brown-Out Detection pois
    MCUCR = bit (BODS) | bit (BODSE);
    // The BODS bit is automatically cleared after three clock cycles
    MCUCR = bit (BODS); 
    
    interrupts();
    sleep_cpu();      
} 

void loop() {

  byte triesBeforeSleep = 5; byte i = 0;
  Serial.println("Otetaan kuva");
  xbee.write( "aloita" );
  //delay(500);
  delay(10);
  Camera();
  //delay(500);
  delay(10);
  xbee.write( "lopeta" );
  Serial.println("Kuva otettu"); 
  
  while((digitalRead(wakePin)) || (++i < triesBeforeSleep))
  {
    if (digitalRead(wakePin)) { i = 0;}
    //wakeVal = digitalRead(wakePin);
    //disSleep();
    //Serial.print("a");
    //Serial.print("while looppi ");
    Serial.println(digitalRead(wakePin));
    delay(500);
  }
  delay(500);
  Serial.println(digitalRead(wakePin));Serial.println("Nukkuun"); 
  delay(20);
  Serial.flush();
  sleepNow();     // sleep function called here 
}
