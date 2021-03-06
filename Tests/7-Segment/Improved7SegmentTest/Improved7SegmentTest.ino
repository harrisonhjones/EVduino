//Pin Assignments (You should change these)
const int CLK       = 12;           //Connected to TPIC pin 13: SRCLK (aka Clock) / CLK
const int LATCH     = 11;          //Connected to TPIC pin 12: RCLK (aka Latch/load/CS/SS...) / RCK
const int OE        = 10;          //Connected to TPIC pin 9: OE (Output Enable) / G
const int DOUT      = 9;          //Connected to TPIC pin 3: SER (aka MOSI) / SER IN

const byte numTable[] =
{
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110   // 9
};

void setDisplay(int value1, int value2)
{
  Serial.print("Speed: ");
  Serial.print(value1,DEC);
  Serial.print(value2,DEC);
  Serial.println(" mph");
  
  digitalWrite(LATCH,LOW);      
  shiftOut(DOUT,CLK,LSBFIRST,numTable[value2]);
  shiftOut(DOUT,CLK,LSBFIRST,numTable[value1]);
  digitalWrite(LATCH,HIGH); 
}

int showSpeed(int speed)
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
  return 0;
}

void setup()
{
  Serial.begin(9600);

  //Set pin modes
  pinMode(CLK,OUTPUT);
  pinMode(LATCH,OUTPUT);
  pinMode(DOUT, OUTPUT);
  pinMode(OE, OUTPUT);

  //7-Segment Display Init
  digitalWrite(OE,LOW);        //Enables SR Operation
  
  // Randomize the random function by seeding it with the contents of analog read which, if left unconnected, should be noisy
  randomSeed(analogRead(0));

  // Show 00
  showSpeed(0);
  delay(500);
  
  // Test numbers > 99. Should show 99
  showSpeed(100);
  delay(500);
  
  // Test numbers < 0. Should show 00
  showSpeed(-5);
  delay(500);
}

void loop() {
  // Show speeds randomly between 0 and 99
  showSpeed(random(0,100));
  delay(500);
  
}


