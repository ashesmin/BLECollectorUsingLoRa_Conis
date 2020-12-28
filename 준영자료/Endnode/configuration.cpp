#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "configuration.h"

void setLoRaExpId(loraConfiguration *configLoRaData, char* expid)
{
	memcpy(configLoRaData->loraTraffic.expId, expid, 10);
}
void setLoRaInterval(loraConfiguration *configLoRaData, int interval)
{
	configLoRaData->loraTraffic.interval =  interval ;
}
void setLoRaTotalPacketNum(loraConfiguration *configLoRaData, int totalPacketNum)
{
	configLoRaData->loraTraffic.totalPacketNum =  totalPacketNum ;
}
void setLoRaSlotLength(loraConfiguration *configLoRaData, int slotLength)
{
    configLoRaData->loraTraffic.slotLength =  slotLength ;
}
void setLoRaStartTime(loraConfiguration *configLoRaData, std::string expStartTime)
{
    time_t time;
    tm* timeInfo = localtime(&time);
    char cstr[10];
    strcpy(cstr, expStartTime.substr(0,4).c_str());
    timeInfo->tm_year = atoi(cstr)-1900;
	strcpy(cstr, expStartTime.substr(4,2).c_str());
    timeInfo->tm_mon = atoi(cstr)-1;
    strcpy(cstr, expStartTime.substr(6,2).c_str());
    timeInfo->tm_mday = atoi(cstr);
    strcpy(cstr, expStartTime.substr(9,2).c_str());
    timeInfo->tm_hour = atoi(cstr);
    strcpy(cstr, expStartTime.substr(12,2).c_str());
    timeInfo->tm_min = atoi(cstr);
    strcpy(cstr, expStartTime.substr(15,2).c_str());
    timeInfo->tm_sec = atoi(cstr);
    timeInfo->tm_isdst = 0;

    configLoRaData->loraTraffic.expStartTime=mktime(timeInfo);
}
void setLoRaSrcAddress(loraConfiguration *configLoRaData, uint8_t srcAddress)
{
	configLoRaData->loraTraffic.srcAddr =  srcAddress ;
}
void setLoRaDstAddress(loraConfiguration *configLoRaData, uint8_t dstAddress)
{
	configLoRaData->loraTraffic.dstAddr =  dstAddress ;
}
void setLoRaSendSequence(loraConfiguration *configLoRaData, int sendSquence)
{
	configLoRaData->loraTraffic.sendSquence =  sendSquence ;
}
void setLoRaEndNodeNum(loraConfiguration *configLoRaData, int endnodeNum)
{
	configLoRaData->loraTraffic.endNodeNum =  endnodeNum ;
}



void setLoRaMode(loraConfiguration *configLoRaData, uint8_t mode)
{
	configLoRaData->loraParameter.mode =  mode ;
}
void setLoRaPower(loraConfiguration *configLoRaData, char power)
{
 	configLoRaData->loraParameter.power =  power ;
}	
void setLoRaChannel(loraConfiguration *configLoRaData, uint32_t channel)
{
	 configLoRaData->loraParameter.channel =  channel ;
}

char* getLoRaExpId(loraConfiguration *configLoRaData )
{
	return configLoRaData->loraTraffic.expId;
}
int getLoRaInterval(loraConfiguration *configLoRaData )
{
	return configLoRaData->loraTraffic.interval;
}
int getLoRaTotalPacketNum(loraConfiguration *configLoRaData )
{
	return configLoRaData->loraTraffic.totalPacketNum;
}
int getLoRaSlotLength(loraConfiguration *configLoRaData )
{
	return configLoRaData->loraTraffic.slotLength;
}
time_t getLoRaStartTime(loraConfiguration *configLoRaData )
{
	return  configLoRaData->loraTraffic.expStartTime;
}
uint8_t getLoRaSrcAddress(loraConfiguration *configLoRaData )
{
	return configLoRaData->loraTraffic.srcAddr;
}
uint8_t getLoRaDstAddress(loraConfiguration *configLoRaData )
{
	return configLoRaData->loraTraffic.dstAddr;
}
int getLoRaSendSequence(loraConfiguration *configLoRaData)
{
	return configLoRaData->loraTraffic.sendSquence;
}
int getLoRaEndNodeNum(loraConfiguration *configLoRaData)
{
	return configLoRaData->loraTraffic.endNodeNum;
}

uint8_t getLoRaMode(loraConfiguration *configLoRaData )
{
	return configLoRaData->loraParameter.mode;
}
char getLoRaPower(loraConfiguration *configLoRaData )
{
	return configLoRaData->loraParameter.power;
}
uint32_t getLoRaChannel(loraConfiguration *configLoRaData )
{
	return configLoRaData->loraParameter.channel;
}

   
    