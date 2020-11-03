#ifndef DATA_TRANSMITTER_H
#define DATA_TRANSMITTER_H

#include <Arduino.h>
#include "DataLogger.h"

void convertValues(Record_t* rec);
void preparePostString(void);
void sendHttpPost(void);

#endif //DATA_TRANSMITTER_H
