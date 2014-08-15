#include <SPI.h>
#include <SD.h>

#define SHOW_DEBUG
#define SHOW_INFO
#define SHOW_WARNING

#ifdef SHOW_DEBUG
  #define DEBUG(x) Serial.print(x)
  #define DEBUG_DEC(x) Serial.print(x, DEC)
  #define DEBUGLN(x) Serial.println(x)
#else
  #define DEBUG(x)
  #define DEBUG_DEC(x)
  #define DEBUGLN(x)
#endif

#ifdef SHOW_INFO
  #define INFO(x) Serial.print(x)
  #define INFO_DEC(x) Serial.print(x, DEC)
  #define INFOLN(x) Serial.println(x)
#else
  #define INFO(x)
  #define INFO_DEC(x)
  #define INFOLN(x)
#endif

#ifdef SHOW_WARNING
  #define WARN(x) Serial.print(x)
  #define WARN_DEC(x) Serial.print(x, DEC)
  #define WARNLN(x) Serial.println(x)
#else
  #define WARN(x)
  #define WARN_DEC(x)
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
#define DEFAULT_TRIP_VALUE 0

#define TRIP_ODOMETER 0
#define TRIP_1 1
#define TRIP_2 2
#define MAX_TRIP_COUNT 3

long tripDistance[MAX_TRIP_COUNT];

char tempCharString[100];
char tempChar;

File myFile;

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
        return -1;
  };
}

