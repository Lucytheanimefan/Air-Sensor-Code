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

void setup() {

  Serial.begin(9600);   //set the serial's Baudrate of the air detector module
  Serial1.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available() > 0) {
    testvalue = Serial1.read();
    if (testvalue == 66 && !(cnt == 0)) {
      cnt = 0;
    }
    //    if (testvalue == 66 && cnt)
    cnt = cnt + 1;

            Serial.print(testvalue, HEX);
            Serial.print(" ");

}
}
