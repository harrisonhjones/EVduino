void setDisplay(int value1, int value2)
{
  print("Speed: ");
  print(value1);
  print(value2);
  println(" mph");
}

void showSpeed(int speed)
{
  int speedDigits[] = {0,0,0};
  int speedDigitCounter = 0;
  if(speed > 999)
  {
    speed = 99;
  }
  else if(speed < 0)
  {
    speed = 0;
  }
  while (speed > 0) {
      speedDigits[speedDigitCounter++] = speed % 10;
      speed = speed / 10;
  }
  
  if(speedDigitCounter == 3)
  {
    speedDigits[1] = 9;
    speedDigits[0] = 9;
  }

  setDisplay(speedDigits[1], speedDigits[0]);
}

void setup()
{
    showSpeed(0);
  showSpeed(1000);
  showSpeed(12);
  showSpeed(24);
  showSpeed(15);
  showSpeed(115);
  showSpeed(87);
  showSpeed(0);
  showSpeed(-5);
}

void loop() {
  // put your main code here, to run repeatedly: 
  
}


