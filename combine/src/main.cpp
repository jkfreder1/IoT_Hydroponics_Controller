#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>

int trigPin = 33;    // Trigger
int echoPin = 25;    // Echo
long duration, cm, inches;

float waterTempHighParam=90;
float waterTempLowParam=40;
float airHumHighParam=80;
float airHumLowParam=20;
float airTempHighParam=90;
float airTempLowParam=50;

long sonarHighParam=2;
long sonarLowParam=8;

#define COLUMS           16
#define ROWS             2
#define DHTPIN 4 
#define DHTTYPE DHT11 
#define LCD_SPACE_SYMBOL 0x20  //space symbol from the LCD ROM, see p.9 of GDM2004D datasheet
const int oneWireBus = 23;  

#define TdsSensorPin 14
#define VREF 5.0 // analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point
int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 25;  

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);
DHT dht(DHTPIN, DHTTYPE); 


void setup(){
  Serial.begin (115200);
  pinMode(trigPin, OUTPUT);///sonar
  pinMode(echoPin, INPUT);

  lcd.begin();/////display
  lcd.backlight();

  dht.begin();///temp humid

  sensors.begin();///water temp
  pinMode(TdsSensorPin,INPUT);
}

int getMedianNum(int bArray[], int iFilterLen)
{
 int bTab[iFilterLen];
 for (byte i = 0; i<iFilterLen; i++)
 bTab[i] = bArray[i];
 int i, j, bTemp;
 for (j = 0; j < iFilterLen - 1; j++)
 {
 for (i = 0; i < iFilterLen - j - 1; i++)
 {
 if (bTab[i] > bTab[i + 1])
 {
 bTemp = bTab[i];
 bTab[i] = bTab[i + 1];
 bTab[i + 1] = bTemp;
 }
 }
 }
 if ((iFilterLen & 1) > 0)
 bTemp = bTab[(iFilterLen - 1) / 2];
 else
 bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
 return bTemp;
} 

void displayWaterTemp(float temperatureF){
  lcd.setCursor(0,0);//water temp display
  if(temperatureF < waterTempLowParam){
	lcd.print("Error!");
	lcd.setCursor(0,1);
	lcd.print("Water temp low");
  }
  else if(temperatureF > waterTempHighParam){
    lcd.print("Error!");
	lcd.setCursor(0,1);
	lcd.print("Water temp high");
  }
  else{
	lcd.print("Water temp ");
  	lcd.print(temperatureF);
  }
  delay(5000);
  lcd.clear();
}

void displayDHT(float h, float f){
  lcd.setCursor(0,0);//humidity display
  if(h < airHumLowParam){
	lcd.print("Error! AirHum");
  }
  else if(h > airHumHighParam){
    lcd.print("Error! AirHum");
  }
  else{
	lcd.print("humidity ");
  	lcd.print(h);
    lcd.print("%");
  }
  
  lcd.setCursor(0,1); //air temp display
  if(f < airTempLowParam){
	lcd.print("Error! AirTemp");
  }
  else if(f > airTempHighParam){
    lcd.print("Error! AirTemp");
  }
  else{
	lcd.print("Air Temp "); 
  	lcd.print(f);
    lcd.print("F");
  }
  delay(5000);
  lcd.clear();
}

void displaySonar(long inches){
  lcd.setCursor(0,0);//sonar display
  if(inches > sonarLowParam){
	lcd.print("Error!");
	lcd.setCursor(0,1);
	lcd.print("Water too low");
  }
  else if(inches < sonarHighParam){
    lcd.print("Error!");
	lcd.setCursor(0,1);
	lcd.print("Water too high");
  }
  else{
  	lcd.print(inches);
    lcd.print(" inches");
  }
  delay(5000);
  lcd.clear();
}

void displayTDS(float tdsValue){
  lcd.setCursor(0,0);
  lcd.print(tdsValue);
  lcd.print(" ppm");
  delay(5000);
  lcd.clear();
}

void loop(){
/*
static unsigned long analogSampleTimepoint = millis();//Tds value
 if(millis()-analogSampleTimepoint > 40U) {
 analogSampleTimepoint = millis();
 analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);
 analogBufferIndex++;
 if(analogBufferIndex == SCOUNT)
 analogBufferIndex = 0;
 }
 static unsigned long printTimepoint = millis();
 if(millis()-printTimepoint > 800U){
 printTimepoint = millis();
 for(copyIndex=0;copyIndex<SCOUNT;copyIndex++)
 analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
 averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF/ 1024.0;
 float compensationCoefficient=1.0+0.02*(temperature-25.0);
 float compensationVolatge=averageVoltage/compensationCoefficient;
tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5;
 }*/

  digitalWrite(trigPin, LOW);///////sonar sensor
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;     
  inches = (duration/2) / 74;
  
  //delay(2000);//////DHT temp humid
  float h = dht.readHumidity();
  float f = dht.readTemperature(true);

  sensors.requestTemperatures();/////Water temp
  float temperatureF = sensors.getTempFByIndex(0);

  displayWaterTemp(temperatureF);

  displayDHT(h, f);

  displaySonar(inches);

  displayTDS(tdsValue);
}