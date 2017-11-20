/*
 * Board: Arduino Pro tai Arduino Pro Mini
 * Processor: ATmega328P 8MHz 3.3V - toiminee myös muutki mutta käytetään tätä
 */

#include <avr/sleep.h>      // lepotila, interrupt

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
  //attachInterrupt(0, sleepNow, LOW); // use interrupt 0 (pin 2) and run function wakeUpNow when pin 2 gets LOW  

  // XBee
  xbee.begin( 115200 );
}  

void sleepNow() {  
    //pinMode(wakePin, INPUT);  
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here  
    cli();
    sleep_enable();          // enables the sleep bit in the mcucr register  
    attachInterrupt(0,wakeUpNow, RISING); // use interrupt 0 (pin 2) and run function  
    sei();
    //sleep_mode();            // here the device is actually put to sleep!!  
    sleep_cpu (); 
} 
/*
void disSleep(){
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP  
    sleep_disable();         // first thing after waking from sleep: disable sleep...  
    detachInterrupt(0);      // disables interrupt 0 on pin 2 so the wakeUpNow code will not be executed during normal running time.  
}*/

void loop() {
  //disSleep();
  //Serial.println("KAMERA OTTAA KUVAN");
  //delay(10);
  //wakeVal = digitalRead(wakePin);
  //while(wakeVal == 1)
  //cli();
  byte triesBeforeSleep = 10; byte i = 0;
  Serial.println("Otetaan kuva");
  xbee.write( "aloita" );
  delay(500);
  Camera();
  delay(500);
  xbee.write( "lopeta" );
  delay(4000);
  
  Serial.println("Kuva otettu");
  while(digitalRead(wakePin)) //|| ++i < triesBeforeSleep)
  {
    //wakeVal = digitalRead(wakePin);
    //disSleep();
    Serial.println("while looppi");
    delay(500);
    
    
    //delay(10);
  }
  Serial.println("Menossa nukkumaan");
  //delay(500);
  //sei();
  
  sleepNow();     // sleep function called here
  delay(5000);  
  
  
  /*
  wakeVal = digitalRead(wakePin);
  if (wakeVal == HIGH)
  {  disSleep();
     if (wakeState == LOW)
     {
      Serial.println("Motion detected");
      wakeState = HIGH;
     }
  }
  else{
    
    if (wakeState == HIGH){
      delay(100);
      Serial.println("Motion stopped - Going to Sleep Mode !");
      wakeState = LOW;
      sleepNow();     // sleep function called here
    }
  */  
}
