#include <SPI.h>
#include <SD.h>

//#define SHOW_DEBUG
#define SHOW_INFO
#define SHOW_WARNING

#ifdef SHOW_DEBUG
  #define DEBUG(x) Serial.print(x)
  #define DEBUGLN(x) Serial.println(x)
#else
  #define DEBUG(x)
  #define DEBUGLN(x)
#endif

#ifdef SHOW_INFO
  #define INFO(x) Serial.print(x)
  #define INFOLN(x) Serial.println(x)
#else
  #define INFO(x)
  #define INFOLN(x)
#endif

#ifdef SHOW_WARNING
  #define WARN(x) Serial.print(x)
  #define WARNLN(x) Serial.println(x)
#else
  #define WARN(x)
  #define WARNLN(x)
#endif

// Uno
//const int chipSelect = 10;  
// Goldilocks
const int chipSelect = 14;    

// Useful strings
#define WELCOME_TEXT_LENGTH 48

char welcomeText[WELCOME_TEXT_LENGTH];
char defaultWelcomeText = 'Welcome driver';

char tempCharString[100];
char tempChar;

File myFile;

int sdCardFileSetup(char *filename, String defaultText)
{
  DEBUG("DEBUG:Checking if '");
  DEBUG(filename);
  DEBUG("' exists: ");

  if (SD.exists(filename)) {
      DEBUGLN("YES");
      return 0;
    }
    else {
      DEBUGLN("NO");

      DEBUG("DEBUG:Creating'");
      DEBUG(filename);
      DEBUG("' and writing default text '");
      DEBUG(defaultText);
      DEBUGLN("'");

      myFile = SD.open(filename, FILE_WRITE);
      if(myFile)
      {
        myFile.print(defaultText);
        myFile.close();
        DEBUGLN("DEBUG: Default text write success");
        return 0;
      }
      else
      {
        WARN("ERROR:Unable to open '");
        WARN(filename);
        WARNLN("' for writing");
        return 1;
      }
    }

}

void showSDCardFileContents(char *filename)
{
  DEBUG("DEBUG:Showing contents of file '");
  DEBUG(filename);
  DEBUGLN("'");
  
  if (getFileContents(filename) == 0) {
    DEBUG("DEBUG:{");
    DEBUG(tempCharString);
    DEBUGLN("}");
  }
  else
  {
    WARN("ERROR:Unable to show contents of '");
    WARN(filename);
    WARNLN("'");
  }
}

void loadWelcomeText()
{
  DEBUG("DEBUG:Loading welcome text from '");
  DEBUG("welcome.txt");
  DEBUGLN("'");
  
  if (getFileContents("welcome.txt") == 0) {
    strncpy(welcomeText, tempCharString, WELCOME_TEXT_LENGTH);
    DEBUGLN("DEBUG:Welcome text loaded from file");
  }
  else
  {
    WARN("ERROR:Unable to load welcome text. Loading with default text '");
    WARN(defaultWelcomeText);
    WARNLN("'");
  }
}

int getFileContents(char *filename)
{
  int tempCounter = 0;
  // open the welcome file for reading
  myFile = SD.open(filename, FILE_READ);
  DEBUG("DEBUG:Grabbing contents of file '");
  DEBUG(filename);
  DEBUGLN("'");
  if (myFile) {
    // push the contents of the file out the serial port
    while (myFile.available()) {
      //Serial.write(myFile.read());
       tempCharString[tempCounter] = myFile.read(); // Store it
       tempCounter++; // Increment where to write next 
    }
    // close the file:
    myFile.close();
    return 0;
  } else {
        WARN("ERROR:Unable to open '");
        WARN(filename);
        WARNLN("' for reading");
        return 1;
  };
}
int sdCardSetup()
{
  DEBUGLN("DEBUG:Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    WARNLN("ERROR:SDCARD_FAILED:initialization failed.");
    return 1;
  } else {
    DEBUGLN("DEBUG:Wiring is correct and a SD card is present.");
    pinMode(10, OUTPUT);
    
    sdCardFileSetup("welcome.txt","Default Welcome Text");
    sdCardFileSetup("odo.txt","0");
    sdCardFileSetup("odo-h.txt","");
    sdCardFileSetup("trip1.txt","0");
    sdCardFileSetup("trip2.txt","0");
    sdCardFileSetup("readme.txt","Readme text overridden by error");

    showSDCardFileContents("welcome.txt");
    
    loadWelcomeText();
    INFO("INFO:Welcome Text{");
    INFO(welcomeText);
    INFOLN("}");
    
    return 0;
  }
}

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  /*while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }*/

  if(sdCardSetup()==0)  //No errors setting up the SD card
  {
    INFOLN("INFO:SD Card is setup and files are present");
  }
  else
  {
    WARNLN("ERROR:SDCARD_BADFILEINIT:There was an error setting up the files on the SD Card");
  }


  

  
}


void loop(void) {
  
}
