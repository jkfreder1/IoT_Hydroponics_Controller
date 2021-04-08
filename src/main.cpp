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
#include "time.h"
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

int waterTempHighParam=90;
int waterTempLowParam=40;
int airHumHighParam=60;
int airHumLowParam=20;
int airTempHighParam=80;
int airTempLowParam=50;
int sonarHighParam=2;
int sonarLowParam=8;
int tdshighParam=400;
int tdslowParam=50;
int phhighParam=9;
int phlowParam=2;

int counter=0;
/*
#define uS_TO_S_FACTOR 1000000 //////////sleep DONT DELETE
#define TIME_TO_SLEEP  20        
#define BUTTON_PIN_BITMASK 0x200000000
*/

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
//mqtt credentials
const char* ssid = "Clairvoyant";
const char* password = "PaLe1SaRacHer";
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

float calibration_value = 21;
int phval = 0; 
unsigned long int avgval; 
int array1;
int array2;
int array3;

float h;
float f;
float temperatureF;
float ph_act;

int button=0;
int counter2;
int clear;

bool errorAir = 0;
bool errorWaterTemp = 0;
bool errorpH = 0;
bool errorTDS = 0;
bool errorWaterLevel = 0;

int h_offset,f_offset,ph_act_offset,inches_offset,temperatureF_offset,tds_offset;
int h_input=32,f_input=72,ph_act_input=7,inches_input=5,temperatureF_input=67,tds_value_input=83;


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
StaticJsonDocument<512> doc6;
String jsonData6 = "/data6.json";
StaticJsonDocument<512> timeStamp;
String jsonTimeStamp = "/timeStamp.json";
//daily documents
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
StaticJsonDocument<1024> daily6;
String jsonDaily6 = "/daily6.json";
StaticJsonDocument<1024> dailyTimeStamp;
String jsonDailyTimeStamp = "/dailyTimeStamp.json";
//weekly documents
StaticJsonDocument<1024> weekly1;
String jsonWeekly1 = "/weekly1.json";
StaticJsonDocument<1024> weekly2;
String jsonWeekly2 = "/weekly2.json";
StaticJsonDocument<1024> weekly3;
String jsonWeekly3 = "/weekly3.json";
StaticJsonDocument<1024> weekly4;
String jsonWeekly4 = "/weekly4.json";
StaticJsonDocument<1024> weekly5;
String jsonWeekly5 = "/weekly5.json";
StaticJsonDocument<1024> weekly6;
String jsonWeekly6 = "/weekly6.json";
StaticJsonDocument<1024> weeklyTimeStamp;
String jsonWeeklyTimeStamp = "/weeklyTimeStamp.json";
//monthly documents
StaticJsonDocument<1024> monthly1;
String jsonMonthly1 = "/monthly1.json";
StaticJsonDocument<1024> monthly2;
String jsonMonthly2 = "/monthly2.json";
StaticJsonDocument<1024> monthly3;
String jsonMonthly3 = "/monthly3.json";
StaticJsonDocument<1024> monthly4;
String jsonMonthly4 = "/monthly4.json";
StaticJsonDocument<1024> monthly5;
String jsonMonthly5 = "/monthly5.json";
StaticJsonDocument<1024> monthly6;
String jsonMonthly6= "/monthly6.json";
StaticJsonDocument<1024> monthlyTimeStamp;
String jsonMonthlyTimeStamp = "/monthlyTimeStamp.json";

JsonObject airTemp = doc1.to<JsonObject>();
JsonObject airHum = doc2.to<JsonObject>();
JsonObject waterTemp = doc3.to<JsonObject>();
JsonObject tds = doc4.to<JsonObject>();
JsonObject pH = doc5.to<JsonObject>();
JsonObject waterLevel = doc6.to<JsonObject>();
JsonObject timex = timeStamp.to<JsonObject>();

