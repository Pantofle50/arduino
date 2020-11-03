
#include "DataTransmitter.h"
#include "GlobalDefs.h"
#include "DataLogger.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

 HTTPClient http;

char post_str[80];

char volt_str[10];
char curr_str[10];
char pow_str[10];
char rpm_str[10];


void convertValues(Record_t* rec){

  dtostrf(rec->voltage_V, 4, 2, volt_str);  
  dtostrf(rec->current_mA, 4, 2, curr_str);  
  dtostrf(rec->power_mW, 4, 2, pow_str); 
  dtostrf(rec->rpm, 4, 2, rpm_str); 
  
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
  #ifdef DEBUG_COM
  Serial.println(post_str);
  #endif
}

void sendHttpPost(void){
 
 http.begin("http://albre.jemivedro.cz/read_hamster.php");  
 http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 

 int httpResponseCode = http.POST(post_str);   
 
 if(httpResponseCode>0)
 {

  String response = http.getString(); 
  #ifdef DEBUG_COM                      
  Serial.println(httpResponseCode);   
  Serial.println(response);           
  #endif
 }
 else{
  #ifdef DEBUG_COM  
  Serial.print("Error on sending POST: ");
  Serial.println(httpResponseCode);
  #endif

  http.end(); 
 }
}
