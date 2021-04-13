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
#include <AsyncTCP.h>
using namespace std;
deque<int> intd;
deque<int> inta;
deque<int> inta1;
deque<int> intw;
deque<int> inttds;
deque<int> intph;

int trigPin = 33;    // Trigger
int echoPin = 32;    // 25 Echo
long duration, cm, inches;

float waterTempHighParam=90;
float waterTempLowParam=40;
float airHumHighParam=60;
float airHumLowParam=20;
float airTempHighParam=80;
float airTempLowParam=50;
float sonarHighParam=2;
float sonarLowParam=8;
float tdshighParam=400;
float tdslowParam=50;
float phhighParam=9;
float phlowParam=2;

int counter=0;
/*
#define uS_TO_S_FACTOR 1000000 //////////sleep DONT DELETE
#define TIME_TO_SLEEP  20        
#define BUTTON_PIN_BITMASK 0x200000000
*/

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
int ipadd[5];

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

int h_offset,f_offset,ph_act_offset,inches_offset,temperatureF_offset,tds_offset;
int h_input=32,f_input=72,ph_act_input=7,inches_input=5,temperatureF_input=67,tds_value_input=83;


uint8_t LED1pin = 26;
bool LED1status = LOW;

uint8_t LED2pin = 27;
bool LED2status = LOW;

bool runRoutes = true;
String readFile(fs::FS &fs, const char * path){
  //Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  //Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
    //Serial.println(fileContent);
  }
  //Serial.println(fileContent);
  return fileContent;
}
String getip_address(fs::FS &fs, const char * path){
  //Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  //Serial.println("- read from file:");
  //String fileContent;
  int i=0;
  String A;
  String first;
  String second;
  String third;
  String fourth;
  
  while(file.available()){
    A= String((char)file.read());
    if(A=="."){
      i=i+1;
    }
    else{
      switch(i){
        case 0:
         first+=A;
        break;
        case 1:
        second+=A;
        break;
        case 2:
        third+=A;
        break;
        case 3:
        fourth+=A;
        break;
        
       }
    }


  }

  ipadd[0]=first.toInt();
  Serial.println("These are the IP numbers");
  Serial.println(ipadd[0]);
  ipadd[1]=second.toInt();
  Serial.println(ipadd[1]);
  ipadd[2]=third.toInt();
  Serial.println(ipadd[2]);
  ipadd[3]=fourth.toInt();
  Serial.println(ipadd[3]);


  


  //Serial.println(fileContent);
  return fourth;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  Serial.print("&*");
  Serial.print(message);
  Serial.println("&*");
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}
String processor(const String& var){
  //Serial.println(var);
  /*if(var == "inputString"){
    return readFile(SPIFFS, "/inputString.txt");
  }*/
  /*else if(var == "inputInt"){
    return readFile(SPIFFS, "/inputInt.txt");
  }*/
  if(var == "floatHigher"){
    return readFile(SPIFFS, "/floatHigher.txt");
  }
  else if (var=="floatLower"){
    return readFile(SPIFFS, "/floatLower.txt");
  }
  else if (var=="mqttUsername"){
  return readFile(SPIFFS, "/mqttUsername.txt");
}
  else if (var=="mqttPassword"){
  return readFile(SPIFFS, "/mqttPassword.txt");
}
else if (var=="mqttServer"){
  return readFile(SPIFFS, "/mqttServer.txt");
}
else if (var=="mqttPort"){
  return readFile(SPIFFS, "/mqttPort.txt");
}
else if (var=="timeStamp1"){
  return readFile(SPIFFS, "/timeStamp1.txt");
}
else if (var=="runTime1"){
  return readFile(SPIFFS, "/runTime1.txt");
}
else if (var=="lowerBound2"){
  return readFile(SPIFFS, "/lowerBound2.txt");
}
else if (var=="upperBound2"){
  return readFile(SPIFFS, "/upperBound2.txt");
}
else if (var=="timeStamp2"){
  return readFile(SPIFFS, "/timeStamp2.txt");
}
else if (var=="runTime2"){
  return readFile(SPIFFS, "/runTime2.txt");
}
else if (var=="lowerBound3"){
  return readFile(SPIFFS, "/lowerBound3.txt");
}
else if (var=="upperBound3"){
  return readFile(SPIFFS, "/upperBound3.txt");
}
else if (var=="timeStamp3"){
  return readFile(SPIFFS, "/timeStamp3.txt");
}
else if (var=="runTime3"){
  return readFile(SPIFFS, "/runTime3.txt");
}

else if (var=="lowerBound4"){
  return readFile(SPIFFS, "/lowerBound4.txt");
}
else if (var=="upperBound4"){
  return readFile(SPIFFS, "/upperBound4.txt");
}
else if (var=="timeStamp4"){
  return readFile(SPIFFS, "/timeStamp4.txt");
}
else if (var=="runTime4"){
  return readFile(SPIFFS, "/runTime4.txt");
}
else if (var=="lowerBound5"){
  return readFile(SPIFFS, "/lowerBound5.txt");
}
else if (var=="upperBound5"){
  return readFile(SPIFFS, "/upperBound5.txt");
}
else if (var=="timeStamp5"){
  return readFile(SPIFFS, "/timeStamp5.txt");
}
else if (var=="runTime5"){
  return readFile(SPIFFS, "/runTime5.txt");
}

else if (var=="lowerBound6"){
  return readFile(SPIFFS, "/lowerBound6.txt");
}
else if (var=="upperBound6"){
  return readFile(SPIFFS, "/upperBound6.txt");
}
else if (var=="timeStamp6"){
  return readFile(SPIFFS, "/timeStamp6.txt");
}
else if (var=="runTime6"){
  return readFile(SPIFFS, "/runTime6.txt");
}

  return String();
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}


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

