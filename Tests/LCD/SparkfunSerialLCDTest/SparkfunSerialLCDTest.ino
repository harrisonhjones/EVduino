#include <SoftwareSerial.h>

SoftwareSerial lcd(4, 5);

long randNumber;
char speedString[16];

void setup()
{
  //Serial.begin(9600);  // start serial monitor serial
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
  
}

void loop()
{  
  clearLCD();
  selectLineOne();
  randNumber = random(100);
  sprintf(speedString, "Speed: %02imph", randNumber); 
  
  lcd.print(speedString);
  delay(250);
}

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
