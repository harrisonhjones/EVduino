//Pin Assignments (You should change these)
const int CLK       = 9;           //Connected to TPIC pin 13: SRCLK (aka Clock)
const int LATCH     = 10;          //Connected to TPIC pin 12: RCLK (aka Latch/load/CS/SS...)
const int OE        = 11;          //Connected to TPIC pin 9: OE (Output Enable)
const int DOUT      = 12;          //Connected to TPIC pin 3: SER (aka MOSI)

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
