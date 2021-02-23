
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <WiFi.h>
#include <WebServer.h>

#include <iostream>

#include <deque>
using namespace std;
deque<int> intd;
deque<int> inta;
deque<int> inta1;
deque<int> intw;
deque<int> inttds;
deque<int> intph;


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
int counter=0;

#define COLUMS           16
#define ROWS             2
#define DHTPIN 4
#define DHTTYPE DHT11
#define LCD_SPACE_SYMBOL 0x20  //space symbol from the LCD ROM, see p.9 of GDM2004D datasheet
const int oneWireBus = 23;

#define TdsSensorPin 34
#define VREF 5.0 // analog reference voltage(Volt) of the ADC
#define SCOUNT 30 // sum of sample point
int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 25;


float calibration_value = 120.34;//ph
int phval = 0; 
unsigned long int avgval; 
int array1;
int array2;
int array3;

float h;
float f;
float temperatureF;
int ph_act;


int button=0;
int counter2;
int clear;


const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);
uint8_t LED1pin = 26;
bool LED1status = LOW;

uint8_t LED2pin = 27;
bool LED2status = LOW;



OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE); //lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);


hw_timer_t * timer = NULL;


int getMedianNum(int bArray[], int iFilterLen)//////////tds sensor
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




String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +="table {border-collapse: collapse; border: 1px solid black; width: 100%;}";
  ptr +="th_1 {background-color: #3498db; color: #ffff;border: 1px solid black;}";
  ptr +="th_2 {1px solid black;}";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
   ptr +=".button-test {background-color: #34495e;}\n";
  ptr +=".button-test:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";



   if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}

  ptr += "<table class=\"table\"> <th class=\"th_1\">Water Temperature</th> <th class=\"th_1\">Air Temperature</th> <th class=\"th_1\">Air Humidity</th> <th class=\"th_1\">Water Level</th> </tr>";
  ptr += "<tr> <td>"; ptr+=intw[0]; ptr+="</td>"; ptr += "<td>"; ptr+=inta1[0]; ptr+="</td>"; ptr += "<td>"; ptr+=inta[0]; ptr+="</td>";ptr += "<td>"; ptr+=intd[0]; ptr+="</td>";ptr += "<td>"; ptr+=inttds[0]; ptr+="</td>";ptr += "<td>"; ptr+=intph[0]; ptr+="</td> </tr>";
  ptr += "<tr> <td>"; ptr+=intw[1]; ptr+="</td>"; ptr += "<td>"; ptr+=inta1[1]; ptr+="</td>"; ptr += "<td>"; ptr+=inta[1]; ptr+="</td>";ptr += "<td>"; ptr+=intd[1]; ptr+="</td>";ptr += "<td>"; ptr+=inttds[1]; ptr+="</td>";ptr += "<td>"; ptr+=intph[1]; ptr+="</td> </tr>";
  ptr += "<tr> <td>"; ptr+=intw[2]; ptr+="</td>"; ptr += "<td>"; ptr+=inta1[2]; ptr+="</td>"; ptr += "<td>"; ptr+=inta[2]; ptr+="</td>";ptr += "<td>"; ptr+=intd[2]; ptr+="</td>";ptr += "<td>"; ptr+=inttds[2]; ptr+="</td>";ptr += "<td>"; ptr+=intph[2]; ptr+="</td> </tr>";
  ptr += "<tr> <td>"; ptr+=intw[3]; ptr+="</td>"; ptr += "<td>"; ptr+=inta1[3]; ptr+="</td>"; ptr += "<td>"; ptr+=inta[3]; ptr+="</td>";ptr += "<td>"; ptr+=intd[3]; ptr+="</td>";ptr += "<td>"; ptr+=inttds[3]; ptr+="</td>";ptr += "<td>"; ptr+=intph[3]; ptr+="</td> </tr>";
  ptr += "<tr> <td>"; ptr+=intw[4]; ptr+="</td>"; ptr += "<td>"; ptr+=inta1[4]; ptr+="</td>"; ptr += "<td>"; ptr+=inta[4]; ptr+="</td>";ptr += "<td>"; ptr+=intd[4]; ptr+="</td>";ptr += "<td>"; ptr+=inttds[4]; ptr+="</td>";ptr += "<td>"; ptr+=intph[4]; ptr+="</td> </tr>";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO4 Status: OFF | GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status));
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status));
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status));
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true));
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false));
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

int store;
void IRAM_ATTR incrCounter(){
  counter++;
  counter2++;
  if(counter>160){
    store=1;;
  }
}

