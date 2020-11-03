#include "DataLogger.h"
#include <Arduino.h>

static Record_t records[AVE_SAMPLE_LENGTH];
static int free_record_ptr;

void logInit(void){

  for(int i = 0; i<AVE_SAMPLE_LENGTH; i++)
  {
    records[i].voltage_V = 0;
    records[i].current_mA = 0;
    records[i].power_mW = 0;
    records[i].rpm = 0;
  }

  free_record_ptr = 0;
}


void logSaveRecord(Record_t* rec){

  records[free_record_ptr].voltage_V = rec->voltage_V;
  records[free_record_ptr].current_mA = rec->current_mA;
  records[free_record_ptr].power_mW = rec->power_mW;
  records[free_record_ptr].rpm = rec->rpm;

  free_record_ptr++;

  if (free_record_ptr >= AVE_SAMPLE_LENGTH)
  {
    free_record_ptr = 0;
  }
  
}

void logGetRecord(Record_t* rec){

  //Take average of all saved records and return it
  for(int i = 0; i<AVE_SAMPLE_LENGTH; i++)
  {
    rec->voltage_V = rec->voltage_V + records[i].voltage_V;
    rec->current_mA = rec->current_mA + records[i].current_mA;
    rec->power_mW = rec->power_mW + records[i].power_mW;
    rec->rpm = rec->rpm + records[i].rpm;
  }

    rec->voltage_V = rec->voltage_V / AVE_SAMPLE_LENGTH;
    rec->current_mA = rec->current_mA / AVE_SAMPLE_LENGTH;
    rec->power_mW = rec->power_mW / AVE_SAMPLE_LENGTH;
    rec->rpm = rec->rpm / AVE_SAMPLE_LENGTH;
}
