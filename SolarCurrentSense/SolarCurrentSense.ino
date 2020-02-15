/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_INA219.h> // You will need to download this library

Adafruit_INA219 sensor219; 

const char* ssid     = "wificko_n";
const char* password = "subaruimpreza";

HTTPClient http;

float busVoltage_V = 0;
float current_mA = 0; 
float power_mW = 0;

char volt_str[10];
char curr_str[10];
char pow_str[10];

char post_str[60];
char vol_par[] = "VOL=";
char cur_par[] = "&CUR=";
char pow_par[] = "&POW=";

void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    sensor219.begin();
    sensor219.setCalibration_32V_1A();

     
}

int value = 0;

void loop()
{
  
  //delay(5000);
  busVoltage_V = sensor219.getBusVoltage_V();
  current_mA = (sensor219.getShuntVoltage_mV())/0.3986;
  power_mW = busVoltage_V * current_mA; 

  dtostrf(busVoltage_V, 4, 2, volt_str);  //4 is mininum width, 6 is precision
  dtostrf(current_mA, 4, 2, curr_str);  //4 is mininum width, 6 is precision
  dtostrf(power_mW, 4, 2, pow_str);  //4 is mininum width, 6 is precision
  
  Serial.print("Bus Voltage:   "); 
  Serial.print(busVoltage_V); 
  Serial.println(" V");  
  Serial.println(volt_str);  
  
  Serial.print("Current:       "); 
  Serial.print(current_mA); 
  Serial.println(" mA");
  Serial.println(curr_str); 

  Serial.print("Power:         "); 
  Serial.print(power_mW); 
  Serial.println(" mW");  
  Serial.println(pow_str); 
  
  Serial.println(""); 
  
  post_str[0] = '\0';
  strcat(post_str,vol_par);
  strcat(post_str,volt_str);
  strcat(post_str,cur_par);
  strcat(post_str,curr_str);
  strcat(post_str,pow_par);
  strcat(post_str,pow_str);
  Serial.println(post_str);
 
   http.begin("http://albre.jemivedro.cz/read_solar_power.php");  
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
delay(5*60*1000);

}
