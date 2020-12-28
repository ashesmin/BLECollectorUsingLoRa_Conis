#include "configFunction.h"
#include "arduPiLoRa.h"
#include "json/json.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <iostream>

int readLoRaConfig(char* filename, loraConfiguration*** loraConfig, int* expNum) {
    FILE *fp = fopen(filename, "r");
    char buf[100000];
    Json::Value root, exps;
    Json::Reader reader;
    int i;

    fread(buf, 100000, 1, fp);
    bool parsedsuccess = reader.parse(buf, root, false);
    if(!parsedsuccess) {
        std::cout << "Failed to parse Json : " << reader.getFormattedErrorMessages() <<std::endl;
    }

    *expNum = root["loraExpNum"].asInt();

    *loraConfig = (loraConfiguration**)malloc(sizeof(loraConfiguration*)*(*expNum));
    for(i=0;i<*expNum;i++) {
        (*loraConfig)[i] = (loraConfiguration*)malloc(sizeof(loraConfiguration));
    }



    exps = root["exps"];



    for(i=0;i<root["loraExpNum"].asInt();i++) {
        Json::Value lora,wifi, loraTraffic, loraParameter;
        lora = exps[i]["exp"][1]["LoRa"];
        wifi = exps[i]["exp"][1]["WiFi"];

        loraParameter = lora[0];
        loraTraffic = lora[1];
 
        char *cExpID = (char*)malloc(10);
        memcpy(cExpID, loraTraffic["expId"].asString().c_str(), 10);
		setLoRaExpId((*loraConfig)[i], cExpID);
		free(cExpID);


	    setLoRaInterval((*loraConfig)[i],loraTraffic["packetInterval"].asInt());
	    setLoRaTotalPacketNum((*loraConfig)[i],loraTraffic["packetNum"].asInt());
        setLoRaSlotLength((*loraConfig)[i],loraTraffic["sendSlotLength"].asInt());
        setLoRaStartTime((*loraConfig)[i],loraTraffic["startTime"].asString());
        setLoRaSrcAddress((*loraConfig)[i], (uint8_t)3);
        setLoRaEndNodeNum((*loraConfig)[i], loraTraffic["sendTranceiverInfo"].size());
    

    int j;
     

        for(j = 0 ; j <loraTraffic["sendTranceiverInfo"].size();j++)
        {
            if(loraTraffic["sendTranceiverInfo"][j]["src"] == getLoRaSrcAddress((*loraConfig)[i]))
            {
               setLoRaDstAddress((*loraConfig)[i],loraTraffic["sendTranceiverInfo"][j]["dst"].asInt());
               setLoRaSendSequence((*loraConfig)[i], loraTraffic["sendTranceiverInfo"][j]["sqc"].asInt());
            } 
        }
  

        if(!loraParameter["Channel"].asString().compare(std::string("CH_01_900"))) {
		    setLoRaChannel((*loraConfig)[i], CH_01_900);
	    } else if(!loraParameter["Channel"].asString().compare(std::string("CH_02_900"))) {
            setLoRaChannel((*loraConfig)[i], CH_02_900);
        } else if(!loraParameter["Channel"].asString().compare(std::string("CH_03_900"))) {
            setLoRaChannel((*loraConfig)[i], CH_03_900);
        } else if(!loraParameter["Channel"].asString().compare(std::string("CH_04_900"))) {
            setLoRaChannel((*loraConfig)[i], CH_04_900);
        } else if(!loraParameter["Channel"].asString().compare(std::string("CH_05_900"))) {
            setLoRaChannel((*loraConfig)[i], CH_05_900);
        } else if(!loraParameter["Channel"].asString().compare(std::string("CH_06_900"))) {
	        setLoRaChannel((*loraConfig)[i], CH_06_900);
        } else if(!loraParameter["Channel"].asString().compare(std::string("CH_07_900"))) {
            setLoRaChannel((*loraConfig)[i], CH_07_900);
		} else if(!loraParameter["Channel"].asString().compare(std::string("CH_08_900"))) {
	        setLoRaChannel((*loraConfig)[i], CH_08_900);
	    } else if(!loraParameter["Channel"].asString().compare(std::string("CH_09_900"))) {
	        setLoRaChannel((*loraConfig)[i], CH_09_900);
	    } else if(!loraParameter["Channel"].asString().compare(std::string("CH_10_900"))) {
	        setLoRaChannel((*loraConfig)[i], CH_10_900);
	    } else if(!loraParameter["Channel"].asString().compare(std::string("CH_11_900"))) {
	        setLoRaChannel((*loraConfig)[i], CH_11_900);
	    } else if(!loraParameter["Channel"].asString().compare(std::string("CH_12_900"))) {
	        setLoRaChannel((*loraConfig)[i], CH_12_900);
	    }
  	       else if(!loraParameter["Channel"].asString().compare(std::string("CH_00_900"))) {
                  setLoRaChannel((*loraConfig)[i], CH_00_900);
        }


         setLoRaMode((*loraConfig)[i],loraParameter["Mode"].asInt());

  
        if(!loraParameter["Power"].asString().compare(std::string("H"))) {
	        setLoRaPower((*loraConfig)[i], 'H');
        } else if(!loraParameter["Power"].asString().compare(std::string("M"))) {
            setLoRaPower((*loraConfig)[i], 'M');
        } else if(!loraParameter["Power"].asString().compare(std::string("L"))) {
            setLoRaPower((*loraConfig)[i], 'L');
        }
/*
         printf("%d\n",getLoRaInterval((*loraConfig)[i]));
         printf("%d\n",getLoRaTotalPacketNum((*loraConfig)[i]));
         printf("%d\n",getLoRaSlotLength((*loraConfig)[i]));
         printf("%d\n",getLoRaSrcAddress((*loraConfig)[i]));
         printf("%d\n",getLoRaEndNodeNum((*loraConfig)[i]));
         printf("%d\n",getLoRaMode((*loraConfig)[i]));
         printf("%d\n",getLoRaSendSequence((*loraConfig)[i])); 
  */    

    }

    return 0;
}


void waitUntilLoRaStartTime(loraConfiguration* loraConfig) {
    time_t t;

    time(&t);

    time_t tmp = getLoRaStartTime(loraConfig);

    while(difftime(tmp, time(NULL))>0) {
                                    }
    printf("시간 지났다.\n");
}
