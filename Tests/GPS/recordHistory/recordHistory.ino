#include <SoftwareSerial.h>
#include "TinyGPS.h"
#include<stdlib.h>
#include <SPI.h>
#include <SD.h>

#define SHOW_DEBUG
#define SHOW_INFO
#define SHOW_WARNING

#ifdef SHOW_DEBUG
  #define DEBUG(x) Serial.print(x)
  #define DEBUG_DEC(x) Serial.print(x, DEC)
  #define DEBUGLN(x) Serial.println(x)
#else
  #define DEBUG(x)
  #define DEBUG_DEC(x)
  #define DEBUGLN(x)
#endif

#ifdef SHOW_INFO
  #define INFO(x) Serial.print(x)
  #define INFO_DEC(x) Serial.print(x, DEC)
  #define INFOLN(x) Serial.println(x)
#else
  #define INFO(x)
  #define INFO_DEC(x)
  #define INFOLN(x)
#endif

#ifdef SHOW_WARNING
  #define WARN(x) Serial.print(x)
  #define WARN_DEC(x) Serial.print(x, DEC)
  #define WARNLN(x) Serial.println(x)
#else
  #define WARN(x)
  #define WARN_DEC(x)
  #define WARNLN(x)
#endif

TinyGPS gps;

// Uno
//const int chipSelect = 10;  
// Goldilocks
const int chipSelect = 14;  


long startMillis;
long secondsToFirstLocation = 0;

float lastLat = 0.0;
float lastLong = 0.0;
boolean firstRead = true;

File myFile;

float haversine(float flat1, float flon1, float flat2, float flon2)
{
  Serial.println();
  Serial.println("Haverside Called");
  Serial.print("\tLat 1: ");
  Serial.println(flat1);
  Serial.print("\tLon 1: ");
  Serial.println(flon1);
  Serial.print("\tLat 2: ");
  Serial.println(flat2);
  Serial.print("\tLon 2: ");
  Serial.println(flon2);
  
  
  // Source: http://www.movable-type.co.uk/scripts/latlong.html
  
  // Formula: 
  // distance = c * r
  // R = radius of earth
  // c = 2 * atan2(sqrt(a),sqrt(1-a))
  // a = a1^2 + a2a*a2c*a2c^2
  // a1 = sin(dPhi/2)
  // a2a = cos(Phi1)
  // a2b = cos(Phi2);
  // a2c = sin^2(dPsi/2)
  // dPhi = Phi2 - Phi1
  // Phi = lat in radians
  // Psi = long in radians
  
  float a=0;
  float a1=0;
  float a2=0;
  float c=0;
  
  float dPhi = radians(flat2-flat1);
  Serial.print("dPhi: "); 
  Serial.println(dPhi);
  float Phi1 = radians(flat1);
  float Phi2 = radians(flat2);
  float dPsi = radians((flon2)-(flon1));
  Serial.print("dPsi: "); 
  Serial.println(dPsi);
  
  a1 = (sin(dPhi/2.0) * sin(dPhi/2.0));  // a1^2
  a2 = cos(Phi1); // a2a 
  a2 *= cos(Phi2); // a2a*a2b
  a2 *= sin(dPsi/2.0); // a2a*a2b*a2c
  a2 *= sin(dPsi/2.0); // a2a*a2b*a2c^2
  a = a1 + a2; // a
  
  c = (2*atan2(sqrt(a),sqrt(1.0-a)));
  
  return c * 20925525; // distance = c * r. Return distance in feet
}

int writeSDCard(char *filename, String txtToWrite, boolean append = true)
{
  DEBUG("DEBUG:Opening '");
  DEBUG(filename);
  DEBUGLN("' for writing");
        
  if (SD.exists(filename)) {
    if(append == false)
    {
      SD.remove(filename);
    }
      myFile = SD.open(filename, FILE_WRITE);
      if(myFile)
      {
        myFile.print(txtToWrite);
        myFile.close();
        DEBUG("DEBUG: Text '");
        DEBUG(txtToWrite);
        DEBUGLN("' written successfully");
        return 0;
      }
      else
      {
        WARN("ERROR:Unable to open '");
        WARN(filename);
        WARNLN("' for writing");
        return -1;
      }
    }
    else {
      WARN("ERROR:The file '");
      WARN(filename);
      WARNLN("' does not exist");
      return -1;
    }
}

