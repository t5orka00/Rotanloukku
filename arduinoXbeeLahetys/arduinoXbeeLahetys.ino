/*
  The circuit: 
  * RX is digital pin 2 (connect to TX of XBee)
  * TX is digital pin 3 (connect to RX of XBee)
*/

#include <SoftwareSerial.h>

SoftwareSerial xbee(2, 3); // RX, TX

char c = 'A';
const int XBee_wake = 9; // Arduino pin used to sleep the XBee


void setup()  {
   Serial.begin(9600);

   // set the data rate for the SoftwareSerial port
   xbee.begin( 9600 );
}

void loop()  {
  
  //pinMode(XBee_wake, OUTPUT);   // wake up the XBee
  //digitalWrite(XBee_wake, LOW);
  
  xbee.write( c );
  
  //--- display the character just sent on console ---
  Serial.println( c );
  
  //--- get the next letter in the alphabet, and reset to ---
  //--- 'A' once we have reached 'Z'. 
  c = c + 1;
  if ( c>'Z' ) 
       c = 'A';
  
  delay( 1000 );

  // put the XBee to sleep
  //pinMode(XBee_wake, INPUT); // put pin in a high impedence state
  //digitalWrite(XBee_wake, HIGH);

}
