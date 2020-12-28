#include "trafficGenerator.h"

loraConfiguration *t_config;
MyPacket *my_packet;
int seqNum;
timer_t timer; 	

int milli = 1000;

int (*func_ptr)(MyPacket *mypacket) = sendLoRa;

timer_t createTimer(int signalNum){
	timer_t timer_id;
	struct sigevent se;
	se.sigev_notify=SIGEV_SIGNAL;
	se.sigev_signo = signalNum;

	timer_create(CLOCK_REALTIME, &se, &timer_id);
	return timer_id;
}

void installSigHandler(int signalNum, void(*handler)(int)){
	sigset_t set;
	struct sigaction act;
	
	act.sa_handler = handler;
	act.sa_flags = SA_RESTART;
	sigaction(signalNum, &act, NULL);

	sigemptyset(&set);
	sigaddset(&set, signalNum);
	sigprocmask(SIG_UNBLOCK, &set, NULL);

	return;
}

void setTimer(timer_t timer_id, int seconds){
	struct itimerspec timer;
	int intervalInt = seconds / milli;
	double intervalDec = seconds - (intervalInt * milli);

	timer.it_value.tv_sec = 0;
	timer.it_value.tv_nsec = 1000000;
	timer.it_interval.tv_sec = intervalInt;
	timer.it_interval.tv_nsec = intervalDec * 1000000;

	timer_settime(timer_id, 0, &timer, NULL);
	return;
}

void configPacket(MyPacket *my_packet){

	const char *nictype = "lora";
	unsigned short type1 = 1;	// int
	unsigned short type2 = 2;	// double
	
	int src = getLoRaSrcAddress(t_config);
	// int dst = getLoRaDstAddress(t_config);

	printf("src :: %d \n", src);
	printf("sizeof src %d :: \n", sizeof(src));

	// printf("dst :: %d \n", dst);
	// printf("sizeof dst %d :: \n", sizeof(dst));

	 //int temp2 = 36.5;
	//int temp3 = 10;

	my_packet->SetNicType(nictype);

	my_packet->SetPacketExpId(getLoRaExpId(t_config));
	my_packet->SetSeqNumber(seqNum + 1);
	my_packet->SetGenerateTime(time(NULL));

	my_packet->AppendPayload(type1,  sizeof(int), &src);	
	// my_packet->AppendPayload(type1,  sizeof(int), &dst);	
}

	void sigHandler(int signalNum){

	int snd_state;
	my_packet = new MyPacket;
	configPacket(my_packet);

	printf("Send Start!\n");
	if(!strcmp(my_packet->GetNicType(), "lora"))
		snd_state = func_ptr(my_packet);
	
	else{
		printf("invalid NicType\n");
	}
	seqNum++;
	if(snd_state == 0){
	cout << loraJSONString(my_packet) << endl;
	printf("sigHandler:: Send LoRa Packet %d Complete!\n\n", seqNum);
	
	}
	
	else if (snd_state == 1)
		printf("sigHandler:: Send LoRa Packet %d Error\n\n", seqNum);

	else 
		printf("sigHandler:: Send LoRa Packet %d Not Execute\n\n", seqNum);

	delete my_packet;
	
	
 
}

void trafficGenerate(int signalNum, loraConfiguration *configData){


	t_config = configData;

 	srand(time(0));  
    rand();
	int min = 0;  int max = getLoRaSlotLength(t_config)/milli;

	seqNum = 0;
	
	int totalLoRaSlotLength = getLoRaEndNodeNum(t_config)*(getLoRaSlotLength(t_config)+getLoRaInterval(t_config));

	timer = createTimer(signalNum);
	int loraSendTimeInSlotLength = (double)rand()/ RAND_MAX *(max-min)+min;   
	int sendTime = (getLoRaSendSequence(t_config))*(getLoRaSlotLength(t_config)/milli+getLoRaInterval(t_config)/milli) +loraSendTimeInSlotLength/milli;
     
    printf("ToTal EndNode Number : %d , Send Sequnce : %d \n",getLoRaEndNodeNum(t_config),getLoRaSendSequence(t_config));
    printf("TotalLoRaSlotLength(sec) : %d\n",(getLoRaEndNodeNum(t_config)*(getLoRaSlotLength(t_config)+getLoRaInterval(t_config)))/milli);
    printf("LoRaNodeInterval(sec) : %d\n",getLoRaInterval(t_config)/milli);
    printf("LoRaSlotLength(sec) : %d\n",getLoRaSlotLength(t_config)/milli);
    printf("LoRaSendTimeInSlotLength(sec) : %d\n",loraSendTimeInSlotLength);
   
    unistd::sleep(sendTime);
	
	installSigHandler(signalNum, sigHandler);
 	
	setTimer(timer, totalLoRaSlotLength);
 	
	while(seqNum<getLoRaTotalPacketNum(configData)){
	}
	timer_delete(timer);
    
}




