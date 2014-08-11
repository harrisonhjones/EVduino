//Pin Assignments (You should change these)
const int CLK       = 12;           //Connected to TPIC pin 13: SRCLK (aka Clock) / CLK
const int LATCH     = 11;          //Connected to TPIC pin 12: RCLK (aka Latch/load/CS/SS...) / RCK
const int OE        = 10;          //Connected to TPIC pin 9: OE (Output Enable) / G
const int DOUT      = 9;          //Connected to TPIC pin 3: SER (aka MOSI) / SER IN

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
}

void loop()
{
  for(int i=0;i<=255;i++)
  {
    setDisplay(i);
    delay(5);
  }
}

void setDisplay(int value)
{
  digitalWrite(LATCH,LOW);      
  shiftOut(DOUT,CLK,LSBFIRST,value);
  digitalWrite(LATCH,HIGH); 
}
