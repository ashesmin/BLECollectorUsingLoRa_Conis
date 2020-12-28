 
#include <time.h>
#include <stdint.h>
#include <iostream>

typedef struct {
    
	char expId[10];
	int  interval;
	int  totalPacketNum;
    int  slotLength;   
    time_t expStartTime;
    uint8_t srcAddr;
    uint8_t dstAddr;
    int sendSquence;
    int endNodeNum;
} loraTrafficConfig;

typedef struct {
    
    uint8_t mode;
 	char power;
 	uint32_t channel;
} loraParameterConfig;


typedef struct {

} wifiTrafficConfig;

typedef struct {

} wifiParameterConfig;


typedef struct {
    
    
} wifiConfiguration; 

typedef struct {
    
    loraTrafficConfig loraTraffic;
    loraParameterConfig loraParameter;
    
} loraConfiguration;


void setLoRaExpId(loraConfiguration *, char*);
void setLoRaInterval(loraConfiguration *, int);
void setLoRaTotalPacketNum(loraConfiguration *, int);
void setLoRaSlotLength(loraConfiguration *, int);
void setLoRaStartTime(loraConfiguration *, std::string); 
void setLoRaDstAddress(loraConfiguration *, uint8_t);
void setLoRaSrcAddress(loraConfiguration *, uint8_t);
void setLoRaSendSequence(loraConfiguration *, int);
void setLoRaEndNodeNum(loraConfiguration *, int);

void setLoRaMode(loraConfiguration *, uint8_t);
void setLoRaPower(loraConfiguration *, char);
void setLoRaChannel(loraConfiguration *, uint32_t);


char* getLoRaExpId(loraConfiguration * );
int getLoRaInterval(loraConfiguration * );
int getLoRaTotalPacketNum(loraConfiguration * );
int getLoRaSlotLength(loraConfiguration * );
time_t getLoRaStartTime(loraConfiguration * ); 
uint8_t getLoRaSrcAddress(loraConfiguration * );
uint8_t getLoRaDstAddress(loraConfiguration * );
int getLoRaSendSequence(loraConfiguration *);
int getLoRaEndNodeNum(loraConfiguration *);

uint8_t getLoRaMode(loraConfiguration * );
char getLoRaPower(loraConfiguration * );
uint32_t getLoRaChannel(loraConfiguration * );