#include <Adafruit_VC0706.h>// Kamera

SoftwareSerial cameraconnection = SoftwareSerial(3, 4);   // RX ja TX kameralle

Adafruit_VC0706 cam = Adafruit_VC0706(&cameraconnection);

void Camera() 
{
  cam.begin();    // Käynnistetään kamera
  delay(500);
  // Haluttu kuvakoko
  //cam.setImageSize(VC0706_640x480);        // biggest
  //cam.setImageSize(VC0706_320x240);        // medium
  cam.setImageSize(VC0706_160x120);          // small

  // Pieni viive jotta käynnistys on tapahtunut
  delay(1000);

  cam.takePicture();  // Otetaan Kuva

    do {
    delay(10); 
  } while (Serial.available() && Serial.read() >= 0);

  // Get the size of the image (frame) taken  
  uint16_t jpglen = cam.frameLength();
  pinMode(8, OUTPUT);
  // Read all the data up to # bytes!

  while (jpglen > 0) {
    uint8_t *buffer;
    uint8_t bytesToRead = min(64, jpglen); // jos 64 jumittaa niin nopeudeks 32
    buffer = cam.readPicture(bytesToRead);
    xbee.write(buffer,bytesToRead);
    jpglen -= bytesToRead;
  }
}