// Replace the next variables with your SSID/Password combination
//const char* ssid = "ATT9JNx4NI";
//const char* password = "6fzv98tb2x?5";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
//const char* mqttServer = "192.168.1.66";
//const char* mqttServer;
//const int mqttPort = 1883;
//const char* mqttUser="user";
//const char* mqttUser= readFile(SPIFFS, "/mqttUsername.txt").c_str();
//const char* mqttPassword = "password";
//const char* mqttPassword="password";
//new

const char* MQTT_USERNAME = "mqttUsername";
const char* MQTT_PASSWORD = "mqttPassword";
const char* MQTT_SERVER = "mqttServer";
const char* MQTT_PORT = "mqttPort";


//const char* PARAM_INT = "inputInt";
const char* PARAM_HIGHER = "floatHigher";
const char* PARAM_LOWER = "floatLower";
const char* TIMESTAMP_1="timeStamp1";
const char* RUNTIME_1="runTime1";

const char* LOWERBOUND_2="lowerBound2";
const char* UPPERBOUND_2="upperBound2";
const char* TIMESTAMP_2="timeStamp2";
const char* RUNTIME_2="runTime2";

const char* LOWERBOUND_3="lowerBound3";
const char* UPPERBOUND_3="upperBound3";
const char* TIMESTAMP_3="timeStamp3";
const char* RUNTIME_3="runTime3";

const char* LOWERBOUND_4="lowerBound4";
const char* UPPERBOUND_4="upperBound4";
const char* TIMESTAMP_4="timeStamp4";
const char* RUNTIME_4="runTime4";

const char* LOWERBOUND_5="lowerBound5";
const char* UPPERBOUND_5="upperBound5";
const char* TIMESTAMP_5="timeStamp5";
const char* RUNTIME_5="runTime5";

const char* LOWERBOUND_6="lowerBound6";
const char* UPPERBOUND_6="upperBound6";
const char* TIMESTAMP_6="timeStamp6";
const char* RUNTIME_6="runTime6";


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
StaticJsonDocument<1024> timeStamp;
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

//Air Temp
String upperBound1 = "/floatHigher.txt";
String lowerBound1 = "/floatLower.txt";
String startTime1 = "/timeStamp1.txt";
String endTime1 = "/runTime1.txt";

//Air hum
String upperBound2 = "/upperbound2.txt";
String lowerBound2 = "/lowerbound2.txt";
String startTime2 = "/timeStamp2.txt";
String endTime2 = "/runTime2.txt";

//Water temp
String upperBound3 = "/upperbound3.txt";
String lowerBound3 = "/lowerbound3.txt";
String startTime3 = "/timeStamp3.txt";
String endTime3 = "/runTime3.txt";

//tds
String upperBound4 = "/upperbound4.txt";
String lowerBound4 = "/lowerbound4.txt";
String startTime4 = "/timeStamp4.txt";
String endTime4 = "/runTime4.txt";

//pH
String upperBound5 = "/upperbound5.txt";
String lowerBound5 = "/lowerbound5.txt";
String startTime5 = "/timeStamp5.txt";
String endTime5 = "/runTime5.txt";

//water level
String upperBound6= "/upperbound6.txt";
String lowerBound6 = "/lowerbound6.txt";

