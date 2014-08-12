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
  
  Serial.print("Checking if welcome.txt exists: ");
  if (SD.exists("WELCOME.TXT")) {
    Serial.println("YES");
  }
  else {
    Serial.println("NO");
    // open a new file and immediately close it:
    Serial.println("Creating WELCOME.TXT...");
    myFile = SD.open("WELCOME.TXT", FILE_WRITE);
    myFile.print("Default Welcome Text");
    myFile.close();
  }

  // open the welcome file for reading
  myFile = SD.open("WELCOME.TXT");
  
  if (myFile) {
    Serial.println("Welcome Text: ");
    // push the contents of the file out the serial port
    while (myFile.available()) {
        Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    Serial.println("Error Opening WELCOME.TXT");
  };
}


void loop(void) {
  
}
