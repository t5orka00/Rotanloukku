#include <avr/sleep.h>  

const int wakePin = 2;                 // pin used for waking up  

int wakeVal = 0;
int wakeState = LOW;

void wakeUpNow() {


}  

void setup() {  
  Serial.begin(9600);
  pinMode(wakePin, INPUT);  
  //attachInterrupt(0, sleepNow, LOW); // use interrupt 0 (pin 2) and run function wakeUpNow when pin 2 gets LOW  
}  

void sleepNow() {  
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here  
    sleep_enable();          // enables the sleep bit in the mcucr register  
    attachInterrupt(0,wakeUpNow, RISING); // use interrupt 0 (pin 2) and run function  
    sleep_mode();            // here the device is actually put to sleep!!  
} 

void disSleep(){
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP  
    sleep_disable();         // first thing after waking from sleep: disable sleep...  
    detachInterrupt(0);      // disables interrupt 0 on pin 2 so the wakeUpNow code will not be executed during normal running time.  
}

void loop() {
  {
  disSleep();
  //Serial.println("KAMERA OTTAA KUVAN");
  //delay(10);
  wakeVal = digitalRead(wakePin);
  while(wakeVal == 1)
  {
    wakeVal = digitalRead(wakePin);
    //disSleep();
    //delay(1000);
    //Serial.println("while looppi");
    //delay(10);
  }
  Serial.println("Menossa nukkumaan");
  sleepNow();     // sleep function called here
    
  
  
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
  */}  
}