int h_offset,f_offset,ph_act_offset,inches_offset,temperatureF_offset,tds_offset;
int h_input,f_input,ph_act_input,inches_input,temperatureF_input,tds_value_input;

void calibration(float hum,float far,int ph,int in,float atemp,float tds){
  h_offset=h_input-hum;
  f_offset=f_input-far;
  ph_act_offset=ph_act_input-ph_act;
  inches_offset=inches_input-inches;
  temperatureF_offset=temperatureF_input-temperatureF;
  tds_offset=tds_value_input-tdsValue;
}

void buttonISR(){
  if(counter2>10){
  button++;
  clear=1;
  counter2=0;
  }
}

void button2ISR(){
  if(counter2>10){
    counter2=0;
    ESP.restart();
  }
}

void button3ISR(){
  calibration(h,f,ph_act,inches,temperatureF,tdsValue);
}

void setup(){
  Serial.begin (115200);
  pinMode(trigPin, OUTPUT);///sonar
  pinMode(echoPin, INPUT);

  lcd.begin();/////display
  lcd.backlight();

  dht.begin();///temp humid

  sensors.begin();///water temp
  pinMode(TdsSensorPin,INPUT);

  pinMode(LED1pin, OUTPUT);//wifi
  pinMode(LED2pin, OUTPUT);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");

  pinMode(TdsSensorPin,INPUT);//tds sensor

  timer = timerBegin(0, 8, true);//////////// timer
  timerAttachInterrupt(timer, &incrCounter, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);

  pinMode(13,OUTPUT);///button interrupts
  pinMode(18,INPUT);
  pinMode(15,INPUT);
  pinMode(19,INPUT);
  attachInterrupt(digitalPinToInterrupt(18), button3ISR,RISING);
  attachInterrupt(digitalPinToInterrupt(15), buttonISR, RISING);
  attachInterrupt(digitalPinToInterrupt(19), button2ISR, RISING);
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
}


void displayTDS(float tdsValue){
  lcd.setCursor(0,0);
  lcd.print(tdsValue);
  lcd.print(" ppm");
}

void displaypH(float ph_act){
  lcd.setCursor(0,0);
  lcd.print(ph_act);
  lcd.print("pH");
}



void loop(){

 //if(counter>30&&counter<40){
 static unsigned long analogSampleTimepoint = millis();//Tds sensor
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
}
 //}

  //if(counter>60&&counter<70){
  digitalWrite(trigPin, LOW);//sonar sensor
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;
  inches = (duration/2) / 74;
  //}

  //if(counter>90&&counter<100){
  h = dht.readHumidity();//DHT temp humid
  f = dht.readTemperature(true);
  //}

  //if(counter>120&&counter<130){
  sensors.requestTemperatures();//Water temp
  temperatureF = sensors.getTempFByIndex(0);
  //}

//if(counter>150){
 array1=analogRead(0);
 array2=analogRead(0);
 array3=analogRead(0);
 array1=array2+array3+array1;
 float volt=(float)array1*5.0/1024/3;
 ph_act = -5.70 * volt + calibration_value;
 //}

 //if(counter>160){
 //counter=0;
//counter2=0;
 //}
 if(store==1){
    store=0;
    intd.push_back(inches);
    inta.push_back(h);
    inta1.push_back(f);
    intw.push_back(temperatureF);
    inttds.push_back(tdsValue);
    intph.push_back(ph_act);
 }

if(clear==1){
  clear=0;
  lcd.clear();
}

if(button>4){//5
  button=0;
}

  

 switch (button){
    case 0:
      lcd.setCursor(0,0);
      lcd.print("Hydroponics");
      break;
    case 1:
      displayDHT(h,f);
      break;
    case 2:
      displayWaterTemp(temperatureF);
      break;
    case 3:
      displayTDS(tdsValue);
      break;
    case 4:
      displaySonar(inches);
      break;
    //case 5:
      //displaypH(ph_act);
      //break;
    default: break;
 }

  if(intph.size()>5){
    intph.pop_front();
  }
  if(inttds.size()>5){
    inttds.pop_front();
  }
  if(intd.size()>5){
    intd.pop_front();
  }
  if(inta.size()>5){
    inta.pop_front();
  }
  if(inta1.size()>5){
    inta1.pop_front();
  }
  if(intw.size()>5){
    intw.pop_front();
  }

  server.handleClient();
  if(LED1status)
  {digitalWrite(LED1pin, HIGH);}
  else
  {digitalWrite(LED1pin, LOW);}

  if(LED2status)
  {digitalWrite(LED2pin, HIGH);}
  else
  {digitalWrite(LED2pin, LOW);}

}