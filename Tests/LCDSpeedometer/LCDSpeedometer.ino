#include <SoftwareSerial.h>

#include "TinyGPS.h"

TinyGPS gps;

long startMillis;
long secondsToFirstLocation = 0;

SoftwareSerial lcd(4, 5);

char speedString[16];

int GPS_PWR_PIN = 6;
int GPS_LED1_PIN = 7;
int speed = 0;
boolean lastLCDWasSpeed = false;
/*
void loop()
{  
  clearLCD();
  selectLineOne();
  randNumber = random(100);
  sprintf(speedString, "Speed: %02imph", randNumber); 
  
  lcd.print(speedString);
  delay(250);
}*/

void setLCDCursor(byte cursor_position)
{
  lcd.write(0xFE);  // send the special command
  lcd.write(0x80);  // send the set cursor command
  lcd.write(cursor_position);  // send the cursor position
}

void clearLCD(){
   lcd.write(0xFE);  // send the special command
   lcd.write(0x01);  // send the clear screen command
   delay(10);
}
void setBacklight(byte brightness)
{
  lcd.write(0x80);  // send the backlight command
  lcd.write(brightness);  // send the brightness value
}
void selectLineOne()
{
  setLCDCursor(0);  
}

void selectLineTwo()
{
  setLCDCursor(16);  
}

void setup()
{
  // Start the software serial to communicate with the GPS
  Serial1.begin(4800);
  
  // start hard serial port to communicate with the computer
  Serial.begin(9600);
  
  // Serial.begin(9600);  // start serial monitor serial
  lcd.begin(9600);  // start lcd serial
  clearLCD();
  selectLineOne();
  
  for (int i=0; i<3; i++)
  {
    setBacklight(0);
    delay(250);
    setBacklight(255);
    delay(250);
  }
  randomSeed(analogRead(0));
  
  lcd.println("Startup Complete");
  delay(1000);
  clearLCD();
  selectLineOne();
  
  startMillis = millis();
  pinMode(GPS_LED1_PIN, OUTPUT);     
  pinMode(GPS_PWR_PIN, OUTPUT);
 
  // Set to LOW to enable GPS. Set to HIGH to disable 
  digitalWrite(GPS_PWR_PIN, LOW);
  lcd.println("Searching for GPS lock...");
}

void loop()
{
  bool newData = false;
  unsigned long chars = 0;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial1.available())
    {
      int c = Serial1.read();
      ++chars;
      // Serial.write(c);
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
    
    if(!lastLCDWasSpeed)
      clearLCD();
    selectLineOne();
    speed = (int) fmph;
    if (speed < 0)
      speed = 00;
    if (speed > 50)
      speed = 99;
    sprintf(speedString, "Speed: %02imph", speed); 
    lcd.print(speedString);
    lastLCDWasSpeed = true;
    
    // Debug speed
    Serial.print("\tSpeed=");
    Serial.print(speed);
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
    lcd.println("[ERROR] No GPS: check wiring");
  }
  else if(secondsToFirstLocation == 0){
    // if you have received some chars but not yet got a fix then indicate still searching and elapsed time
    long seconds = (millis() - startMillis) / 1000;
    
    Serial.print("Elapsed time:");
    Serial.print(seconds);
    Serial.println("s");
  }
}
