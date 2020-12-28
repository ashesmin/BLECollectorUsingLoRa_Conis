#include "lora.h"

time_t previousMeasureTime = 0 ;

uint8_t dstAddr;

void initLoRa(loraConfiguration *configData)
{
		int init_state;

		printf("SX1272 module and Raspberry Pi: send packets without ACK\n");

		init_state = sx1272.ON();
		printf("Setting power ON: state %d\n", init_state);

        init_state |= sx1272.setMode(getLoRaMode(configData));
		printf("Setting Mode: state %d\n", init_state);

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
 sx1272.setPreambleLength(6);

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

int sendFoo(MyPacket* packet) {
	printf("Test Foo\n");
	return 0;
}


MyPacket* receiveLoRa(int waitTime) {

		MyPacket *receivePacket;
		char packetError[10] = "packeterr";
		char notReceive[10] = "notreceiv";
		int receiveState;

		time_t currentMeasureTime = 0 ;
		float  receiveTime = 0;

		receiveState= sx1272.receivePacketTimeout(waitTime);
		currentMeasureTime = clock();

		receiveTime =  (float)((currentMeasureTime-previousMeasureTime)/(CLOCKS_PER_SEC));
		previousMeasureTime = currentMeasureTime;

		if(receiveState == 0) {

			receivePacket = new MyPacket((uint8_t*)sx1272.packet_received.data);
			printf("Receive LoRa Packet Success!! \n");
                
			return receivePacket;
		}

		else if(receiveState == 1) {
			if((int)receiveTime < (waitTime/1000) - 1 ) {
				printf("Receive LoRa Packet Error(Conflict Or Wrong Destination) \n");
				receivePacket->SetPacketExpId(packetError);
				//receivePacket->SetPacketExpId(error_id);

			if(sx1272.getRSSI() == 0)
				receivePacket->SetLoraRssi(sx1272._RSSI);
                       
			if(sx1272.getSNR() == 0)
				receivePacket->SetLoraSnr(sx1272._SNR);
                       
			if(sx1272.getRSSIpacket() ==0)
				receivePacket->SetLoraPacketRssi(sx1272._RSSIpacket);

			return receivePacket; 
			}

			else if((int)receiveTime == ((waitTime/1000)-1) || (int)receiveTime == (waitTime/1000 ) ) {
				printf("Not Receive Anything\n");
				receivePacket->SetPacketExpId(notReceive);
				//receivePacket->SetPacketExpId(error_id);

				if(sx1272.getRSSI() == 0)
                        		receivePacket->SetLoraRssi(sx1272._RSSI);

                        	if(sx1272.getSNR() == 0)
                        		receivePacket->SetLoraSnr(sx1272._SNR);

                        	if(sx1272.getRSSIpacket() ==0)
                        		receivePacket->SetLoraPacketRssi(sx1272._RSSIpacket);

				return receivePacket;
			}
		}
}
