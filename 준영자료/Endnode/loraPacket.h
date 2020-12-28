/*!	\file	packet.h
	\brief	패킷을 생성하고 패킷내용을 확인하는 함수로 이루어진 클래스를 담고있는 헤더 파일입니다.
	\class	MyPacket
	\brief	패킷을 생성하고 패킷내용을 확인하는 함수로 이루어진 클래스
*/


#pragma once

#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "json/json.h"
#include "arduPiLoRa.h"

using namespace std;

typedef struct {
	unsigned short p_type;
	unsigned short p_length;
} payload_hdr;
//메모리 할당할 때 그 크기 만큼 
typedef struct {
	unsigned short p_type;	// 2
	unsigned short p_length;	// 2
	char value[];	// 4
} payload;

typedef struct {
	char exp_id[10];
	unsigned short seq_number;
	unsigned short payload_number;
	unsigned int generate_time;
} packet_hdr;

typedef struct {
	//unsigned short exp_id;
	char exp_id[10];	// 10 --> 12
	unsigned short seq_number;	// 2
	unsigned short payload_number;	// 2
	unsigned int generate_time;	// 4
	char payload_data[];
} packet;

typedef struct {
	//unsigned short exp_id;
	char exp_id[10];
	unsigned short seq_number;
	unsigned short payload_number;
	unsigned short packet_len;
	unsigned int generate_time;
	unsigned int lora_receive_time;
	int8_t lora_rssi;
	int8_t lora_snr;
	int16_t lora_packet_rssi;
	payload st_payload_data[];
} st_packet;

class MyPacket {

		public:
				MyPacket(); ///<	동적으로 패킷의 메모리공간을 할당하고 패킷의  내용을 초기화 하는 생성자
				MyPacket(uint8_t* data);
				~MyPacket(); ///<	동적으로 할당된 공간을 해제하는 파괴자

				inline void SetNicType(const char* _nictype) { nictype = _nictype; } ///<	사용된 통신기술을 정수로 저장해주는 함수입니다.
				inline const char* GetNicType() { return nictype;} ///<	통신기술 이름을 반환하는 함수입니다.

				inline void SetPacketExpId(char *exp_id) { memcpy(m_packet->exp_id, exp_id, 10); } ///<	실험의 ID를 패킷에 저장해주는 함수입니다.
				inline void SetSeqNumber(unsigned short  seq_number) { m_packet->seq_number = seq_number; } ///<	패킷의 번호를 패킷에 저장해주는 함수입니다.
				inline void SetGenerateTime(unsigned int  generate_time) { m_packet->generate_time = generate_time; } ///<	패킷생성시간을 패킷에 저장해주는 함수입니다.				
				void AppendPayload(unsigned short p_type, unsigned short p_length, void* value);
				inline unsigned short GetPacketLen(){ return sizeof(packet_hdr) + m_index; } ///< 전송할 패킷의 길이를 반환하는 함수입니다.

				inline char* GetPacketExpId() { return m_packet->exp_id; } ///<	실험의 ID를 반환하는 함수입니다.
				inline unsigned short GetSeqNumber() { return m_packet->seq_number; } ///<	패킷의 번호를 반환하는 함수입니다.
				inline unsigned int GetGenerateTime() { return m_packet->generate_time; } ///<	패킷의 생성시각을 반환하는 함수입니다.
				inline void SetFirstPayloadCursor(){ m_payload = (payload*)((char*)m_packet + sizeof(packet_hdr));}
				inline payload* GetPayload() { return m_payload; }
				inline void SetLoraRssi(int8_t rssi) { lora_rssi = rssi; } ///<	패킷에 수신했을 때의 rssi를 저장해주는 함수입니다.
				inline void SetLoraSnr(int8_t snr) { lora_snr = snr; } ///<	패킷에 수신했을 때의 snr을 저장해주는 함수입니다.
				inline void SetLoraPacketRssi(int16_t packetrssi) { lora_packet_rssi = packetrssi; } ///<	패킷에 수신했을 때의 packet rssi를 저장해주는 함수입니다.
				inline void SetReceiveTime(unsigned int receive_time) { lora_receive_time = receive_time; } ///<	패킷수신시간을 패킷에 저장해주는 함수입니다.
				inline void SetLoraPacketLen(unsigned short p_len) { packet_len = p_len; } ///< 수신한 데이터에서 패킷의 길이를 저장해주는 함수입니다.
				inline int8_t GetLoraRssi() { return lora_rssi; } ///<	패킷의 Lora rssi를 반환하는 함수입니다.
				inline int8_t GetLoraSnr() { return lora_snr; } ///<	패킷의 Lora snr을 반환하는 함수입니다.
				inline int16_t GetLoraPacketRssi() { return lora_packet_rssi; } ///<	패킷의 Lora packet rssi를 반환하는 함수입니다.
				inline unsigned int GetReceiveTime() { return lora_receive_time; } ///<	패킷 수신시각을 반환하는 함수입니다.
				inline unsigned short GetLoraPacketLen() { return packet_len; } ///< 수신한 패킷의 길이를 반환하는 함수입니다.

				inline unsigned short GetPayloadNum(){ return m_packet->payload_number; }
				inline packet* GetMyPacket() { return m_packet; } ///<	패킷의 주소를 반환하는 함수입니다.

		private:
				char *packet_buffer;
				packet *m_packet;
				char *payload_buffer;
				payload *m_payload;
				unsigned short m_index;
				unsigned short m_realloc;

				unsigned int lora_receive_time;
				unsigned short packet_len;
				int8_t lora_rssi;
				int8_t lora_snr;
				int16_t lora_packet_rssi;
				const char *nictype;

};