JsonObject airTemp = doc1.to<JsonObject>();
JsonObject airHum = doc2.to<JsonObject>();
JsonObject waterTemp = doc3.to<JsonObject>();
JsonObject tds = doc4.to<JsonObject>();
JsonObject pH = doc5.to<JsonObject>();
JsonObject waterLevel = doc6.to<JsonObject>();
JsonObject hourlyTimeStamp = timeStamp.to<JsonObject>();

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
JsonArray data2 = airHum.createNestedArray("dataset");
//int airHumCount = 0;
int totCount2 = 0;
JsonArray data3 = waterTemp.createNestedArray("dataset");
//int waterTempCount = 0;
int totCount3 = 0;
JsonArray data4 = tds.createNestedArray("dataset");
//int tdsCount = 0;
int totCount4 = 0;
JsonArray data5 = pH.createNestedArray("dataset");
//int tdsCount = 0;
int totCount5 = 0;
JsonArray data6 = waterLevel.createNestedArray("dataset");
//int tdsCount = 0;
int totCount6 = 0;
JsonArray data7 = hourlyTimeStamp.createNestedArray("dataset");

JsonArray dailyAirTempData = dailyAirTempAvg.createNestedArray("dataset");
JsonArray dailyAirHumData = dailyAirHumAvg.createNestedArray("dataset");
JsonArray dailyWaterTempData = dailyWaterTempAvg.createNestedArray("dataset");
JsonArray dailyTdsData = dailyTdsAvg.createNestedArray("dataset");
JsonArray dailypHData = dailypHAvg.createNestedArray("dataset");
JsonArray dailyWaterLevelData = dailyWaterLevelAvg.createNestedArray("dataset");
JsonArray dailyTimeStampArray = dailyTimeStamp.createNestedArray("dataset");

JsonArray weeklyAirTempData = weeklyAirTempAvg.createNestedArray("dataset");
JsonArray weeklyAirHumData = weeklyAirHumAvg.createNestedArray("dataset");
JsonArray weeklyWaterTempData = weeklyWaterTempAvg.createNestedArray("dataset");
JsonArray weeklyTdsData = weeklyTdsAvg.createNestedArray("dataset");
JsonArray weeklypHData = weeklypHAvg.createNestedArray("dataset");
JsonArray weeklyWaterLevelData = weeklyWaterLevelAvg.createNestedArray("dataset");
JsonArray weeklyTimeStampArray = weeklyTimeStamp.createNestedArray("dataset");

JsonArray monthlyAirTempData = monthlyAirTempAvg.createNestedArray("dataset");
JsonArray monthlyAirHumData = monthlyAirHumAvg.createNestedArray("dataset");
JsonArray monthlyWaterTempData = monthlyWaterTempAvg.createNestedArray("dataset");
JsonArray monthlyTdsData = monthlyTdsAvg.createNestedArray("dataset");
JsonArray monthlypHData = monthlypHAvg.createNestedArray("dataset");
JsonArray monthlyWaterLevelData = monthlyWaterLevelAvg.createNestedArray("dataset");
JsonArray monthlyTimeStampArray = monthlyTimeStamp.createNestedArray("dataset");

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

int errorflag1=0; // tds
int errorflag2=0; // waterLvl
int errorflag3=0; // Air 
int errorflag4=0; // waterTemp
int errorflag5=0; // pH

int dailyArray = 0;
int weeklyArray = 0;
int monthlyArray = 0;

int outlet1 = 12;
int outlet2 = 14;
int outlet3 = 27;
int outlet4 = 26;

const char* ntpServer = "pool.ntp.org";
const char* connect_mqtt(){
    String work;
    //work=s;
    work=readFile(SPIFFS,"/mqttServer.txt");
    //Serial.print(work);
  
  //Serial.print(mqttServer2);
  Serial.println("connect-");
  
  Serial.print(client.state());
  Serial.print("working fine");
  delay(2000);
  return work.c_str();
  
}

 int connect_port(){
   String porty;
   porty=readFile(SPIFFS,"/mqttPort.txt");
   int port_int=porty.toInt();

   return port_int;
   
}
const long  gmtOffset_sec = -14400;
const int   daylightOffset_sec = 0;

