// Ei käytössä täältä mitään atm

/*
const int XBee_wake = 9; // Arduino pin used to sleep the XBee

void writeXBee(char text[]) {
  pinMode(5, INPUT);
  pinMode(6, OUTPUT);
  
  //pinMode(XBee_wake, OUTPUT);   // wake up the XBee
  //digitalWrite(XBee_wake, LOW);
  
  xbee.write( text );
  Serial.println( text );

  // put the XBee to sleep
  //pinMode(XBee_wake, INPUT); // put pin in a high impedence state
  //digitalWrite(XBee_wake, HIGH);

}*/
/*
void sendImageToXBee(char fileName[]) {
//  file.open("fileName");
//  if (file) 
{
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
        byte clientBuf[64];
    int clientCount = 0;
    while(file.available())
    {
      clientBuf[clientCount] = file.read();
      clientCount++;

      if(clientCount > 63)
      {
        // Serial.println("Packet");
        xbee.write(clientBuf,64);
        clientCount = 0;
        Serial.print("laheta");
      }
    }
    //final <64 byte cleanup packet
    if(clientCount > 0) xbee.write(clientBuf,clientCount);            
    // close the file:
    file.close();
  }
}*/