JsonObject dailyAirTempAvg = daily1.to<JsonObject>();
JsonObject dailyAirHumAvg = daily2.to<JsonObject>();
JsonObject dailyWaterTempAvg = daily3.to<JsonObject>();
JsonObject dailyTdsAvg = daily4.to<JsonObject>();
JsonObject dailypHAvg = daily5.to<JsonObject>();
JsonObject dailyWaterLevelAvg = daily6.to<JsonObject>();
JsonObject dailyTime = dailyTimeStamp.to<JsonObject>();

JsonObject weeklyAirTempAvg = weekly1.to<JsonObject>();
JsonObject weeklyAirHumAvg = weekly2.to<JsonObject>();
JsonObject weeklyWaterTempAvg = weekly3.to<JsonObject>();
JsonObject weeklyTdsAvg = weekly4.to<JsonObject>();
JsonObject weeklypHAvg = weekly5.to<JsonObject>();
JsonObject weeklyWaterLevelAvg = weekly6.to<JsonObject>();
JsonObject weeklyTime = weeklyTimeStamp.to<JsonObject>();

JsonObject monthlyAirTempAvg = monthly1.to<JsonObject>();
JsonObject monthlyAirHumAvg = monthly2.to<JsonObject>();
JsonObject monthlyWaterTempAvg = monthly3.to<JsonObject>();
JsonObject monthlyTdsAvg = monthly4.to<JsonObject>();
JsonObject monthlypHAvg = monthly5.to<JsonObject>();
JsonObject monthlyWaterLevelAvg = monthly6.to<JsonObject>();
JsonObject monthlyTime = monthlyTimeStamp.to<JsonObject>();


JsonArray data1 = airTemp.createNestedArray("dataset");
int airTempCount = 0;
int totCount1 = 0;
JsonArray data2 = airHum.createNestedArray("air humidity data");
//int airHumCount = 0;
int totCount2 = 0;
JsonArray data3 = waterTemp.createNestedArray("water temp data");
//int waterTempCount = 0;
int totCount3 = 0;
JsonArray data4 = tds.createNestedArray("tds data");
//int tdsCount = 0;
int totCount4 = 0;
JsonArray data5 = pH.createNestedArray("dataset");
//int tdsCount = 0;
int totCount5 = 0;
JsonArray data6 = waterLevel.createNestedArray("dataset");
//int tdsCount = 0;
int totCount6 = 0;

JsonArray dailyAirTempData = dailyAirTempAvg.createNestedArray("dataset");
JsonArray dailyAirHumData = dailyAirHumAvg.createNestedArray("dataset");
JsonArray dailyWaterTempData = dailyWaterTempAvg.createNestedArray("dataset");
JsonArray dailyTdsData = dailyTdsAvg.createNestedArray("dataset");
JsonArray dailypHData = dailypHAvg.createNestedArray("dataset");
JsonArray dailyWaterLevelData = dailyWaterLevelAvg.createNestedArray("dataset");

JsonArray weeklyAirTempData = weeklyAirTempAvg.createNestedArray("dataset");
JsonArray weeklyAirHumData = weeklyAirHumAvg.createNestedArray("dataset");
JsonArray weeklyWaterTempData = weeklyWaterTempAvg.createNestedArray("dataset");
JsonArray weeklyTdsData = weeklyTdsAvg.createNestedArray("dataset");
JsonArray weeklypHData = weeklypHAvg.createNestedArray("dataset");
JsonArray weeklyWaterLevelData = weeklyWaterLevelAvg.createNestedArray("dataset");

JsonArray monthlyAirTempData = monthlyAirTempAvg.createNestedArray("dataset");
JsonArray monthlyAirHumData = monthlyAirHumAvg.createNestedArray("dataset");
JsonArray monthlyWaterTempData = monthlyWaterTempAvg.createNestedArray("dataset");
JsonArray monthlyTdsData = monthlyTdsAvg.createNestedArray("dataset");
JsonArray monthlypHData = monthlypHAvg.createNestedArray("dataset");
JsonArray monthlyWaterLevelData = monthlyWaterLevelAvg.createNestedArray("dataset");

