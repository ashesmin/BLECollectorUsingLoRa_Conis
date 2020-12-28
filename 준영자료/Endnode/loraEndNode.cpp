 	
#include "loraEndNode.h"
loraConfiguration** configData;
void loraEndNodeExp()
{
 	char file_name[] = {"./usermode.cfg"};
 	int i, flag, exp_number;
	int interval;

	flag=readLoRaConfig(file_name, &configData, &exp_number);
	if(flag ==-1) {
		printf("Fail to read file .... \n");
	}

    
    	 
	for(i = 0; i < exp_number; i++) {
 initLoRa(configData[i]);

		waitUntilLoRaStartTime(configData[i]);
//		initLoRa(configData[i]);
		std::cout << std::endl << std::endl;

		std::cout << "Main:: ----------------" << std::endl;

	    
		trafficGenerate(SIGUSR1, configData[i]);
		
		std::cout << "Main:: Exp " << i+1 << " Done." << std::endl << std::endl;
	}

}
