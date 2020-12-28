
#include <iostream>
#include "configuration.h"

int readLoRaExpNum(char *);
int readLoRaConfig(char *, loraConfiguration ***, int *);
void waitUntilLoRaStartTime(loraConfiguration *);