void printLocalTime(int time, int count, String filename){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  else{
    //Serial.println("Time obtained/n");
  }

  String s;
  File outfile = SPIFFS.open(filename, "w");

  switch (time) {
  case 1:
    char timeHourly[10];
    strftime(timeHourly,10, "%H:%M:%S", &timeinfo);
    s = timeHourly;
    data7[count] = s;
    break;
  case 2:
    char timeDaily[10];
    strftime(timeDaily,10, "%m/%d", &timeinfo);
    s = timeDaily;
    dailyTimeStampArray[count] = s;
    break;
  case 3:
    char timeWeekly[10];
    strftime(timeWeekly,10, "%m/%d", &timeinfo);
    s = timeWeekly;
    weeklyTimeStampArray[count] = s;
    break;
  case 4:
    char timeMonthly[10];
    strftime(timeMonthly,10, "%A %Y", &timeinfo);
    s = timeMonthly;
    monthlyTimeStampArray[count] = s;
    break;
  default:
    break;

}
  if(serializeJsonPretty(hourlyTimeStamp, outfile) == 0){
    Serial.println("Failed to write to file");
  }
  else
    //Serial.println("something written in file");
  outfile.close();
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
  File testfile = SPIFFS.open("/timeStamp.json", "r");
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

bool checkTime(String currentTime, String startTime, String endTime){
  if(currentTime > startTime){
    if(currentTime < endTime){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    return 0;
  }
}

bool checkSize(String filename){
  File testfile = SPIFFS.open(filename, "r");
  if(testfile){
    unsigned int fileSize = testfile.size();
    if (fileSize){
      return 1;
    }
    else{
      return 0;
    }
  }
  else{
    Serial.println("Failed to read to file");
  }
  testfile.close();
  return 0;
}

String getTime(){
  struct tm timeinfo;
  String s = "";
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return s;
  }
  else{
    //Serial.println("Time obtained/n");
  }

  char timeHourly[10];
  strftime(timeHourly,10, "%H:%M:%S", &timeinfo);
  s = timeHourly;
  return s;
}

String getTimeStamp(String filename){
  String timeStamp = "";
  char c;
  File testfile = SPIFFS.open(filename, "r");
  if(testfile){
    while(testfile.available()){
      c = testfile.read();
      timeStamp.concat(c);
    }
  }
  else{
    Serial.println("Failed to read to file");
  }
  testfile.close();
  //Serial.println(timeStamp);
  return timeStamp;
}

float getParameter(String filename){
  float value = 0;
  File testfile = SPIFFS.open(filename, "r");
  if(testfile){
    value = testfile.parseFloat();
  }
  else{
    Serial.println("Failed to read to file");
  }
  testfile.close();
  return value;
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

/*String processor(const String& var){
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
*/
inline const char * const BoolToString(bool b)
{
  return b ? "true" : "false";
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

  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(401);
  });
  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request){
   request->send(SPIFFS, "/logged_out.html", String(), false, processor);
  });
  server.on("/airTempError", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", BoolToString(errorflag3));
  });
  
  server.on("/waterTempError", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", BoolToString(errorflag4));
  });

  server.on("/waterLvlError", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", BoolToString(errorflag2));
  });
  server.on("/pHError", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", BoolToString(errorflag5));
  });
  server.on("/nutrientLvlError", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", BoolToString(errorflag1));
  });
 }
 
 void routesLoop() {
 // Route to load data1.json file 
 server.on("/data1.json", HTTP_GET, [](AsyncWebServerRequest *request){
    
    request->send(SPIFFS, "/data1.json", "application/json");
  });

   // Route to load data2.json file 
  server.on("/data2.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/data2.json", "application/json");
  });

   // Route to load data3.json file 
 server.on("/data3.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/data3.json", "application/json");
  });

   // Route to load data4.json file 
  server.on("/data4.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/data4.json", "application/json");
  });

 // Route to load data4.json file 
  server.on("/data5.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/data5.json", "application/json");
  });

  // Route to load data4.json file 
  server.on("/data6.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/data6.json", "application/json");
  });

    // Route to load daily1.json file 
  server.on("/daily1.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/daily1.json", "application/json");
  });

   // Route to load daily2.json file 
  server.on("/daily2.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/daily2.json", "application/json");
  });

   // Route to load daily3.json file 
  server.on("/daily3.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/daily3.json", "application/json");
  });

   // Route to load daily4.json file 
  server.on("/daily4.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/daily4.json", "application/json");
  });
 
  // Route to load daily5.json file 
  server.on("/daily5.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/daily5.json", "application/json");
  });

  // Route to load daily6.json file 
  server.on("/daily6.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/daily6.json", "application/json");
  });

  // Route to load weekly1.json file 
  server.on("/weekly1.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/weekly1.json", "application/json");
  });

  // Route to load weekly2.json file 
  server.on("/weekly2.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/weekly2.json", "application/json");
  });

  // Route to load weekly3.json file 
  server.on("/weekly3.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/weekly3.json", "application/json");
  });

  // Route to load weekly4.json file 
  server.on("/weekly4.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/weekly4.json", "application/json");
  });

  // Route to load weekly5.json file 
  server.on("/weekly5.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/weekly5.json", "application/json");
  });

  // Route to load weekly6.json file 
  server.on("/weekly6.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/weekly6.json", "application/json");
  });

  // Route to load monthly1.json file 
  server.on("/monthly1.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/monthly1.json", "application/json");
  });

  // Route to load monthly2.json file 
  server.on("/monthly2.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/monthly2.json", "application/json");
  });

  // Route to load monthly3.json file 
  server.on("/monthly3.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/monthly3.json", "application/json");
  });

  // Route to load monthly4.json file 
  server.on("/monthly4.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/monthly4.json", "application/json");
  });

    // Route to load monthly5.json file 
  server.on("/monthly5.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/monthly5.json", "application/json");
  });

   // Route to load monthly6.json file 
  server.on("/monthly6.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/monthly6.json", "application/json");
  });
  
 // Route to load timeStamp.json file 
  server.on("/monthly6.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/monthly6.json", "application/json");
  });

  // Route to load timeStamp.json file 
  server.on("/timeStamp.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/timeStamp.json", "application/json");
  });

  // Route to load timeStamp.json file 
  server.on("/dailyTimeStamp.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/dailyTimeStamp.json", "application/json");
  });

   // Route to load timeStamp.json file 
  server.on("/weeklyTimeStamp.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/weeklyTimeStamp.json", "application/json");
  });

   // Route to load timeStamp.json file 
  server.on("/monthlyTimeStamp.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/monthlyTimeStamp.json", "application/json");
  });

 }



