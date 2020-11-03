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

#include "DataTransmitter.h"
#include "GlobalDefs.h"
#include "DataLogger.h"

ESP8266WiFiMulti WiFiMulti;

/* 
 *  My functions
 */
void rpmFallEdgeISR(void);

/* 
 *  My variables
 */

volatile long t_end, t_diff, tim;
volatile int updated;

Record_t temp_record;
 
void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  //Temporary record variable init
  temp_record.voltage_V = 0;
  temp_record.current_mA = 0;
  temp_record.power_mW = 0;
  temp_record.rpm = 0;

  //GPIO init
  pinMode(RPM_IN_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(NETNAME, PWD);

  //Timer variables init
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

//    if((current_record.rpm > 0) && (current_record.rpm < RPM_MAX_LIMIT)){
//      //convertValues(&current_record);
//      preparePostString();
//      sendHttpPost();
//      //current_record.rpm = 0;
//    }
  }
  else
  {
    Serial.printf("Waiting for WiFi connection");
    delay(5000);
  }

  if(updated == 1)
  {
    temp_record.rpm = 60 /((t_diff)*0.0000032);
    temp_record.rpm = temp_record.rpm / POLE_PAIRS;

    if((temp_record.rpm > 0) && (temp_record.rpm < RPM_MAX_LIMIT)){
   
      //TODO update the rest
      //temp_record.voltage_V = 0;
      //temp_record.current_mA = 0;
      //temp_record.power_mW = 0;
  
      logSaveRecord(&temp_record);
  
      //Send the data!
      logGetRecord(&temp_record);
      convertValues(&temp_record);
      preparePostString();
      sendHttpPost();

    }
    else
    {
      //Rpm value out of limits - ignore it
    }

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
