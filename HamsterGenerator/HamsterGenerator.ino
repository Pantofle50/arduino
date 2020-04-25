/**
   HamsterGenerator.ino

    Created on: 24.05.2015

*/
/*
 * 
 * Board: Wemos D1 mini (LOLIN Wemos D1 R2 & mini)
 * 80MHz 4MB (1M SPIFFS)
 * 
 * 
 * 
 * 
 * 
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_INA219.h> 

ESP8266WiFiMulti WiFiMulti;
Adafruit_INA219 sensor219; 


#define SAMPLING_PERIOD 1500 //ms
/* 
 *  My functions
 */
void convertValues(void);
void preparePostString(void);

/* 
 *  My variables
 */
const char* ssid     = "wificko_n";
const char* password = "subaruimpreza";

float voltage_V;
float current_mA;
float power_mW;
float rpm;

char post_str[80];

char volt_str[10];
char curr_str[10];
char pow_str[10];
char rpm_str[10];
 
void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

  voltage_V=12.23;
  current_mA=186.4;
  power_mW=1256.25;
  rpm=125.5;

  convertValues();
  preparePostString();
  sendHttpPost();
  
  }
  else
  {
    Serial.printf("Waiting for WiFi connection");
    delay(5000);
  }

  delay(SAMPLING_PERIOD);
}

void convertValues(void){

  dtostrf(voltage_V, 4, 2, volt_str);  
  dtostrf(current_mA, 4, 2, curr_str);  
  dtostrf(power_mW, 4, 2, pow_str); 
  dtostrf(rpm, 4, 2, rpm_str); 
  
}

void preparePostString(void){

  char vol_par[] = "VOL=";
  char cur_par[] = "&CUR=";
  char pow_par[] = "&POW=";
  char rpm_par[] = "&RPM=";

  post_str[0] = '\0';
  strcat(post_str,vol_par);
  strcat(post_str,volt_str);
  strcat(post_str,cur_par);
  strcat(post_str,curr_str);
  strcat(post_str,pow_par);
  strcat(post_str,pow_str);
  strcat(post_str,rpm_par);
  strcat(post_str,rpm_str);
  
  Serial.println(post_str);

}

void sendHttpPost(void){

 HTTPClient http;
 
 http.begin("http://albre.jemivedro.cz/read_hamster.php");  
 http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 

 int httpResponseCode = http.POST(post_str);   
 
 if(httpResponseCode>0)
 {

  String response = http.getString();                       
  Serial.println(httpResponseCode);   
  Serial.println(response);           

 }
 else{

  Serial.print("Error on sending POST: ");
  Serial.println(httpResponseCode);

 }

 http.end(); 
}
