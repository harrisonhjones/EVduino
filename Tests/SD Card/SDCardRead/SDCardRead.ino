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


  Serial.print("\nInitializing SD card...");
  pinMode(10, OUTPUT);     // change this to 53 on a mega

  // open the welcome file for reading
  myFile = SD.open("welcome.txt");
  
  if (myFile) {
    Serial.println("Welcome Text: ");
    // push the contents of the file out the serial port
    while (myFile.available()) {
        Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    Serial.println("error opening welcome.txt");
  };
}


void loop(void) {
  
}
