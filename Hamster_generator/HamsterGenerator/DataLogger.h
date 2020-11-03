#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include <Arduino.h>

typedef struct {
  float voltage_V;
  float current_mA;
  float power_mW;
  float rpm;
} Record_t;

#define AVE_SAMPLE_LENGTH    3  //samples

#define TIMEOUT_MS    2000  //ms
#define TIMEOUT_MULT  2     //ms
#define TIMEOUT_LIMIT 60000 //ms


void logInit(void);
void logSaveRecord(Record_t* rec);
void logGetRecord(Record_t* rec);

#endif //DATA_LOGGER_H
