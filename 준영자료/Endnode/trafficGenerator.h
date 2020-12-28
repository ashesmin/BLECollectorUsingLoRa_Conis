#include <sys/time.h>
#include <signal.h>
#include "lora.h"
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include "loraLog.h"

timer_t createTimer(int);
void installSigHandler(int, void(*handler)(int));
void setTimer(timer_t, int);
void configPacket(MyPacket *);
void sigHandler(int);
void trafficGenerate(int, loraConfiguration *);
