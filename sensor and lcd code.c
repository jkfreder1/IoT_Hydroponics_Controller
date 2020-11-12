#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>

int trigPin = 33;    // Trigger
int echoPin = 25;    // Echo
long duration, cm, inches;

float waterTempHighParam;
float waterTempLowParam;
float airHumHighParam;
float airHumLowParam;
float airTempHighParam;
float airTempLowParam;

long sonarHighParam;
long sonarLowParam;

#define COLUMS           16
#define ROWS             2
#define DHTPIN 4 
#define DHTTYPE DHT11 
#define LCD_SPACE_SYMBOL 0x20  //space symbol from the LCD ROM, see p.9 of GDM2004D datasheet
const int oneWireBus = 23;     

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
  }
  
  lcd.setCursor(0,1); //air temp display
  if(f < airTempLowParam){
	lcd.print("Error! AirTemp");
  }
  else if(f > airTempHighParam){
    lcd.print("Error! AirTemp");
  }
  else{
	lcd.print("humidity ");
  	lcd.print(h);
  }
  delay(5000);
  lcd.clear();
}

void displaySonar(long inches){
  lcd.setCursor(0,0);//sonar display
  if(inches < sonarLowParam){
	lcd.print("Error!");
	lcd.setCursor(0,1);
	lcd.print("Water too low");
  }
  else if(inches > sonarHighParam){
    lcd.print("Error!");
	lcd.setCursor(0,1);
	lcd.print("Water too high");
  else{
	lcd.print("inches ");
  	lcd.print(inches);
  }
  delay(5000);
  lcd.clear();
}

void loop(){
  digitalWrite(trigPin, LOW);///////sonar sensor
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;     
  inches = (duration/2) / 74;
  
  delay(2000);//////DHT temp humid
  float h = dht.readHumidity();
  float f = dht.readTemperature(true);

  sensors.requestTemperatures();/////Water temp
  float temperatureF = sensors.getTempFByIndex(0);

  displayWaterTemp(temperatureF);

  displayDHT(h, f);

  displaySonar(inches);
}