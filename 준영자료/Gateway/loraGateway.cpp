
#include "loraGateway.h"
loraConfiguration** configData;
char globalExpId[10];
int milli = 1000;

void loraGatewayExp()
{
 	char file_name[] = {"./usermode.cfg"};
 	int i, flag, expNum;
	int interval;

	flag=readLoRaConfig(file_name, &configData, &expNum);
	if(flag ==-1) {
		printf("Fail to read file .... \n");
	}

	for(i = 0; i < expNum; i++) {
		initLoRa(configData[i]);
		waitUntilLoRaStartTime(configData[i]);
//		initLoRa(configData[i]);
		memcpy(globalExpId, getLoRaExpId(configData[i]), 10);

		cout << "Usermode Global ExpID::" << globalExpId << endl;

		std::cout << "Main:: ----------------" << std::endl;
        
		int totalLoRaReceiveLength = (getLoRaEndNodeNum(configData[i])*(getLoRaSlotLength(configData[i])+getLoRaInterval(configData[i])))*getLoRaTotalPacketNum(configData[i]);
        printf("totalLoRaReceiveLength : %d(sec)\n",totalLoRaReceiveLength/milli); 
        printf("receiveWatitTime: %d(ms)\n",getLoRaReceiveWaitTime(configData[i])); 
   
	    unsigned long clkStart = millis();
	 
		while(((unsigned long)millis()-clkStart)<totalLoRaReceiveLength)
		{
		
		MyPacket *packet;

		packet = receiveLoRa(getLoRaReceiveWaitTime(configData[i]));
		//cout << loraJSONString(packet) << endl;
		loraJSONString(packet);
		//cout << "current timer : "<<(unsigned long)millis()-clkStart <<"/"<<totalLoRaReceiveLength<<endl;
		delete packet;
		}   


		
		std::cout << "Main:: Exp " << i+1 << " Done." << std::endl << std::endl;
	}

}




