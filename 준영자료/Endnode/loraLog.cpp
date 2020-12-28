#include "loraLog.h"
Json::Value rootArray;
string replaceString(std::string subject, const std::string &search, const std::string &replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}
 
bool writeToFile(const char* file_name, const char* buffer, int length) {
    FILE* f ;
	if ((f = fopen(file_name, "r+")) == NULL)
     f = fopen(file_name,"w+");


		char nxt = ',';
		char start_brackets = '[';
		char end_brackets = ']';
		char startArrayJson[2] = {0,};
		char tmp[2] = {0,};
		 

	     fseek(f, 0, SEEK_SET);
	     
         if(fread(startArrayJson, sizeof(char), 1, f) == 0)
         {
         	fwrite(&start_brackets, sizeof(char), 1, f);
         } 
       
         fseek(f, -1, SEEK_END);
         fread(tmp, sizeof(char), 1, f);

         if(tmp[0] == end_brackets)
         {
 
             fseek(f, -1, SEEK_END);
         	 fwrite(&nxt, sizeof(char), 1, f);
         }

        
		size_t file_size = fwrite(buffer, 1, length, f);
        fwrite(&end_brackets, sizeof(char), 1, f);
        
        
		fclose(f);

		return true;
}

string loraJSONString(MyPacket *my_packet){

 	my_packet->SetReceiveTime(time(NULL));

	string str_nictype;
		string output_config, replace_string;
		char *type;
		char cExpID[11];
		payload *payload_data;
		unsigned short index = 0;

		Json::Value root;

		for(int i = 0; i < 10; ++i)
			cExpID[i] = my_packet->GetPacketExpId()[i];
		cExpID[10] = '\0';

		string sExpID = cExpID;


		root["expid"] = sExpID;
		root["seqnum"] = my_packet->GetSeqNumber();
		root["tlength"] = my_packet->GetPacketLen();
		root["payloadnum"] = my_packet->GetPayloadNum();
		root["generatetime"] = static_cast<int> (my_packet->GetGenerateTime());
		root["receivetime"] = static_cast<int> (my_packet->GetReceiveTime());
 

//*************************************************************************************//
		cout << "payload num = " << my_packet->GetPayloadNum() << endl;

		my_packet->SetFirstPayloadCursor();

		Json::Value data;
		for(unsigned short i = 0; i < my_packet->GetPayloadNum(); ++i){
			payload_data = (payload*)((char*)my_packet->GetPayload() + index);

			if(payload_data->p_type == 1){
				string payload_type = "loraSrc";

				int value;
				memcpy(&value, payload_data->value, payload_data->p_length);
				data[payload_type] = value;
			}
			else if(payload_data->p_type == 2){
				string payload_type = "double value";
				
				double value;
				memcpy(&value, payload_data->value, payload_data->p_length);
				data[payload_type] = value;
			}
			else if(payload_data->p_type == 3){
				string payload_type = "int value";
				
				int value;
				memcpy(&value, payload_data->value, payload_data->p_length);
				data[payload_type] = value;
			}
			
			
			index += sizeof(payload_hdr) + payload_data->p_length;			
		}

		root["payload"] = data;
//*************************************************************************************//

		Json::StyledWriter writer;
		output_config = writer.write(root);

		replace_string = replaceString(output_config,"\"", "\\\"");
		replace_string = replaceString(replace_string, "\n", "");
		replace_string = replaceString(replace_string, "\t", "");
		replace_string = replaceString(replace_string, "\r", "");
		replace_string = replaceString(replace_string, " ", "");
		replace_string = replaceString(replace_string, "\u0000", "");
		replace_string = replaceString(replace_string, "\u0019", "");
		
		bool result = writeToFile("lora_packet.json", output_config.c_str(), output_config.length());

		return replace_string;
}