void setup(){
  Serial.begin (115200);
  Serial.print("it is starting");
  /*
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  */
 //setup_wifi();

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
  pinMode(15,INPUT);////15
  pinMode(19,INPUT);
  attachInterrupt(digitalPinToInterrupt(5), button3ISR,RISING);
  attachInterrupt(digitalPinToInterrupt(15), buttonISR, RISING);////15
  attachInterrupt(digitalPinToInterrupt(19), button2ISR, RISING);

  /*
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);/////sleep DONT DELETE
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_15,1);
  */
  
  AsyncWiFiManager wifiManager(&server,&dns);
  wifiManager.autoConnect("AutoConnectAP");
    Serial.println("connected...yeey :)");
    


  

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

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

  printLocalTime(1, 0, jsonTimeStamp);
  printLocalTime(2, 0, jsonDailyTimeStamp);
  printLocalTime(3, 0, jsonWeeklyTimeStamp);
  printLocalTime(4, 0, jsonMonthlyTimeStamp);


  // Send web page with input fields to client
  /*server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", "/index.html", processor);
  });*/

  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET inputString value on <ESP_IP>/get?inputString=<inputMessage>
    /*if (request->hasParam(PARAM_STRING)) {
      inputMessage = request->getParam(PARAM_STRING)->value();
      writeFile(SPIFFS, "/inputString.txt", inputMessage.c_str());
    }
    // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    else if (request->hasParam(PARAM_INT)) {
      inputMessage = request->getParam(PARAM_INT)->value();
      writeFile(SPIFFS, "/inputInt.txt", inputMessage.c_str());
      Serial.println(inputMessage);
    }*/
    // GET inputFloat value on <ESP_IP>/get?inputFloat=<inputMessage>
    if (request->hasParam(PARAM_HIGHER)) {
      inputMessage = request->getParam(PARAM_HIGHER)->value();
      writeFile(SPIFFS, "/floatHigher.txt", inputMessage.c_str());
    }
    else if (request->hasParam(PARAM_LOWER)) {
      inputMessage = request->getParam(PARAM_LOWER)->value();
      writeFile(SPIFFS, "/floatLower.txt", inputMessage.c_str());
    }
    else if (request->hasParam(TIMESTAMP_1)) {
      inputMessage = request->getParam(TIMESTAMP_1)->value();
      writeFile(SPIFFS, "/timeStamp1.txt", inputMessage.c_str());
    }
    else if (request->hasParam(RUNTIME_1)) {
      inputMessage = request->getParam(RUNTIME_1)->value();
      writeFile(SPIFFS, "/runTime1.txt", inputMessage.c_str());
    }
    else if (request->hasParam(LOWERBOUND_2)) {
      inputMessage = request->getParam(LOWERBOUND_2)->value();
      writeFile(SPIFFS, "/lowerBound2.txt", inputMessage.c_str());
    }
    else if (request->hasParam(UPPERBOUND_2)) {
      inputMessage = request->getParam(UPPERBOUND_2)->value();
      writeFile(SPIFFS, "/upperBound2.txt", inputMessage.c_str());
    }
     else if (request->hasParam(TIMESTAMP_2)) {
      inputMessage = request->getParam(TIMESTAMP_2)->value();
      writeFile(SPIFFS, "/timeStamp2.txt", inputMessage.c_str());
    }
    else if (request->hasParam(RUNTIME_2)) {
      inputMessage = request->getParam(RUNTIME_2)->value();
      writeFile(SPIFFS, "/runTime2.txt", inputMessage.c_str());
    }
    else if (request->hasParam(LOWERBOUND_3)) {
      inputMessage = request->getParam(LOWERBOUND_3)->value();
      writeFile(SPIFFS, "/lowerBound3.txt", inputMessage.c_str());
    }
    else if (request->hasParam(UPPERBOUND_3)) {
      inputMessage = request->getParam(UPPERBOUND_3)->value();
      writeFile(SPIFFS, "/upperBound3.txt", inputMessage.c_str());
    }
     else if (request->hasParam(TIMESTAMP_3)) {
      inputMessage = request->getParam(TIMESTAMP_3)->value();
      writeFile(SPIFFS, "/timeStamp3.txt", inputMessage.c_str());
    }
    else if (request->hasParam(RUNTIME_3)) {
      inputMessage = request->getParam(RUNTIME_3)->value();
      writeFile(SPIFFS, "/runTime3.txt", inputMessage.c_str());
    }

    //4
    else if (request->hasParam(LOWERBOUND_4)) {
      inputMessage = request->getParam(LOWERBOUND_4)->value();
      writeFile(SPIFFS, "/lowerBound4.txt", inputMessage.c_str());
    }
    else if (request->hasParam(UPPERBOUND_4)) {
      inputMessage = request->getParam(UPPERBOUND_4)->value();
      writeFile(SPIFFS, "/upperBound4.txt", inputMessage.c_str());
    }
     else if (request->hasParam(TIMESTAMP_4)) {
      inputMessage = request->getParam(TIMESTAMP_4)->value();
      writeFile(SPIFFS, "/timeStamp4.txt", inputMessage.c_str());
    }
    else if (request->hasParam(RUNTIME_4)) {
      inputMessage = request->getParam(RUNTIME_4)->value();
      writeFile(SPIFFS, "/runTime4.txt", inputMessage.c_str());
    }

    else if (request->hasParam(LOWERBOUND_5)) {
      inputMessage = request->getParam(LOWERBOUND_5)->value();
      writeFile(SPIFFS, "/lowerBound5.txt", inputMessage.c_str());
    }
    else if (request->hasParam(UPPERBOUND_5)) {
      inputMessage = request->getParam(UPPERBOUND_5)->value();
      writeFile(SPIFFS, "/upperBound5.txt", inputMessage.c_str());
    }
     else if (request->hasParam(TIMESTAMP_5)) {
      inputMessage = request->getParam(TIMESTAMP_5)->value();
      writeFile(SPIFFS, "/timeStamp5.txt", inputMessage.c_str());
    }
    else if (request->hasParam(RUNTIME_5)) {
      inputMessage = request->getParam(RUNTIME_5)->value();
      writeFile(SPIFFS, "/runTime5.txt", inputMessage.c_str());
    }

    else if (request->hasParam(LOWERBOUND_6)) {
      inputMessage = request->getParam(LOWERBOUND_6)->value();
      writeFile(SPIFFS, "/lowerBound6.txt", inputMessage.c_str());
    }
    else if (request->hasParam(UPPERBOUND_6)) {
      inputMessage = request->getParam(UPPERBOUND_6)->value();
      writeFile(SPIFFS, "/upperBound6.txt", inputMessage.c_str());
    }
     else if (request->hasParam(TIMESTAMP_6)) {
      inputMessage = request->getParam(TIMESTAMP_6)->value();
      writeFile(SPIFFS, "/timeStamp6.txt", inputMessage.c_str());
    }
    else if (request->hasParam(RUNTIME_6)) {
      inputMessage = request->getParam(RUNTIME_6)->value();
      writeFile(SPIFFS, "/runTime6.txt", inputMessage.c_str());
    }
    else if (request->hasParam(MQTT_USERNAME)) {
      inputMessage = request->getParam(MQTT_USERNAME)->value();
      writeFile(SPIFFS, "/mqttUsername.txt", inputMessage.c_str());
    }
    else if (request->hasParam(MQTT_PASSWORD)) {
      inputMessage = request->getParam(MQTT_PASSWORD)->value();
      writeFile(SPIFFS, "/mqttPassword.txt", inputMessage.c_str());
    }
    else if (request->hasParam(MQTT_SERVER)) {
      inputMessage = request->getParam(MQTT_SERVER)->value();
      writeFile(SPIFFS, "/mqttServer.txt", inputMessage.c_str());
    }
    else if (request->hasParam(MQTT_PORT)) {
      inputMessage = request->getParam(MQTT_PORT)->value();
      writeFile(SPIFFS, "/mqttPort.txt", inputMessage.c_str());
    }
    
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);
  });
  server.onNotFound(notFound);
  getip_address(SPIFFS,"/mqttServer.txt");
  //int a=192;
  //int b= 168;
  //int c=1;
  //int d=66;
  IPAddress cool(ipadd[0],ipadd[1],ipadd[2],ipadd[3]);
  //cool = connect_mqtt();
  int beans= connect_port();
  client.setServer(cool,beans);
  
 
  
  
  //client.setCallback(callback);

  Serial.println(WiFi.localIP());
  routes();
  server.begin();


}
void reconnect() {
  // Loop until we're reconnected

  
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),readFile(SPIFFS, "/mqttUsername.txt").c_str(),readFile(SPIFFS, "/mqttPassword.txt").c_str())) {
      Serial.println("connected");
      
      client.subscribe("esp32/output");
    } else {
      Serial.print(readFile(SPIFFS, "/mqttUsername.txt").c_str());
      Serial.print(readFile(SPIFFS, "/mqttPassword.txt").c_str());
      Serial.print(readFile(SPIFFS, "/mqttServer.txt").c_str());
      Serial.print(readFile(SPIFFS, "/mqttPort.txt").c_str());
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
}
 int clear2=0; 
 int errorflag=0;

