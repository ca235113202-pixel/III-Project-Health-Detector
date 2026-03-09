// IOT CODING

#define BLYNK_TEMPLATE_ID "TMPL3bOlr-Ae5"
#define BLYNK_TEMPLATE_NAME "health"
#define BLYNK_AUTH_TOKEN "x6K6FhupIvezGbOBIHD36jeMUpckl7hC"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//dafok28235@bitfami.com
//Galwin@0123
#include <WiFiClientSecure.h>

#include <Wire.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
 
#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;
int temp=0;
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  
// Create an object for the MPU6050 sensor
Adafruit_MPU6050 mpu;

const char *ssid = "health";
const char *password = "123456789";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
unsigned int a=0,b=0,c=0,d=0,e=0,f=0;
unsigned int A=0,VALA=1,VALB=0;
int AVALA=0,AVALB=0,AVALC=0,AVALD=0,AVALE=0,AVALG=1,count=0;
int Gpsdata;             // for incoming serial data
  unsigned int finish =0;  // indicate end of message
  unsigned int pos_cnt=0;  // position counter
  unsigned int lat_cnt=0;  // latitude data counterij.~~
  unsigned int log_cnt=0;  // longitude data counter
  unsigned int flg    =0;  // GPS flag
  unsigned int com_cnt=0;  // comma counter
  char lat[20];            // latitude array
  char lg[20];             // longitude array

void Receive_GPS_Data();

//cayenne app
//email.id: pillboxnew@gmail.com
//p.w: 123456789

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000

// Create a PulseOximeter object
PulseOximeter pox;
int t=0,B,A1=0,A2=0;
int S=0,H;
int count0=0,count1=0,hb,spo2;
int C=1,D=1,E=0,F=0;
#define buzzer D5




uint32_t tsLastReport = 0;

// Callback routine is executed when a pulse is detected
void onBeatDetected() 
{
    Serial.println("Beat!");
}

void setup() {
   // initialize LCD
   pinMode(A0,INPUT);
   
  lcd.begin();
  // turn on LCD backlight                      
  lcd.backlight();
  pinMode(buzzer,OUTPUT);
    Serial.begin(9600);
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
   WiFi.begin(ssid , password);
  
    
   

    Serial.print("Initializing pulse oximeter..");

    // Initialize sensor
    if (!pox.begin()) {
        Serial.println("FAILED");
        for(;;);
    } else {
        Serial.println("SUCCESS");
    }

  // Configure sensor to use 7.6mA for LED drive
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

    // Register a callback routine
    pox.setOnBeatDetectedCallback(onBeatDetected);


if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
 
  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
 
  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
 
  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
 
  // Add a delay for stability
  delay(100);

}

void loop()
{
  if ( count<10)
  { 
    // Read from the sensor
    pox.update();

    // Grab the updated heart rate and SpO2 levels
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        Serial.print("Heart rate:");
        Serial.print(pox.getHeartRate());
        Serial.print("SpO2:");
        Serial.print(pox.getSpO2());
        Serial.println("%");
       S=(pox.getSpO2());
        H=(pox.getHeartRate());
         tsLastReport = millis(); // cayenne virtualWrite(1, pox.getHeartRate())
       count0++;
    }
  }
  if(count0>=5)
  {
    temp=analogRead(A0)/2.85;
    count0=0;
Blynk.virtualWrite(V3, temp);
     Blynk.virtualWrite(V0, H);
     Blynk.virtualWrite(V1, S);
// Get new sensor events with the readings
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
 
  // Print out the acceleration readings in m/s^2
  Serial.print("Acceleration:   X:");
  Serial.print(a.acceleration.x);
 // Serial.print(", Y:");
  //Serial.print(a.acceleration.y);
  //Serial.print(", Z:");
 // Serial.print(a.acceleration.z);
  //Serial.println(" (m/s^2)");
 Blynk.virtualWrite(V2, a.acceleration.x);
if(a.acceleration.x>=8)
{
  digitalWrite(buzzer,HIGH);
  delay(3000);
  digitalWrite(buzzer,LOW);
}
     if (!pox.begin())
      {
        Serial.println("FAILED");
        for(;;);
    } 
    else
     {
        Serial.println("SUCCESS");
    }

    pox.setOnBeatDetectedCallback(onBeatDetected);
  }
}
//***************************************************************************************************************************************************************
// GPS VALUE
//***************************************************************************************************************************************************************
void Receive_GPS_Data()
{
  
while(finish==0){
while(Serial.available()>0)
{
  Gpsdata = Serial.read();
flg = 1;
if( Gpsdata=='$' && pos_cnt == 0)   // finding GPRMC header
pos_cnt=1;
if( Gpsdata=='G' && pos_cnt == 1)
pos_cnt=2;
if( Gpsdata=='P' && pos_cnt == 2)
pos_cnt=3;
if( Gpsdata=='R' && pos_cnt == 3)
pos_cnt=4;
if( Gpsdata=='M' && pos_cnt == 4)
pos_cnt=5;
if( Gpsdata=='C' && pos_cnt==5 )
pos_cnt=6;
if(pos_cnt==6 &&  Gpsdata ==','){   // count commas in message
com_cnt++;
flg=0;
}

if(com_cnt==3 && flg==1){
if(lat_cnt<10)
{
lat[lat_cnt++] =  Gpsdata;  
}// latitude
flg=0;
}

if(com_cnt==5 && flg==1){
if(log_cnt<12)
{
lg[log_cnt++] =  Gpsdata;         // Longitude

}
flg=0;
}

if( Gpsdata == '*' && com_cnt >= 5){
com_cnt = 0;                      // end of GPRMC message
lat_cnt = 0;
log_cnt = 0;
flg     = 0;
finish  = 1;
}
}
}
}