int writeSDCard(char *filename, String txtToWrite, boolean append = true)
{
  DEBUG("DEBUG:Opening '");
  DEBUG(filename);
  DEBUGLN("' for writing");
        
  if (SD.exists(filename)) {
    if(append == false)
    {
      SD.remove(filename);
    }
      myFile = SD.open(filename, FILE_WRITE);
      if(myFile)
      {
        myFile.print(txtToWrite);
        myFile.close();
        DEBUG("DEBUG: Text '");
        DEBUG(txtToWrite);
        DEBUGLN("' written successfully");
        return 0;
      }
      else
      {
        WARN("ERROR:Unable to open '");
        WARN(filename);
        WARNLN("' for writing");
        return -1;
      }
    }
    else {
      WARN("ERROR:The file '");
      WARN(filename);
      WARNLN("' does not exist");
      return -1;
    }
}

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

      if(writeSDCard(filename, defaultText, false))
      {
        DEBUGLN("DEBUG: Default text write success");
        return 0;
      }
      else
      {
        WARN("ERROR:Unable to write default text to '");
        WARN(filename);
        WARNLN("'");
        return -1;
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

int getTripValue(int tripNumber)
{
  DEBUG("DEBUG:Getting value for trip '");
  DEBUG_DEC(tripNumber);
  DEBUGLN("'");
  if((tripNumber >= MAX_TRIP_COUNT) || (tripNumber < 0))
  {
    WARNLN("ERROR:Trip number out of bounds'");
    return -1;
  }
  return tripDistance[tripNumber];
}

int setTripValue(int tripNumber, long tripValue)
{
  DEBUG("DEBUG:Setting value for trip '");
  DEBUG_DEC(tripNumber);
  DEBUGLN("'");
  if((tripNumber >= MAX_TRIP_COUNT) || (tripNumber < 0))
  {
    WARNLN("ERROR:Trip number out of bounds'");
    return -1;
  }
  tripDistance[tripNumber] = tripValue;
  return 0;
}

int saveTripValue(int tripNumber)
{
  DEBUG("DEBUG:Saving value for trip '");
  DEBUG_DEC(tripNumber);
  DEBUGLN("'");
  if((tripNumber >= MAX_TRIP_COUNT) || (tripNumber < 0))
  {
    WARNLN("ERROR:Trip number out of bounds'");
    return -1;
  }
  
  char* filename;
  if (tripNumber == TRIP_ODOMETER)
  {
    filename = "odo.txt";
  }
  else if (tripNumber == TRIP_1)
  {
    filename = "trip1.txt";
  }
  else if (tripNumber == TRIP_2)
  {
    filename = "trip2.txt";
  }
  else
  {
    WARN("ERROR:Unknown trip number'");
    WARN_DEC(tripNumber);
    WARNLN("'");
    return -1;
  }
  
  long tripValue = getTripValue(tripNumber);
  if(tripValue < 0)
    tripValue = 0;
    
  writeSDCard(filename, String(tripValue), false);
  setTripValue(tripNumber, tripValue);
  return 0;
}
  
int loadTripValue(int tripNumber)
{
  DEBUG("DEBUG:Loading trip '");
  DEBUG_DEC(tripNumber);
  DEBUGLN("' value");
  
  char* filename;
  if (tripNumber == TRIP_ODOMETER)
  {
    filename = "odo.txt";
  }
  else if (tripNumber == TRIP_1)
  {
    filename = "trip1.txt";
  }
  else if (tripNumber == TRIP_2)
  {
    filename = "trip2.txt";
  }
  else
  {
    WARN("ERROR:Unknown trip number '");
    WARN_DEC(tripNumber);
    WARNLN("'");
    return -1;
  }
  
  if (getFileContents(filename) == 0) {
    tripDistance[tripNumber] = atol(tempCharString);
    if(tripDistance[tripNumber] == 0)
      WARNLN("WARN:Trip load returned '0'");
    else
    {
      DEBUG("DEBUG:Trip '");
      DEBUG_DEC(tripNumber);
      DEBUG(" set to '");
      DEBUG_DEC(tripDistance[tripNumber]);
      DEBUGLN("'");
    }
    return 0;
  }
  else
  {
    WARN("ERROR:Unable to load trip value. Loading with default value '");
    WARN_DEC(DEFAULT_TRIP_VALUE);
    WARN("'");
    tripDistance[tripNumber] = DEFAULT_TRIP_VALUE;
    return -1;
  }
}

int sdCardSetup()
{
  DEBUGLN("DEBUG:Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    WARNLN("ERROR:SDCARD_FAILED:initialization failed.");
    return -1;
  } else {
    DEBUGLN("DEBUG:Wiring is correct and a SD card is present.");
    pinMode(10, OUTPUT);
    
    sdCardFileSetup("welcome.txt","Default Welcome Text");
    sdCardFileSetup("odo.txt","0");
    sdCardFileSetup("odo-h.txt","");
    sdCardFileSetup("trip1.txt","0");
    sdCardFileSetup("trip2.txt","0");
    sdCardFileSetup("readme.txt","Readme text overridden by error");
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
    
    loadWelcomeText();
    INFO("INFO:Welcome Text{");
    INFO(welcomeText);
    INFOLN("}");
    
    loadTripValue(0);
    INFO("INFO:Odometer '");
    INFO_DEC(getTripValue(0));
    INFOLN("' miles");
    
    /*loadTripValue(1);
    INFO("INFO:Trip 1 '");
    INFO_DEC(getTripValue(1));
    INFOLN("' miles");*/
    
    /*loadTripValue(2);
    INFO("INFO:Trip 2 '");
    INFO_DEC(getTripValue(2));
    INFOLN("' miles");*/
    
    //loadTripValue(3);
    
    INFOLN("INFO:Increasing ODO by 5 miles");
    setTripValue(0, getTripValue(0) + 5);
    INFO("INFO:Odometer '");
    INFO_DEC(getTripValue(0));
    INFOLN("' miles");
    
    INFOLN("INFO:Saving ODO to file");
    saveTripValue(0);
     
    INFO("INFO:Odometer '");
    loadTripValue(0);
    INFO_DEC(getTripValue(0));
    INFOLN("' miles");
  }
  else
  {
    WARNLN("ERROR:SDCARD_BADFILEINIT:There was an error setting up the files on the SD Card");
  }


  

  
}


void loop(void) {
  
}
