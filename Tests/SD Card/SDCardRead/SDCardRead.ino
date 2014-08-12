#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;    

File myFile;

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    return;
  } else {
   Serial.println("Wiring is correct and a card is present."); 
  }

  Serial.print("\nInitializing SD card...");
  pinMode(10, OUTPUT);     // change this to 53 on a mega
  
  Serial.print("Checking if welcome.txt exists: ");
  if (SD.exists("WELCOME.TXT")) {
    Serial.println("YES");
  }
  else {
    Serial.println("NO");
    // open a new file and immediately close it:
    Serial.println("Creating welcome.txt...");
    myFile = SD.open("welcome.txt", FILE_WRITE);
    if(myFile)
    {
      myFile.print("Default Welcome Text");
      myFile.close();
    }
    else
    {
      Serial.println("Error Opening welcome.txt for writing");
    }
  }

  // open the welcome file for reading
  myFile = SD.open("welcome.txt", FILE_READ);
  
  if (myFile) {
    Serial.println("Welcome Text: ");
    // push the contents of the file out the serial port
    while (myFile.available()) {
        Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    Serial.println("Error Opening welcome.txt for reading");
  };
}


void loop(void) {
  
}
