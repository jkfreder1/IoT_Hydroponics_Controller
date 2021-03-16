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
#include "WiFi.h"
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h> 
#include "SPIFFS.h"
#include <stdlib.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <string.h>
#include <bits/stdc++.h> 

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
int counter3 = 0;
// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
//mqtt credentials
const char* ssid = "ATT9JNx4NI";
const char* password = "6fzv98tb2x?5";
const char* mqttServer = "192.168.1.66";
const int mqttPort = 1883;
const char* mqttUser = "user";
const char* mqttPassword = "password";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
float temperatureMQTT = 0;
float humidityMQTT = 0;

AsyncWebServer server(80);////////new web server
DNSServer dns;
const int ledPin = 2;
String ledState;
const char* http_username = "admin";
const char* http_password = "admin";

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

uint8_t LED1pin = 26;
bool LED1status = LOW;

uint8_t LED2pin = 27;
bool LED2status = LOW;



void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      //digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      //digitalWrite(ledPin, LOW);
    }
  }
}





OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE); //lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);


hw_timer_t * timer = NULL;

//database initialization
StaticJsonDocument<512> doc1;
String jsonData1 = "/data1.json";

StaticJsonDocument<512> doc2;
String jsonData2 = "/data2.json";

StaticJsonDocument<512> doc3;
String jsonData3 = "/data3.json";

StaticJsonDocument<512> doc4;
String jsonData4 = "/data4.json";

StaticJsonDocument<512> doc5;
String jsonData5 = "/data5.json";

StaticJsonDocument<1024> daily1;
String jsonDaily1 = "/daily1.json";

StaticJsonDocument<1024> daily2;
String jsonDaily2 = "/daily2.json";

StaticJsonDocument<1024> daily3;
String jsonDaily3 = "/daily3.json";

StaticJsonDocument<1024> daily4;
String jsonDaily4 = "/daily4.json";

StaticJsonDocument<1024> daily5;
String jsonDaily5 = "/daily5.json";

StaticJsonDocument<1024> weekly1;
StaticJsonDocument<1024> weekly2;
StaticJsonDocument<1024> weekly3;
StaticJsonDocument<1024> weekly4;
StaticJsonDocument<1024> weekly5;

StaticJsonDocument<1024> monthly1;
StaticJsonDocument<1024> monthly2;
StaticJsonDocument<1024> monthly3;
StaticJsonDocument<1024> monthly4;
StaticJsonDocument<1024> monthly5;

JsonObject airTemp = doc1.to<JsonObject>();
JsonObject airHum = doc2.to<JsonObject>();
JsonObject waterTemp = doc3.to<JsonObject>();
JsonObject tds = doc4.to<JsonObject>();
JsonObject pH = doc5.to<JsonObject>();

JsonObject dailyAirTempAvg = daily1.to<JsonObject>();
JsonObject dailyAirHumAvg = daily2.to<JsonObject>();
JsonObject dailyWaterTempAvg = daily3.to<JsonObject>();
JsonObject dailyTdsAvg = daily4.to<JsonObject>();
JsonObject dailypHAvg = daily5.to<JsonObject>();


JsonArray data1 = airTemp.createNestedArray("dataset");
int airTempCount = 0;
int totCount1 = 0;

JsonArray data2 = airHum.createNestedArray("dataset");
//int airHumCount = 0;
int totCount2 = 0;

JsonArray data3 = waterTemp.createNestedArray("dataset");
//int waterTempCount = 0;
int totCount3 = 0;

JsonArray data4 = tds.createNestedArray("dataset");
//int tdsCount = 0;
int totCount4 = 0;

JsonArray dailyAirTempData = dailyAirTempAvg.createNestedArray("dataset");
JsonArray dailyAirHumData = dailyAirHumAvg.createNestedArray("dataset");
JsonArray dailyWaterTempData = dailyWaterTempAvg.createNestedArray("dataset");
JsonArray dailyTdsData = dailyTdsAvg.createNestedArray("dataset");
JsonArray dailypHData = dailypHAvg.createNestedArray("dataset");

int dailyCount = 0;
int weeklyCount = 0;
int monthlyCount = 0;

float avgAirTemp = 0;
float avgAirHum = 0;
float avgWaterTemp = 0;
float avgpH = 0;
float avgTDS = 0;

float dailyAirTemp = 0;

float calcAverage(float avg, float data){
  avg += data;
  avg = avg/2;
  return avg;
}

void printJSON(){
  uint8_t* pBuffer = nullptr;
  File testfile = SPIFFS.open("/data.json", "r");
  if(testfile){
    unsigned int fileSize = testfile.size();
    pBuffer = (uint8_t*)malloc(fileSize + 1);
    testfile.read(pBuffer, fileSize);
    pBuffer[fileSize] = '\0';
    Serial.println((char*)pBuffer);                // Print the file to the serial monitor.
    testfile.close();
  }
  else{
    Serial.println("Failed to read to file");
  }
  free(pBuffer);
}

void store_data(float val, JsonObject root, JsonArray data, int count, String fileName){
  File outfile = SPIFFS.open(fileName, "w");
  data[count] = val;
  if(serializeJsonPretty(root, outfile) == 0){
    Serial.println("Failed to write to file");
  }
  outfile.close();
}

