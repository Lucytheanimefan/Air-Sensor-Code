#include <SHT1x.h>

#include <SPI.h>

#include <SD.h>
#include <SD_t3.h>

#include <RTClib.h>
#include <Wire.h>

//Read and print serial data from Plantower PMS3003
const char* buffer = "LucyTest.txt";
uint16_t TPM01Value = 0;        //define PM1.0 variable
uint16_t TPM2_5Value = 0;       //define PM2.5 variable
uint16_t TPM10Value = 0;       //define PM10 variable
uint16_t PM01Value = 0;        //define PM1.0 variable
uint16_t PM2_5Value = 0;       //define PM2.5 variable
uint16_t PM10Value = 0;       //define PM10 variable

uint8_t receiveDat[24];       //define array to receive data from PMS3003
uint8_t testvalue;
int cnt = 0;
int passes = 1;

File myFile;
String dataString="";

RTC_DS3231 RTC;

//for the temp/rh sensor (sht1x)
float tempCsum;
float tempFsum;
float humiditySum;

float tempC = 0;
float tempF = 0;
float humidity = 0;
int numBadReadings = 0;
const int dataPin = 4;
const int clockPin = 3;
SHT1x sht1x(dataPin, clockPin); //(Data, SCK)

void setup() {

  Serial.begin(9600);   //set the serial's Baudrate of the air detector module
  Serial1.begin(9600);

  //initialize SD card
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  //--------RTC SETUP ------------
  //Need to add RTC checks
  Serial.print("Initializing RTC Chronodot...");
  Wire.begin();
  RTC.begin();
  Serial.print("Doing RTC checks...");
  //Check if RTC is running
  
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //ONLY UNCOMMENT BELOW IF TIME NOT ALREADY SET!!!
    //RTC.adjust(DateTime(__DATE__, __TIME__));
  
  Serial.print("Setting up RTC now...");
  DateTime now = RTC.now();
  //DateTime compiled = DateTime(__DATE__, __TIME__);
//  if (now.unixtime() < compiled.unixtime()) {
//    Serial.println("RTC is older than compile time!  Updating");
//    //ONLY UNCOMMENT BELOW IF TIME NOT ALREADY SET!!!
//    //RTC.adjust(DateTime(__DATE__, __TIME__));
//  }
// // char datastr[100];
//  //RTC.getControlRegisterData( datastr[0]  );
//  Serial.println("done");



  now = RTC.now();
}


void loop()
{
  if (Serial1.available() > 0) {
    testvalue = Serial1.read();
//    if (testvalue == 66 && !(cnt == 0)) {
//      cnt = 0;
//    }
//    //    if (testvalue == 66 && cnt)
    cnt = cnt + 1;
//
            Serial.print(testvalue, HEX);
            Serial.print(" ");
    receiveDat[cnt] = testvalue;   //receive 1 of 24 bytes from air detector module
//    if(receiveDat[1] != 66 && receiveDat[2] != 77){
//      cnt = 0;
//    }
    if (cnt == 24)
    {
      String firstDataString="";
      String temphumString="";
      DateTime now = RTC.now();
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(' ');
      Serial.print(now.hour(), DEC);;
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.print(" ");

      firstDataString = String(now.year(), DEC);
      firstDataString += "/";
      firstDataString += String(now.month(), DEC);
      firstDataString += "/";
      firstDataString += String(now.day(), DEC);
      firstDataString += " ";
      firstDataString += String(now.hour(), DEC);
      firstDataString += ":";
      firstDataString += String(now.minute(), DEC);
      firstDataString += ":";
      firstDataString += String(now.second(), DEC);
      firstDataString += " ";

      myFile=SD.open(buffer, FILE_WRITE);
      String firstStr = "New Logging Session..." + firstDataString;
      sdLog(buffer, firstStr);

      //temperature and humidity
      tempC = sht1x.readTemperatureC();
      tempCsum += tempC;
      tempF = sht1x.readTemperatureF();
      tempFsum += tempF;
      humidity = sht1x.readHumidity();
      //      if (humidity < 0 || humidity > 100) {
      //        humidity = 0;
      //        numBadReadings++;
      //      }
      humiditySum += humidity;

      temphumString=String(humiditySum)+";"+String(tempCsum);
      sdLog(buffer, temphumString); 
      
      Serial.print(" Temp = ");
      //      Serial.print(tempF);
      //      Serial.print(" ");
      //      Serial.print("F, ");
      Serial.print(tempC);
      Serial.print(" ");
      Serial.print("C");
      Serial.print(" Humidity = ");
      Serial.print(humidity);
      Serial.print(" ");
      Serial.print("%");

      cnt = 0;
      Serial.println("");
      TPM01Value = receiveDat[5] * 256 + receiveDat[6]; //calculate "TSI" PM1.0 concentration
      TPM2_5Value = receiveDat[7] * 256 + receiveDat[8]; //calculate "TSI" PM2.5 concentration
      TPM10Value = receiveDat[9] * 256 + receiveDat[10]; //calculate "TSI" PM10 concentration
      Serial.print("{TSI 01 = ");
      Serial.print(TPM01Value);
      Serial.print(", TSI 2.5 = ");
      Serial.print(TPM2_5Value);
      Serial.print(", TSI 10 = ");
      Serial.print(TPM10Value);
      PM01Value = receiveDat[11] * 256 + receiveDat[12]; //calculate PM1.0 concentration
      PM2_5Value = receiveDat[13] * 256 + receiveDat[14]; //calculate PM2.5 concentration
      PM10Value = receiveDat[15] * 256 + receiveDat[16]; //calculate PM10 concentration
      Serial.print(", PM01 Value = ");
      Serial.print(PM01Value);
      Serial.print(", PM2.5 Value = ");
      Serial.print(PM2_5Value);
      Serial.print(", PM10 Value = ");
      Serial.print(PM10Value);
      Serial.println("");
      delay(500);
      dataString += String(TPM01Value) + ", " + String(TPM2_5Value) + ", " + String(TPM10Value);
      dataString += ", " + String(PM01Value) + ", " + String(PM2_5Value) + ", " + String(PM10Value);
      //log data
      sdLog(buffer, dataString);

      tempCsum = 0;
      tempFsum = 0;
      humiditySum = 0;
      dataString = "";
      temphumString="";

    
    }
  }

}

void sdLog(const char* fileName, String stringToWrite) {
  File myFile = SD.open(fileName, FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to");
    Serial.print(fileName);
    Serial.print("...");
    myFile.println(stringToWrite);
    // close the file:
    myFile.close();
    Serial.println("done.");
    //digitalWrite(redPin, LOW);
  } else {
    // if the file didn't open, print an error:
    Serial.print("error opening ");
    Serial.println(fileName);
    //turn on redLED
    //this means that the SD card was in/powered when the program started
    //but it got disconnected somehow
    //digitalWrite(redPin, HIGH);
  }
}
