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
#include <Ticker.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_INA219.h> 

ESP8266WiFiMulti WiFiMulti;
Adafruit_INA219 sensor219; 


#define SAMPLING_PERIOD 1500 //ms
#define RPM_MAX_LIMIT 240 //

#define RPM_IN_PIN 0 //(physical pin D3)

/* 
 *  My functions
 */
void rpmFallEdgeISR(void);
void convertValues(void);
void preparePostString(void);
/* 
 *  My variables
 */
const char* ssid     = "wificko_n";
const char* password = "subaruimpreza";

volatile long t_end, t_diff, tim;
volatile int updated;

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

  voltage_V = 0;
  current_mA = 0;
  power_mW = 0;
  rpm = 0;

  pinMode(RPM_IN_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  t_end = 0;
  t_diff = 0;
  tim = 0;
  updated = 0;

  timer1_enable(TIM_DIV256 , TIM_EDGE  , TIM_LOOP   );
  timer1_write(8388607);
  
  attachInterrupt(digitalPinToInterrupt(RPM_IN_PIN), rpmFallEdgeISR, FALLING);
}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    if((rpm > 0) && (rpm < RPM_MAX_LIMIT)){
      convertValues();
      preparePostString();
      sendHttpPost();
      rpm = 0;
    }
  }
  else
  {
    Serial.printf("Waiting for WiFi connection");
    delay(5000);
  }

  if(updated == 1)
  {
    rpm = 60 /((t_diff)*0.0000032);
    updated = 0;

  }
  
  delay(SAMPLING_PERIOD);
}

void rpmFallEdgeISR(void){

  tim = timer1_read();
  t_diff = t_end - tim;
  t_end = tim;  
  digitalWrite(LED_BUILTIN,!(digitalRead(LED_BUILTIN))); 
  updated = 1;
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
