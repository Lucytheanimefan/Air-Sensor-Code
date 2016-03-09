
#include <RTClib.h>
#include <Wire.h>

//Read and print serial data from Plantower PMS3003
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

RTC_DS3231 RTC;

void setup() {

  Serial.begin(9600);   //set the serial's Baudrate of the air detector module
  Serial1.begin(9600);
  
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
  DateTime compiled = DateTime(__DATE__, __TIME__);
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
     
      DateTime now = RTC.now();
      Serial.print(now.year(), DEC);
      Serial.print('/');
      Serial.print(now.month(), DEC);
      Serial.print('/');
      Serial.print(now.day(), DEC);
      Serial.print(' ');
      Serial.print(now.hour(), DEC);
      Serial.print(':');
      Serial.print(now.minute(), DEC);
      Serial.print(':');
      Serial.print(now.second(), DEC);
      Serial.print(" ");

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
    
    }
  }
//  else{
//    Serial.println("Serial1 not available");
//    delay(1000);
//  }
}