int dailyCount = 0;
int weeklyCount = 0;
int monthlyCount = 0;
int avgCount = 1;

float avgAirTemp = 0;
float avgAirHum = 0;
float avgWaterTemp = 0;
float avgpH = 0;
float avgTDS = 0;
float avgWaterLevel = 0;

float dailyAirTemp = 0;
float dailyAirHum = 0;
float dailyWaterTemp = 0;
float dailypH = 0;
float dailyTDS = 0;
float dailyWaterLevel = 0;

float weeklyAirTemp = 0;
float weeklyAirHum = 0;
float weeklyWaterTemp = 0;
float weeklypH = 0;
float weeklyTDS = 0;
float weeklyWaterLevel = 0;

float monthlyAirTemp = 0;
float monthlyAirHum = 0;
float monthlyWaterTemp = 0;
float monthlypH = 0;
float monthlyTDS = 0;
float monthlyWaterLevel = 0;

int dailyArray = 0;
int weeklyArray = 0;
int monthlyArray = 0;

const char* ntpServer = "pool.ntp.org";

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  /*
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");
  

  Serial.println("Time variables");
  char timeHour[3];
  strftime(timeHour,3, "%H", &timeinfo);
  Serial.println(timeHour);
  char timeWeekDay[10];
  strftime(timeWeekDay,10, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
  */
}

float calcAverage(float avg, float data, int count){
  if (count == 1){
    avg = data;
    return avg;
  }
  else {
    avg = avg + ((data - avg)/count);
    return avg;
  }
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

void store_daily(JsonArray liveData, JsonArray dailyData, JsonObject root, String fileName, int dataCount, int arrayCount){
  File outfile = SPIFFS.open(fileName, "w");
  float tempVal = 0; 
  int count = 1;
  for (int i = 0; i < dataCount; i++){
    tempVal = calcAverage(tempVal, liveData[i], count);
  }
  dailyData[arrayCount] = tempVal;
  //dailyData.add(tempVal);
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


int sleeptime;
void IRAM_ATTR incrCounter(){
  counter++;
  counter2++;
  sleeptime++;
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
int cal=0;
void button3ISR(){
  cal=1;
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

  // Route to load script.js file
  server.on("/graphs.js", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/graphs.js", "text/javascript");
  });

  // Route to load script.js file
  server.on("/tables.js", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/tables.js", "text/javascript");
  });


  // Route to load data.json file 
  server.on("/data.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/data2.json", "application/json");
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

 void routesLoop() {
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

   // Route to load data3.json file 
 server.on("/data3.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/data3.json", "application/json");
  });

   // Route to load data4.json file 
  server.on("/data4.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/data4.json", "application/json");
  });

 // Route to load data4.json file 
  server.on("/data5.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/data5.json", "application/json");
  });

    // Route to load daily1.json file 
  server.on("/daily1.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/daily1.json", "application/json");
  });

   // Route to load daily2.json file 
  server.on("/daily2.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/daily2.json", "application/json");
  });

   // Route to load daily3.json file 
  server.on("/daily3.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/daily3.json", "application/json");
  });

   // Route to load daily4.json file 
  server.on("/daily4.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/daily4.json", "application/json");
  });
 
  // Route to load daily4.json file 
  server.on("/daily5.json", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send(SPIFFS, "/daily4.json", "application/json");
  });
 }