void store_daily(JsonArray liveData, JsonArray dailyData, JsonObject root, String fileName){
  File outfile = SPIFFS.open(fileName, "w");
  for (int i = 0; i < 24; i++){
    dailyAirTemp = calcAverage(dailyAirTemp, liveData[i]);
  }
  dailyData.add(dailyAirTemp);
  if(serializeJsonPretty(root, outfile) == 0){
    Serial.println("Failed to write to file");
  }
  outfile.close();
}

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





String readBME280Humidity() {
  float h = 65 + rand() % (( 85 + 1 ) -65);
  if (isnan(h)) {
    Serial.println("Failed to read from BME280 sensor!");
    return "";
  }
  else {
    //Serial.println(h);
    return String(h);
  }
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if(digitalRead(ledPin)){
      ledState = "ON";
    }
    else{
      ledState = "OFF";
    }
    Serial.print(ledState);
    return ledState;
  }
  return String();
}

 void routes(){
    // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  // Route to load style.scss file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/style.css", "text/css");
  });
  // Route to load script.js file
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

/*

  // Route to load data1.json file 
  server.on("/data1.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/data1.json", "application/json");
  });

   // Route to load data2.json file 
  server.on("/data2.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/data2.json", "application/json");
  });
*/



  // Route to set GPIO to HIGH
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    digitalWrite(ledPin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  // Route to set GPIO to LOW
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    digitalWrite(ledPin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(401);
  });
  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request){
   request->send(SPIFFS, "/logged_out.html", String(), false, processor);
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readBME280Humidity().c_str());
  });
 }



void setup(){
  Serial.begin (115200);
  Serial.print("it is starting");
  //setup_wifi();

  
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  

  pinMode(trigPin, OUTPUT);///sonar
  pinMode(echoPin, INPUT);

  lcd.begin();/////display
  lcd.backlight();

  dht.begin();///temp humid

  sensors.begin();///water temp
  pinMode(TdsSensorPin,INPUT);

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



  //pinMode(ledPin, OUTPUT);///////new web server
  
  
  AsyncWiFiManager wifiManager(&server,&dns);
  wifiManager.autoConnect("AutoConnectAP");
    Serial.println("connected...yeey :)");

  
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }


  Serial.println(WiFi.localIP());
  routes();
  server.begin();
}
void reconnect() {
  // Loop until we're reconnected

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),mqttUser,mqttPassword)) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
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

  if(counter>10&&counter<15){
    h = dht.readHumidity();//DHT temp humid
    avgAirHum = calcAverage(avgAirHum, h);
    f = dht.readTemperature(true);
    avgAirTemp = calcAverage(avgAirTemp, f);
  }

  //if(counter>120&&counter<130){
  sensors.requestTemperatures();//Water temp
  temperatureF = sensors.getTempFByIndex(0);
  //}

//if(counter>150){
 array1=analogRead(0);//////ph sensor
 array2=analogRead(0);
 array3=analogRead(0);
 array1=array2+array3+array1;
 float volt=(float)array1*5.0/1024/3;
 ph_act = -5.70 * volt + calibration_value;
 //}

 if(counter>15){
 counter=0;
 totCount1++;
 }

  //store live and historical data
 if(totCount1 > 5){
  if(airTempCount == 24){
    airTempCount = 0;
  }
  
  float fakeAirTemp = 65 + rand() % (( 85 + 1 ) -65);
  float fakeAirHumid = 65 + rand() % (( 85 + 1 ) -65);

  store_data(fakeAirTemp, airTemp, data1, airTempCount, jsonData1);
  store_data(fakeAirHumid, airHum, data2, airTempCount, jsonData2);
  //store_data(avgWaterTemp, waterTemp, data3, airTempCount, jsonData3);
  //store_data(avgTDS, tds, data4, airTempCount, jsonData4);
  airTempCount++;
  totCount1 = 0;
  serializeJsonPretty(doc2, Serial);
  //printJSON();
  dailyCount++;
  weeklyCount++;
  monthlyCount++;
 }

 if(dailyCount == 24){
   store_daily(data1, dailyAirTempData, dailyAirTempAvg, jsonDaily1);
   store_daily(data2, dailyAirHumData, dailyAirHumAvg, jsonDaily2);
   //store_daily(data3, dailyWaterTempData, dailyWaterTempAvg, jsonDaily3);
   //store_daily(data4, dailyTdsData, dailyTdsAvg, jsonDaily4);
   //store_daily(data5, dailypHData, dailypHAvg, jsonDaily5);
   serializeJsonPretty(daily2, Serial);
   dailyCount = 0;
 }

  // Route to load data1.json file 
  server.on("/data1.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/data1.json", "application/json");
  });

   // Route to load data2.json file 
  server.on("/data2.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/data2.json", "application/json");
  });

   // Route to load data2.json file 
  server.on("/daily1.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/daily1.json", "application/json");
  });

   // Route to load data2.json file 
  server.on("/daily2.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/daily2.json", "application/json");
  });




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

/*
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

if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
    // Temperature in Celsius
    temperatureMQTT = 1.35;   
    // Uncomment the next line to set temperature in Fahrenheit 
    // (and comment the previous temperature line)
    //temperature = 1.8 * bme.readTemperature() + 32; // Temperature in Fahrenheit
    
    // Convert the value to a char array
    char tempString[8];
    dtostrf(temperatureMQTT, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);
    client.publish("esp32/temperature", tempString);

    humidityMQTT = 4.96;
    
    // Convert the value to a char array
    char humString[8];
    dtostrf(humidityMQTT, 1, 2, humString);
    Serial.print("Humidity: ");
    Serial.println(humString);
    client.publish("esp32/humidity", humString);
  }
  
*/
}
