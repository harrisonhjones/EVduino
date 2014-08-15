#include <SoftwareSerial.h>

#include "TinyGPS.h"

TinyGPS gps;

int unoRxPin = 6; // connected to Tx pin of the GPS
int unoTxPin = 7; // connected to Rx pin of the GPS
SoftwareSerial ss(unoRxPin, unoTxPin);

long startMillis;
long secondsToFirstLocation = 0;

void setup()
{
  // Start the software serial to communicate with the GPS
  ss.begin(9600);
  
  // start hard serial port to communicate with the computer
  Serial.begin(9600);
  
  startMillis = millis();
  
  Serial.println("Searching for GPS lock...");
}

void loop()
{
  bool newData = false;
  unsigned long chars = 0;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      int c = ss.read();
      ++chars;
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    // we have a location fix so output the lat / long and time to acquire
    if(secondsToFirstLocation == 0){
      secondsToFirstLocation = (millis() - startMillis) / 1000;
    }
    
    Serial.println("Got new GPS Data");
    
    float flat, flon, fmph;
    unsigned long age, date, time;
    
    gps.f_get_position(&flat, &flon, &age);
    fmph = gps.f_speed_mph(); // speed in miles/hr
    gps.get_datetime(&date, &time, &age);
    
    // Debug latitute
    Serial.print("\tLat=");
    Serial.println(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);

    // Debug longitude
    Serial.print("\tLong=");
    Serial.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    
    
    // Debug speed
    Serial.print("\tSpeed=");
    Serial.print(fmph,6);
    Serial.println(" mph");
    
    Serial.print("Date(ddmmyy): "); Serial.print(date); Serial.print(" Time(hhmmsscc): ");
    Serial.print(time);
    
    Serial.println();
    // Debug Aquire Time
    /*Serial.print("Acquire Time=");
    Serial.print(secondsToFirstLocation);
    Serial.println("s");*/
  }
  
  if (chars == 0){
    // if you haven't got any chars then likely a wiring issue
    Serial.println("[ERROR] No GPS: check wiring");
  }
  else if(secondsToFirstLocation == 0){
    // if you have received some chars but not yet got a fix then indicate still searching and elapsed time
    long seconds = (millis() - startMillis) / 1000;
    
    Serial.print("Elapsed time:");
    Serial.print(seconds);
    Serial.println("s");
  }
}
