#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;    

File myFile;

int sdCardFileSetup(String filename, String defaultText)
{
  Serial.print("INFO:Checking if '");
  Serial.print(filename);
  Serial.print("' exists: ");

  if (SD.exists(filename)) {
      Serial.println("YES");
      return 0;
    }
    else {
      Serial.println("NO");

      Serial.print("INFO:Creating'");
      Serial.print(filename);
      Serial.print("' and writing default text '");
      Serial.print(defaultText);
      Serial.println("'");

      myFile = SD.open(filename, FILE_WRITE);
      if(myFile)
      {
        myFile.print("Default Welcome Text");
        myFile.close();
        Serial.println("INFO: Default text write success")
        return 0;
      }
      else
      {
        Serial.print("ERROR:Unable to open '");
        Serial.print(filename);
        Serial.println("' for writing");
        return 1;
      }
    }

}

void showSDCardFileContents(String filename)
{
  // open the welcome file for reading
  myFile = SD.open(filename, FILE_READ);
  
  if (myFile) {
    Serial.println("INFO:Welcome Text: ");
    // push the contents of the file out the serial port
    while (myFile.available()) {
        Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
        Serial.print("ERROR:Unable to open '");
        Serial.print(filename);
        Serial.println("' for reading");
  };
}
int sdCardSetup()
{
  Serial.print("INFO:Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("ERROR:SDCARD_FAILED:initialization failed.");
    return 1;
  } else {
    Serial.println("INFO:Wiring is correct and a SD card is present.");
    pinMode(10, OUTPUT);
    
    sdCardFileSetup("welcome.txt","Default Welcome Text");
    sdCardFileSetup("odo.txt","0");
    sdCardFileSetup("odo.his.txt","");
    sdCardFileSetup("trip1.txt","0");
    sdCardFileSetup("trip2.txt","0");
    sdCardFileSetup("readme.txt","Readme text overridden by error");

    showSDCardFileContents("welcome.txt");
    

    



    return 0;
  }
}

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  if(sdCardSetup()==0)  //No errors setting up the SD card
  {
    Serial.println("INFO:SD Card is setup and files are present")
  }
  else
  {
    Serial.println("ERROR:SDCARD_BADFILEINIT:There was an error setting up the files on the SD Card")
  }


  

  
}


void loop(void) {
  
}
