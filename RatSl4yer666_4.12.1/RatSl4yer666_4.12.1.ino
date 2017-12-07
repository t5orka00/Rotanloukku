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
const int wakeXBeePin = 7;

int wakeVal = 0;
int wakeState = LOW;

void wakeUpNow() {          // Interrupt
  //
  sleep_disable();          // Poistutaan lepotilasta
  detachInterrupt(0);       // Interrupt väliaikaisesti pois käytöstä
xbee.begin( 115200 );
  pinMode(wakeXBeePin, OUTPUT);   // wake up the XBee
  digitalWrite(wakeXBeePin, LOW); // 10k vastus väliin?
  
  pinMode(9, OUTPUT);   // Kamera
  digitalWrite(9, HIGH); // 10k vastus väliin?

    pinMode(3, INPUT);        // Kamera
  pinMode(4, OUTPUT);
  //stopPowersave();
  //pinMode(5, INPUT);        // XBee:n RX ja TX, varmuuden vuoksi erikseen määritellään täällä
  //pinMode(6, OUTPUT); 

  // Interrupt
  //pinMode(wakePin, INPUT);  
/*
  power_adc_enable();
  power_spi_enable();
  power_timer0_enable();
  power_timer1_enable();
  power_timer2_enable();
  power_twi_enable();*/
} 

void setup() {
  Serial.begin(9600);

  pinMode(5, INPUT);        // XBee:n RX ja TX, varmuuden vuoksi erikseen määritellään täällä
  pinMode(6, OUTPUT); 
  
  pinMode(3, INPUT);        // Kamera RX ja TX
  pinMode(4, OUTPUT);
  
  pinMode(9, OUTPUT);       // Kameran mosfet
  digitalWrite(9, HIGH);    // Mosfettiin virrat
  
  pinMode(wakePin, INPUT);  // Interrupt eli PIR
  
  PCIFR = bit (PCIF1);
  //ADCSRA = 0x00;  // ADC pois

  //GIMSK &= ~_BV(INT0); // disable hardware INT0
  //MCUCR &= ~(_BV(ISC01) | _BV(ISC00)); // set hardware INT0 active on low LEVEL

  // XBee
  xbee.begin( 115200 );
  
  pinMode(wakeXBeePin, OUTPUT);   // XBeen herätyspinni
  digitalWrite(wakeXBeePin, LOW);
  Serial.println("Setup");
  sleepNow();
}  

void sleepNow() {    
  // Virran kulutus Arduino 26.3uA
  // Virran kulutus Arduino ja PIR 145.6uA
  noInterrupts();  
  
  pinMode(wakeXBeePin, OUTPUT); // put pin in a high impedence state
  digitalWrite(wakeXBeePin, HIGH);
  
  pinMode(9, OUTPUT);   // Kamera
  digitalWrite(9, LOW); // 10k vastus väliin?

  pinMode(3, INPUT);        // Kamera
  digitalWrite(3, LOW);
  pinMode(4, INPUT);
  digitalWrite(4, LOW);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here 
  sleep_enable();          // enables the sleep bit in the mcucr register 
  attachInterrupt(0, wakeUpNow, RISING); //attachInterrupt(0, wakeUpNow, RISING); // use interrupt 0 (pin 2) and run function


  //setPowersave();
  // for (byte i = 0; i <= A5; i++)
  //  {
  //   pinMode (i, OUTPUT);    // changed as per below
  //   digitalWrite (i, LOW);  //     ditto
  // }
  
  ADCSRA = 0; 

  /*power_adc_disable();
  power_spi_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable();*/
  /*
  byte i;
  for (i = 0; i <= 13; i++)
    pinMode (i, OUTPUT);     // as required
  for (i = 0; i <= 13; i++)
    digitalWrite (i, LOW);  // as required
  */  
  EIFR = bit (INTF0); // ilman tätä menee interruptit pieleen, herää samantein ku menee nukkuun // clear any outstanding interrupt 0
    // Brown-Out Detection pois
  MCUCR = bit (BODS) | bit (BODSE);
  MCUCR = bit (BODS); // The BODS bit is automatically cleared after three clock cycles, eli tän jälkeen ei parane tekeen mitään ylimäärästä
  interrupts();
  sleep_cpu();      
} 

void loop() {
  Serial.println("herasi");
  byte triesBeforeSleep = 5; byte i = 0; // kauanko sensorin LOW tilaan menemisen jälkeen odotetaan ennen lepotilaan siirtymistä
  delay(1000); // XBee kerkeää herätä

  byte reso = Reso();

  delay(50);
  xbee.write( "aloita" ); // Gateway tietää milloin kuvaa alkaa tulemaan
  //delay(500);
  delay(40);
  Camera(reso);               // Otetaan kuva
  //delay(500);
  delay(40);
  xbee.write( "lopeta" ); // Gateway tietää milloin kuva on lähetetty
  Serial.println("Kuva otettu"); 
  
  while((digitalRead(wakePin)) || (++i < triesBeforeSleep))
  {
    if (digitalRead(wakePin)) { i = 0;}
    Serial.print("Sensorin tila:");
    Serial.println(digitalRead(wakePin));
    delay(500);
  }
  delay(500);
  Serial.print("Sensorin tila:");Serial.println(digitalRead(wakePin));Serial.println("Nukkuun"); 
  delay(20);
  Serial.flush();
  //asm volatile ("  jmp 0");
  sleepNow();     // sleep function called here 
}

byte Reso() {
  xbee.write( "reso" ); // Reso
  unsigned long luotettavaAika = millis();
  Serial.println(millis());
  byte readeded[9];
  byte count = 0;
  byte reso = 1;
  while (millis() - luotettavaAika < 2000) {
    Serial.println(xbee.read());
    Serial.println("tanne paas");
    if (xbee.available()) {
      for(int i = 0; i < 9;i++) {
        readeded[i] = xbee.read();
        Serial.print(readeded[i], HEX);
        //Serial.print(readeded[i]);
        Serial.print(",");
      }
      Serial.println("tanne paas");
      for(int i = 0; i < 9;i++) {
        if (readeded[i] != 255) {count++;}
      }
      
      if (count < 5) {reso = 1;}
      else if (count < 8) {reso = 2;}
      else  {reso = 3;}
      Serial.println("   ");
      Serial.println(reso);
      Serial.println("   ");
      return reso;
    }
  }
}
