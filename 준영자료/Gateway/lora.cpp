#include "lora.h"

uint8_t dstAddr;

extern char globalExpId[10];

void initLoRa(loraConfiguration *configData)
{
		
		int init_state;

		printf("SX1272 module and Raspberry Pi: send packets without ACK\n");

		init_state = sx1272.ON();
		printf("Setting power ON: state %d\n", init_state);

        init_state |= sx1272.setMode(getLoRaMode(configData));
		printf("Setting Mode: state %d\n", init_state);

		setLoRaReceiveWaitTime(configData,getLoRaMode(configData));

		init_state |= sx1272.setHeaderON();
		printf("Setting Header ON: state %d\n", init_state);

		init_state |= sx1272.setChannel(getLoRaChannel(configData));
		printf("Setting Channel: state %d\n", init_state);

		init_state |= sx1272.setCRC_ON();
		printf("Setting CRC ON: state %d\n", init_state);

		init_state |= sx1272.setPower(getLoRaPower(configData));
		printf("Setting Power: state %d\n",
						init_state);

		init_state |= sx1272.setNodeAddress(getLoRaSrcAddress(configData));
		printf("Setting Node address: state %d\n", init_state);

		dstAddr = getLoRaDstAddress(configData);

		if ( init_state == 0) {
			printf("SX1272 successfully configured\n");
		} else {
			printf("SX1272 initialization failed\n");
		}
		delay(1000);

}

int sendLoRa(MyPacket* packet) {  
			int sendState;

//		sendState = sx1272.sendPacketTimeout(dstAddr, (uint8_t*)packet->GetMyPacket(), 100);
		sendState = sx1272.sendPacketTimeout(dstAddr, (uint8_t*)packet->GetMyPacket(), packet->GetPacketLen());

		cout << "snd_stat = " << sendState << endl;

		if(sendState == 0 ) return 0;
		if(sendState == 1 ) return 1;
		if(sendState == 2 ) return 2;

		else 
			return 3; 

}

MyPacket* receiveLoRa(int waitTime) {

		MyPacket *receivePacket;
		//char packeterr[10] = "packeterr";
		//char notreceiv[10] = "notreceiv";
		int receiveState;
        unsigned int receiveGenerateTime= time(NULL);

		receiveState= sx1272.receivePacketTimeout(waitTime);
		
		if(receiveState == 0) {
			receivePacket = new MyPacket((uint8_t*)sx1272.packet_received.data);
			receivePacket->SetReceiveTime(time(NULL));
			receivePacket->SetReceiveGenerateTime(receiveGenerateTime);
 
			printf("Receive LoRa Packet Success!! \n");
                
			if(sx1272.getRSSI() == 0)
				receivePacket->SetLoraRssi(sx1272._RSSI);
                       
			if(sx1272.getSNR() == 0)
				receivePacket->SetLoraSnr(sx1272._SNR);
                       
			if(sx1272.getRSSIpacket() ==0)
				receivePacket->SetLoraPacketRssi(sx1272._RSSIpacket);

			
			

			return receivePacket;
		}

		else if(receiveState == 1) {
			receivePacket = new MyPacket;
			receivePacket->SetReceiveTime(time(NULL));
			receivePacket->SetReceiveGenerateTime(receiveGenerateTime);


				printf("Not Receive Anything\n");
				receivePacket->SetPacketExpId(globalExpId);
				receivePacket->SetGenerateTime(9999);

				if(sx1272.getRSSI() == 0)
                        		receivePacket->SetLoraRssi(sx1272._RSSI);

                        	if(sx1272.getSNR() == 0)
                        		receivePacket->SetLoraSnr(sx1272._SNR);

                        	if(sx1272.getRSSIpacket() ==0)
                        		receivePacket->SetLoraPacketRssi(sx1272._RSSIpacket);

				return receivePacket;
			}


		else if(receiveState == 3) {
			receivePacket = new MyPacket;
			receivePacket->SetReceiveTime(time(NULL));
			receivePacket->SetReceiveGenerateTime(receiveGenerateTime);
	
				 
				printf("Receive LoRa Packet Error(Conflict Or Wrong Destination) \n");
				receivePacket->SetPacketExpId(globalExpId);
				receivePacket->SetGenerateTime(8888);

			if(sx1272.getRSSI() == 0)
				receivePacket->SetLoraRssi(sx1272._RSSI);
                       
			if(sx1272.getSNR() == 0)
				receivePacket->SetLoraSnr(sx1272._SNR);
                       
			if(sx1272.getRSSIpacket() ==0)
				receivePacket->SetLoraPacketRssi(sx1272._RSSIpacket);

			receivePacket->SetReceiveTime(time(NULL));

			return receivePacket; 
			}

			
			else{

			receivePacket = new MyPacket;
		    receivePacket->SetReceiveTime(time(NULL));
		    receivePacket->SetReceiveGenerateTime(receiveGenerateTime);


			    printf("Command Not Excute\n");
				receivePacket->SetPacketExpId(globalExpId);
				receivePacket->SetGenerateTime(0000);

			if(sx1272.getRSSI() == 0)
                  receivePacket->SetLoraRssi(sx1272._RSSI);

            if(sx1272.getSNR() == 0)
                  receivePacket->SetLoraSnr(sx1272._SNR);

          	if(sx1272.getRSSIpacket() ==0)
                  receivePacket->SetLoraPacketRssi(sx1272._RSSIpacket);

				return receivePacket;
			}
		}

