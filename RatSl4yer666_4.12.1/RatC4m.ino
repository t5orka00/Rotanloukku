#include <Adafruit_VC0706.h>// Kamera

SoftwareSerial cameraconnection = SoftwareSerial(3, 4);   // RX ja TX kameralle

Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);

void Camera(byte reso) 
{
  cam.begin();    // Käynnistetään kamera
  // Pieni viive jotta käynnistys on tapahtunut 
  //delay(300); // tässä pitää olla delay, muuten ei tuu kuvat, ei kerkeä käynnistyä, 500 toimii 200 ei toimi, 300 tuntuu pelaavan aina, testattu pienimmällä resolla
  // Haluttu kuvakoko
  //cam.setImageSize(VC0706_640x480);        // biggest
  //cam.setImageSize(VC0706_320x240);        // medium
  //cam.setImageSize(VC0706_160x120);          // small
  for (int i = 0; i <= 2; i++)
  {
    switch(reso) {
      case 1:
        cam.setImageSize(VC0706_160x120);
        Serial.print("pieni");
        break;
      case 2:
        cam.setImageSize(VC0706_320x240);
        Serial.print("keski");
        break;
      case 3:
        cam.setImageSize(VC0706_640x480);
        Serial.print("iso");
        break;
      default:
        cam.setImageSize(VC0706_160x120);
        break;
    } 
  }
  cam.reset();
  cam.begin();
  delay(1500);    // 400 vähintään - hassun keltasia kuvia tulee. 1500 - aika valkea kuva. isompi kuitenki parempi kuvan suhteen
  //delay(50); // reson vaihtoon, voi olla ettei vaihda ilman delaytä

  cam.takePicture();  // Otetaan Kuva

    do {
    delay(10); 
  } while (Serial.available() && Serial.read() >= 0);
  delay(50);
  // Get the size of the image (frame) taken  
  uint16_t jpglen = cam.frameLength();
  pinMode(8, OUTPUT);
  // Read all the data up to # bytes!

  while (jpglen > 0) {
    uint8_t *buffer;
    uint8_t bytesToRead = min(64, jpglen); // jos 64 jumittaa niin nopeudeks 32
    buffer = cam.readPicture(bytesToRead);
    xbee.write(buffer,bytesToRead);    
    //Serial.write(buffer,bytesToRead); 
    jpglen -= bytesToRead;
    delay(50);
  }
  cam.reset();
}