void displayWaterTemp(float temperatureF){
  lcd.setCursor(0,0);//water temp display
  if(checkSize(lowerBound3) && checkSize(upperBound3)){
    waterTempLowParam = getParameter(lowerBound3);
    waterTempHighParam = getParameter(upperBound3);
  }
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
  if(checkSize(lowerBound2) && checkSize(upperBound2)){
    airHumLowParam = getParameter(lowerBound2);
    airHumHighParam = getParameter(upperBound2);
  }
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
  if(checkSize(lowerBound1) && checkSize(upperBound1)){
    airTempLowParam = getParameter(lowerBound1);
    airTempHighParam = getParameter(upperBound1);
  }
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
  if(checkSize(lowerBound6) && checkSize(upperBound6)){
    sonarLowParam = getParameter(lowerBound6);
    sonarHighParam = getParameter(upperBound6);
  }
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
  if(checkSize(lowerBound4) && checkSize(upperBound4)){
    tdslowParam = getParameter(lowerBound4);
    tdshighParam = getParameter(upperBound4);
  }
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
  if(checkSize(lowerBound5) && checkSize(upperBound5)){
    phlowParam = getParameter(lowerBound5);
    phhighParam = getParameter(upperBound5);
  }
  if(ph_act<phlowParam&&ph_act>0){
    lcd.print("Error pH is Low ");
    clear2=1;
  }
  else if(ph_act>phhighParam){
    lcd.print("Error pH is High");
    clear2=1;
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
 array1=analogRead(36);////// 0 ph sensor
 array2=analogRead(36);//0
 array3=analogRead(36);//0
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

/*
 if(totCount1 > 5){
   totCount2++;
 }
 */

  //store live and historical data
 if(totCount1 > 2){
  if(airTempCount == 24){
    airTempCount = 0;
  }
  
  float fakeAirTemp = 65 + rand() % (( 85 + 1 ) -65);
  float fakeAirHumid = 30 + rand() % (( 80 + 1 ) -30);
  float fakeWaterTemp = 65 + rand() % (( 85 + 1 ) -65);
  float fakeTDS = 500 + rand() % (( 500 + 1 ) - 200);
  float fakepH = 0 + rand() % (( 14 + 1 ) - 0);
  float fakeWaterLevel = 0 + rand() % (( 10 + 1 ) - 0);

  store_data(fakeAirTemp, airTemp, data1, airTempCount, jsonData1);
  //store_data(avgAirTemp, airTemp, data1, airTempCount, jsonData1);
  store_data(fakeAirHumid, airHum, data2, airTempCount, jsonData2);
  //store_data(avgAirHum, airHum, data2, airTempCount, jsonData2);
  store_data(fakeWaterTemp, waterTemp, data3, airTempCount, jsonData3);
  //store_data(avgWaterTemp, waterTemp, data3, airTempCount, jsonData3);
  store_data(fakeTDS, tds, data4, airTempCount, jsonData4);
  //store_data(avgTDS, tds, data4, airTempCount, jsonData4);
  store_data(fakepH, pH, data5, airTempCount, jsonData5);
  //store_data(avgpH, pH, data5, airTempCount, jsonData5);
  store_data(fakeWaterLevel, waterLevel, data6, airTempCount, jsonData6);
  //store_data(avgWaterLevel, waterLevel, data6, airTempCount, jsonData6);
  printLocalTime(1, airTempCount, jsonTimeStamp);
  airTempCount++;
  totCount1 = 0;
  avgCount = 1;
  //serializeJsonPretty(doc2, Serial);
  //serializeJsonPretty(timeStamp, Serial);
  //printJSON();
  dailyCount++;
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
   printLocalTime(2, dailyArray, jsonDailyTimeStamp);
   dailyArray++;
   //serializeJsonPretty(daily1, Serial);
   //serializeJsonPretty(daily2, Serial);
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
   printLocalTime(3, weeklyArray, jsonWeeklyTimeStamp);
   weeklyArray++;
   weeklyCount = 0;
   monthlyCount++;
 }

 if(monthlyCount == 4){
   if(monthlyArray == 12){
     monthlyArray = 0;
   }
   store_daily(weeklyAirTempData, monthlyAirTempData, monthlyAirTempAvg, jsonMonthly1, 4, monthlyArray);
   store_daily(weeklyAirHumData, monthlyAirHumData, monthlyAirHumAvg, jsonMonthly2, 4, monthlyArray);
   store_daily(weeklyWaterTempData, monthlyWaterTempData, monthlyWaterTempAvg, jsonMonthly3, 4, monthlyArray);
   store_daily(weeklyTdsData, monthlyTdsData, monthlyTdsAvg, jsonMonthly4, 4, monthlyArray);
   store_daily(weeklypHData, monthlypHData, monthlypHAvg, jsonMonthly5, 4, monthlyArray);
   store_daily(weeklyWaterLevelData, monthlyWaterLevelData, monthlyWaterLevelAvg, jsonMonthly6, 4, monthlyArray);
   printLocalTime(4, monthlyArray, jsonMonthlyTimeStamp);
   monthlyArray++;
   monthlyCount = 0;
 }


if(runRoutes==true){
  routesLoop();
  runRoutes = false;
  //printf("run routes first");
}

if(checkSize(startTime1) && checkSize(endTime1)){
  String curr = getTime();
  String s1 = getTimeStamp(startTime1);
  String e1 = getTimeStamp(endTime1);
  if(checkTime(curr, s1, e1)){
    digitalWrite(outlet1, 1);
    //Serial.println("Turn on outlet");
  }
  else{
    digitalWrite(outlet1, 0);
    //Serial.println("Turn off outlet");
  }
}

if(checkSize(startTime3) && checkSize(endTime3)){
  String curr2 = getTime();
  String s2 = getTimeStamp(startTime3);
  String e2 = getTimeStamp(endTime3);
  if(checkTime(curr2, s2, e2)){
    digitalWrite(outlet2, 1);
    //Serial.println("Turn on outlet");
  }
  else{
    digitalWrite(outlet2, 0);
    //Serial.println("Turn off outlet");
  }
}

if(checkSize(startTime4) && checkSize(endTime4)){
  String curr3 = getTime();
  String s3 = getTimeStamp(startTime4);
  String e3 = getTimeStamp(endTime4);
  if(checkTime(curr3, s3, e3)){
    digitalWrite(outlet3, 1);
    //Serial.println("Turn on outlet");
  }
  else{
    digitalWrite(outlet3, 0);
    //Serial.println("Turn off outlet");
  }
}

if(checkSize(startTime5) && checkSize(endTime5)){
  String curr4 = getTime();
  String s4 = getTimeStamp(startTime5);
  String e4 = getTimeStamp(endTime5);
  if(checkTime(curr4, s4, e4)){
    digitalWrite(outlet4, 1);
  }
  else{
    digitalWrite(outlet4, 0);
  }
}
//else
//printf("routes completed");
  


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
      if(errorflag){
        lcd.setCursor(0,0);
        lcd.print("Critical Error  ");
        lcd.setCursor(0,1);
        lcd.print("Check Sensors   ");
      }
      else{
        lcd.setCursor(0,0);
        lcd.print("Hydroponics     ");
      }
      break;
    case 1:
      if(errorflag3){
        lcd.setCursor(0,0);
        lcd.print("Critical Error  ");
        lcd.setCursor(0,1);
        lcd.print("Check Air   ");
      }
      else{
        displayDHT(h+h_offset,f+f_offset);
      }
      break;
    case 2:
      if(errorflag4){
        lcd.setCursor(0,0);
        lcd.print("Critical Error  ");
        lcd.setCursor(0,1);
        lcd.print("Check WaterT   ");
      }
      else{
        displayWaterTemp(temperatureF+temperatureF_offset);
      }
      break;
    case 3:
      if(errorflag1){
        lcd.setCursor(0,0);
        lcd.print("Critical Error  ");
        lcd.setCursor(0,1);
        lcd.print("Check TDS   ");
      }
      else{
        displayTDS(tdsValue+tds_offset);
      }
      break;
    case 4:
      if(errorflag2){
        lcd.setCursor(0,0);
        lcd.print("Critical Error  ");
        lcd.setCursor(0,1);
        lcd.print("Check WaterL   ");
      }
      else{
        displaySonar(inches+inches_offset);
      }
      break;
    case 5:
      if(errorflag5){
        lcd.setCursor(0,0);
        lcd.print("Critical Error  ");
        lcd.setCursor(0,1);
        lcd.print("Check pH   ");
      }
      else{
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