void setup(){
  Serial.begin (115200);
  Serial.print("it is starting");
  /*
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  */

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

  pinMode(5,INPUT);
  pinMode(15,INPUT);
  pinMode(19,INPUT);
  attachInterrupt(digitalPinToInterrupt(5), button3ISR,RISING);
  attachInterrupt(digitalPinToInterrupt(15), buttonISR, RISING);
  attachInterrupt(digitalPinToInterrupt(19), button2ISR, RISING);

  /*
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);/////sleep DONT DELETE
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_15,1);
  */
  
  
  AsyncWiFiManager wifiManager(&server,&dns);
  wifiManager.autoConnect("AutoConnectAP");
    Serial.println("connected...yeey :)");

  
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  store_data(0, dailyAirTempAvg, dailyAirTempData, 0, jsonDaily1);
  store_data(0, dailyAirHumAvg, dailyAirHumData, 0, jsonDaily2);
  store_data(0, dailyWaterTempAvg, dailyWaterTempData, 0, jsonDaily3);
  store_data(0, dailyTdsAvg, dailyTdsData, 0, jsonDaily4);
  store_data(0, dailypHAvg, dailypHData, 0, jsonDaily5);
  store_data(0, dailyWaterLevelAvg, dailyWaterLevelData, 0, jsonDaily6);

  store_data(0, weeklyAirTempAvg, weeklyAirTempData, 0, jsonWeekly1);
  store_data(0, weeklyAirHumAvg, weeklyAirHumData, 0, jsonWeekly2);
  store_data(0, weeklyWaterTempAvg, weeklyWaterTempData, 0, jsonWeekly3);
  store_data(0, weeklyTdsAvg, weeklyTdsData, 0, jsonWeekly4);
  store_data(0, weeklypHAvg, weeklypHData, 0, jsonWeekly5);
  store_data(0, weeklyWaterLevelAvg, weeklyWaterLevelData, 0, jsonWeekly6);

  store_data(0, monthlyAirTempAvg, monthlyAirTempData, 0, jsonMonthly1);
  store_data(0, monthlyAirHumAvg, monthlyAirHumData, 0, jsonMonthly2);
  store_data(0, monthlyWaterTempAvg, monthlyWaterTempData, 0, jsonMonthly3);
  store_data(0, monthlyTdsAvg, monthlyTdsData, 0, jsonMonthly4);
  store_data(0, monthlypHAvg, monthlypHData, 0, jsonMonthly5);
  store_data(0, monthlyWaterLevelAvg, monthlyWaterLevelData, 0, jsonMonthly6);

  
  Serial.println(WiFi.localIP());
  routes();
  server.begin();

  //configTime(0, 0, ntpServer);
  //printLocalTime();
  
  
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
 int clear2=0; 
 int errorflag=0;
 int errorflag1=0;
 int errorflag2=0;
 int errorflag3=0;
 int errorflag4=0;
 int errorflag5=0;

void displayWaterTemp(float temperatureF){
  lcd.setCursor(0,0);//water temp display
  if(temperatureF < waterTempLowParam && temperatureF>0){
  lcd.print("Error!          ");
  lcd.setCursor(0,1);
  lcd.print("Water temp low  ");
  clear2=1;
  }
  else if(temperatureF > waterTempHighParam){
    lcd.print("Water temp      ");
  lcd.setCursor(0,1);
  lcd.print("Is too high     ");
  clear2=1;
  }
  else if(temperatureF<0){
    clear2=1;
    lcd.print("Critical error");
    lcd.setCursor(0,1);
    lcd.print("Check Sensor");
    errorWaterTemp = 1;
  }
  else{
    if(clear2==1){
      clear2=0;
      lcd.clear();
    }
    lcd.print("Water temp ");
    lcd.print(temperatureF);
  }
}

int clear3=0;

void displayDHT(float h, float f){
  lcd.setCursor(0,0);//humidity display
  if(h < airHumLowParam && !isnan(h)){
  lcd.print("Air Humdity LOW ");
  clear2=1;
  }
  else if(h > airHumHighParam && !isnan(h)){
    lcd.print("Air Humdity HIGH");
    clear2=1;
  }
  else{
    if(clear2==1){
      clear2=0;
      lcd.clear();
    }
  lcd.print("humidity ");
    lcd.print(h);
    lcd.print("%    ");
  }
  lcd.setCursor(0,1); //air temp display
  if(f < airTempLowParam && !isnan(f)){
  lcd.print("AirTemp is low  ");
  clear3=1;
  }
  else if(f > airTempHighParam && !isnan(f)){
    lcd.print("AirTemp is high ");
    clear3=1;
  }
  else if (isnan(h) || isnan(f)) {
    clear2=1;
    lcd.setCursor(0,0);
    lcd.print("Critical error");
    lcd.setCursor(0,1);
    lcd.print("Check Sensor");
    errorAir = 0;
  }
  else{
    if(clear3==1){
      clear3=0;
      lcd.clear();
    }
  lcd.print("Air Temp ");
    lcd.print(f);
    lcd.print("F             ");
  }
}

void displaySonar(long inches){
  lcd.setCursor(0,0);//sonar display
  if(inches > sonarLowParam){
  lcd.print("Error The water ");
  lcd.setCursor(0,1);
  lcd.print("Is too low      ");
  clear2=1;
  }
  else if(inches < sonarHighParam&& inches > 0){
    lcd.print("Error The water ");
  lcd.setCursor(0,1);
  lcd.print("Is too high     ");
  clear2=1;
  }
  else if(inches==0){
    lcd.print("Critical error");
    lcd.setCursor(0,1);
    lcd.print("Check Sensor");
    errorWaterLevel = 1;
  }
  else{
    if(clear2==1){
      clear2=0;
      lcd.clear();
    }
    lcd.print(inches);
    lcd.print(" inches        ");
  }
}


void displayTDS(float tdsValue){
  lcd.setCursor(0,0);
  if(tdsValue<tdslowParam){
    lcd.print("Error TDS Low   ");
    clear2=1;
  }
  else if(tdsValue>tdshighParam){
    lcd.print("Error TDS High  ");
    clear2=1;
  }
  else{
    if(clear2==1){
      clear2=0;
      lcd.clear();
    }
  lcd.setCursor(0,0);
  lcd.print(tdsValue);
  lcd.print(" ppm          ");
  }
}

void displaypH(float ph_act){
  lcd.setCursor(0,0);
  if(ph_act<phlowParam&&ph_act>0){
    lcd.print("Error pH is Low ");
    clear2=1;
  }
  else if(ph_act>phhighParam){
    lcd.print("Error pH is High");
    clear2=1;
    errorpH = 1;
  }
  else{
    if(clear2==1){
      clear2=0;
      lcd.clear();
    }
  lcd.setCursor(0,0);
  lcd.print(ph_act);
  lcd.print("pH            ");
  }
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
 averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF/ 4096.0;//1024.0
 float compensationCoefficient=1.0+0.02*(temperature-25.0);
 float compensationVolatge=averageVoltage/compensationCoefficient;
tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5;
/*
if(tdsValue==0){
  errorflag1=1;
}
else{
  errorflag1=0;
}*/
}

 

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
  if(inches==0){
    errorflag2=1;
    clear2=1;
  }
  else{
    errorflag2=0;
  }
  //}

  //if(counter>10&&counter<15){
    h = dht.readHumidity();//DHT temp humid
    f = dht.readTemperature(true);
  //}

  if (isnan(h) || isnan(f)){
    errorflag3=1;
    clear2=1;
  }
  else{
    errorflag3=0;
  }

  //if(counter>120&&counter<130){
  sensors.requestTemperatures();//Water temp
  temperatureF = sensors.getTempFByIndex(0);
  if(temperatureF<0){
    errorflag4=1;
    clear2=1;
  }
  else{
    errorflag4=0;
  }
  //}

//if(counter>150){
 array1=analogRead(0);//////ph sensor
 array2=analogRead(0);
 array3=analogRead(0);
 array1=array2+array3+array1;
 float volt=(float)array1*5.0/4096/3;
 ph_act = (-5.70 * volt + calibration_value);
 if(ph_act<4){
   ph_act=ph_act+1;
 }
  if(ph_act<0){
   errorflag5=1;
   clear2=1;
 }
 else{
   errorflag5=0;
 }
 //}
 if(counter>10&&counter<15){ //take averages for all sensors
  avgAirTemp = calcAverage(avgAirTemp, f, avgCount);
  avgAirHum = calcAverage(avgAirHum, h, avgCount);
  avgWaterLevel = calcAverage(avgWaterLevel, inches, avgCount);
  avgpH = calcAverage(avgpH, ph_act, avgCount);
  avgWaterTemp = calcAverage(avgWaterTemp, temperatureF, avgCount);
  avgTDS = calcAverage(avgTDS, tdsValue, avgCount);
  avgCount++;
 }

 if(counter>15){
 counter=0;
 totCount1++;
 }

 if(totCount1 > 5){
   totCount2++;
 }

  //store live and historical data
 if(totCount1 > 2){
  if(airTempCount == 24){
    airTempCount = 0;
  }
  
  float fakeAirTemp = 65 + rand() % (( 85 + 1 ) -65);
  float fakeAirHumid = 30 + rand() % (( 80 + 1 ) -30);
  float fakeWaterTemp = 65 + rand() % (( 85 + 1 ) -65);
  float fakeTDS = 65 + rand() % (( 85 + 1 ) -65);
  float fakepH = 65 + rand() % (( 85 + 1 ) -65);
  float fakeWaterLevel = 65 + rand() % (( 85 + 1 ) -65);

  store_data(fakeAirTemp, airTemp, data1, airTempCount, jsonData1);
  store_data(fakeAirHumid, airHum, data2, airTempCount, jsonData2);
  store_data(fakeWaterTemp, waterTemp, data3, airTempCount, jsonData3);
  store_data(fakeTDS, tds, data4, airTempCount, jsonData4);
  store_data(fakepH, pH, data5, airTempCount, jsonData5);
  store_data(fakeWaterLevel, waterLevel, data6, airTempCount, jsonData6);
  airTempCount++;
  totCount1 = 0;
  avgCount = 1;
  serializeJsonPretty(doc2, Serial);
  //printJSON();
  dailyCount++;
  //weeklyCount++;
  //monthlyCount++;
 }

 if(dailyCount == 24){
   if(dailyArray == 24){
     dailyArray = 0;
   }
   store_daily(data1, dailyAirTempData, dailyAirTempAvg, jsonDaily1, 24, dailyArray);
   store_daily(data2, dailyAirHumData, dailyAirHumAvg, jsonDaily2, 24, dailyArray);
   store_daily(data3, dailyWaterTempData, dailyWaterTempAvg, jsonDaily3, 24, dailyArray);
   store_daily(data4, dailyTdsData, dailyTdsAvg, jsonDaily4, 24, dailyArray);
   store_daily(data5, dailypHData, dailypHAvg, jsonDaily5, 24, dailyArray);
   store_daily(data6, dailyWaterLevelData, dailyWaterLevelAvg, jsonDaily6, 24, dailyArray);
   dailyArray++;
   serializeJsonPretty(daily1, Serial);
   serializeJsonPretty(daily2, Serial);
   dailyCount = 0;
   weeklyCount++;
 }

 if(weeklyCount == 7){
   if(weeklyArray == 7){
     weeklyArray = 0;
   }
   store_daily(dailyAirTempData, weeklyAirTempData, weeklyAirTempAvg, jsonWeekly1, 7, weeklyArray);
   store_daily(dailyAirHumData, weeklyAirHumData, weeklyAirHumAvg, jsonWeekly2, 7, weeklyArray);
   store_daily(dailyWaterTempData, weeklyWaterTempData, weeklyWaterTempAvg, jsonWeekly3, 7, weeklyArray);
   store_daily(dailyTdsData, weeklyTdsData, weeklyTdsAvg, jsonWeekly4, 7, weeklyArray);
   store_daily(dailypHData, weeklypHData, weeklypHAvg, jsonWeekly5, 7, weeklyArray);
   store_daily(dailyWaterLevelData, weeklyWaterLevelData, weeklyWaterLevelAvg, jsonWeekly6, 7, weeklyArray);
   weeklyArray++;
   weeklyCount = 0;
   monthlyCount++;
 }

 if(monthlyCount == 4){
   if(monthlyArray == 4){
     monthlyArray = 0;
   }
   store_daily(weeklyAirTempData, monthlyAirTempData, monthlyAirTempAvg, jsonMonthly1, 4, monthlyArray);
   store_daily(weeklyAirHumData, monthlyAirHumData, monthlyAirHumAvg, jsonMonthly2, 4, monthlyArray);
   store_daily(weeklyWaterTempData, monthlyWaterTempData, monthlyWaterTempAvg, jsonMonthly3, 4, monthlyArray);
   store_daily(weeklyTdsData, monthlyTdsData, monthlyTdsAvg, jsonMonthly4, 4, monthlyArray);
   store_daily(weeklypHData, monthlypHData, monthlypHAvg, jsonMonthly5, 4, monthlyArray);
   store_daily(weeklyWaterLevelData, monthlyWaterLevelData, monthlyWaterLevelAvg, jsonMonthly6, 4, monthlyArray);
   monthlyArray++;
   monthlyCount = 0;
 }


routesLoop();
  

/*
//counter2=0;
 //}
 

if(clear==1){
  clear=0;
  lcd.clear();
}

if(button>5){
  button=0;
}

if(errorflag1==1||errorflag2==1||errorflag3==1||errorflag4==1||errorflag5==1){
  errorflag=1;
}
else{
  errorflag=0;
}

 switch (button){
    case 0:
      if(errorflag==1){
        lcd.setCursor(0,0);
        lcd.print("Critical Error  ");
        lcd.setCursor(0,1);
        lcd.print("Check Sensors   ");
      }
      if(errorflag==0){
        lcd.setCursor(0,0);
        lcd.print("Hydroponics     ");
      }
      break;
    case 1:
      if(errorflag==1){
        lcd.setCursor(0,0);
        lcd.print("Critical Error  ");
        lcd.setCursor(0,1);
        lcd.print("Check Sensors   ");
      }
      if(errorflag==0){
        displayDHT(h+h_offset,f+f_offset);
      }
      break;
    case 2:
      if(errorflag==1){
        lcd.setCursor(0,0);
        lcd.print("Critical Error  ");
        lcd.setCursor(0,1);
        lcd.print("Check Sensors   ");
      }
      if(errorflag==0){
        displayWaterTemp(temperatureF+temperatureF_offset);
      }
      break;
    case 3:
      if(errorflag==1){
        lcd.setCursor(0,0);
        lcd.print("Critical Error  ");
        lcd.setCursor(0,1);
        lcd.print("Check Sensors   ");
      }
      if(errorflag==0){
        displayTDS(tdsValue+tds_offset);
      }
      break;
    case 4:
      if(errorflag==1){
        lcd.setCursor(0,0);
        lcd.print("Critical Error  ");
        lcd.setCursor(0,1);
        lcd.print("Check Sensors   ");
      }
      if(errorflag==0){
        displaySonar(inches+inches_offset);
      }
      break;
    case 5:
      if(errorflag==1){
        lcd.setCursor(0,0);
        lcd.print("Critical Error  ");
        lcd.setCursor(0,1);
        lcd.print("Check Sensors   ");
      }
      if(errorflag==0){
        displaypH(ph_act+ph_act_offset);
      }
      break;
    default: break;
 }


  if(cal==1){
    cal=0;
  h_offset=h_input-h;
  f_offset=f_input-f;
  ph_act_offset=ph_act_input-ph_act;
  inches_offset=inches_input-inches;
  temperatureF_offset=temperatureF_input-temperatureF;
  tds_offset=tds_value_input-tdsValue;
  }
  /*
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

  //if(sleeptime>200){
    //esp_deep_sleep_start();/////sleep DONT DELETE
    //sleeptime=0;
  //}
}