int sdCardFileSetup(char *filename, String defaultText)
{
  DEBUG("DEBUG:Checking if '");
  DEBUG(filename);
  DEBUG("' exists: ");

  if (SD.exists(filename)) {
      DEBUGLN("YES");
      return 0;
    }
    else {
      DEBUGLN("NO");

      DEBUG("DEBUG:Creating'");
      DEBUG(filename);
      DEBUG("' and writing default text '");
      DEBUG(defaultText);
      DEBUGLN("'");

      if(writeSDCard(filename, defaultText, false))
      {
        DEBUGLN("DEBUG: Default text write success");
        return 0;
      }
      else
      {
        WARN("ERROR:Unable to write default text to '");
        WARN(filename);
        WARNLN("'");
        return -1;
      }
    }
}

int sdCardSetup()
{
  DEBUGLN("DEBUG:Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    WARNLN("ERROR:SDCARD_FAILED:initialization failed.");
    return -1;
  } else {
    DEBUGLN("DEBUG:Wiring is correct and a SD card is present.");
    pinMode(10, OUTPUT);
    SD.remove("history.txt");
    sdCardFileSetup("history.txt","Lat,Long,Date(ddmmyy),Time(hhmmsscc)");
    return 0;
  }
}

int led1 = 7;
int gpsPwr = 6;

void setup()
{
  // Start the software serial to communicate with the GPS
  Serial1.begin(4800);
  
  // start hard serial port to communicate with the computer
  Serial.begin(115200);
  
  pinMode(led1, OUTPUT);     
  pinMode(gpsPwr, OUTPUT); 
  digitalWrite(gpsPwr, LOW);
  
  if(sdCardSetup()==0)  //No errors setting up the SD card
  {
    INFOLN("INFO:SD Card is setup and files are present");
    
  }
  else
  {
    WARNLN("ERROR:SDCARD_BADFILEINIT:There was an error setting up the files on the SD Card");
  }
}

void loop()
{
  bool newdata = false;
  unsigned long start = millis();
  
  // Every second we print an update
  while (millis() - start < 1000)
  {
    if (feedgps())
      newdata = true;
  }
  
  gpsdump();
}

static bool feedgps()
{
  while (Serial1.available())
  {
    if (gps.encode(Serial1.read()))
      return true;
  }
  return false;
}

int saveHistoryLine(float flat, float flon, unsigned long date, unsigned long ftime)
{
  
  DEBUGLN("DEBUG:Saving history line");
  
  if(millis() > 6000)
  {
    myFile = SD.open("history.txt", FILE_WRITE);
    if(myFile)
    {
      myFile.print(flat,6);
      myFile.print(",");
      myFile.print(flon,6);
      myFile.print(",");
      myFile.print(date);
      myFile.print(",");
      myFile.print(ftime);
      myFile.print(",");
      myFile.println(millis());
      myFile.close();
      DEBUGLN("DEBUG: History line written successfully");
      return 0;
    }
    else
    {
      WARNLN("ERROR:Unable to open history.txt for writing");
      return -1;
    }
  }
  else
  {
    WARNLN("ERROR:Doing a poor man's wait for GPS lock");
  }
}

void gpsdump()
{
  digitalWrite(led1, HIGH);   // turn the LED on (HIGH is the voltage level)
  unsigned long chars = 0;
  unsigned short sentences, failed;

    
  Serial.println("Got new GPS Data");
  
  float flat, flon, fmph;
  unsigned long age, date, time;
  float distance;
  
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
  
  saveHistoryLine(flat, flon, date, time);
  
  
  
  if(firstRead)
  {
    firstRead = false;
  }
  else
  {
    distance =  haversine(lastLat, lastLong, flat, flon);
    Serial.print("Distance traveled since last point: ");
    Serial.print(distance);
    Serial.print(" feet");
  }
  lastLat = flat;
  lastLong = flon;
  
  Serial.println();
  
  digitalWrite(led1, LOW);   // turn the LED on (HIGH is the voltage level)
}
