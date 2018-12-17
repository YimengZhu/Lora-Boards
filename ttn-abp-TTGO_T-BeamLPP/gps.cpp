#include "gps.h"

HardwareSerial GPSSerial(1);

void gps::init()
{
  GPSSerial.begin(9600, SERIAL_8N1, GPS_TX, GPS_RX);
  GPSSerial.setTimeout(2);
}

void gps::encode()
{       
    int data;
    int previousMillis = millis();

    while((previousMillis + 1000) > millis())
    {
        while (GPSSerial.available() )
        {
            char data = GPSSerial.read();
            tGps.encode(data);
            //Serial.print(data);
        }
    }
     //Serial.println("");
}

void gps::buildPacket(uint8_t txBuffer[9])
{
  LatitudeBinary = tGps.location.lat()* 10000;
  LongitudeBinary = tGps.location.lng()* 10000;
  altitudeGps = tGps.altitude.meters()* 100;
     
  sprintf(t, "Lat: %f", tGps.location.lat());
  Serial.println(t);
  
  sprintf(t, "Lng: %f", tGps.location.lng());
  Serial.println(t);

  txBuffer[0] = 0x01;
  txBuffer[1] = 0x88;// cayenneLPP
  
  txBuffer[2] = ( LatitudeBinary >> 16 ) & 0xFF;
  txBuffer[3] = ( LatitudeBinary >> 8 ) & 0xFF;
  txBuffer[4] = LatitudeBinary & 0xFF;

  txBuffer[5] = ( LongitudeBinary >> 16 ) & 0xFF;
  txBuffer[6] = ( LongitudeBinary >> 8 ) & 0xFF;
  txBuffer[7] = LongitudeBinary & 0xFF;

  txBuffer[8] = ( altitudeGps >> 16 ) & 0xFF;
  txBuffer[9] = ( altitudeGps >> 8 ) & 0xFF;
  txBuffer[10] = altitudeGps & 0xFF;

}

bool gps::checkGpsFix()
{
  encode();
  if (tGps.location.isValid() && 
      tGps.location.age() < 2000 &&
      tGps.hdop.isValid() &&
      tGps.hdop.value() <= 300 &&
      tGps.hdop.age() < 2000 &&
      tGps.altitude.isValid() && 
      tGps.altitude.age() < 2000 )
  {
    Serial.println("Valid gps Fix.");
    return true;
  }
  else
  {
     Serial.println("No gps Fix.");

    return false;
  }
}
