/*!	\file	packet.h
	\brief	패킷을 생성하고 패킷내용을 확인하는 함수로 이루어진 클래스를 담고있는 헤더 파일입니다.
*/
/*!
	\class	MyPacket
	\brief	패킷을 생성하고 패킷내용을 확인하는 함수로 이루어진 클래스
	\param *packet_buffer
	\param *m_packet
	\param *payload_buffer
	\param *m_payload
	\param m_index
	\param m_realloc
	\param lora_receive_time
	\param packet_len
	\param lora_rssi
	\param lora_snr
	\param lora_packet_rssi
	\param *nictype
*/
/*!
	\typedef payload_hdr
	\brief payload에 대한 header를 담고있는 구조체
	\param p_type
	\param p_length
*/
/*!
	\var typedef payload
	\brief 실제 payload의 내용을 담고있는 구조체
	\param p_type
	\param p_length
	\param value[]
*/
/*!	\var typedef packet_hdr
	\brief packet에 대한 header를 담고있는 구조체
	\param exp_id
	\param seq_number
	\param payload_number
	\param generate_time
*/
/*!
	\var typedef packet
	\brief End-Node로부터 수신한 packet을 담고있는 구조체
	\param exp_id
	\param seq_number
	\param payload_number
	\param generate_time
	\param payload_data
*/
/*!
	\var typedef st_packet_hdr
	\brief End-Node로부터 수신한 패킷에 성능척도를 추가한 구조체의 header(메시지큐에 넣기 위해서 객체->구조체로 변경)
	\param exp_id
	\param seq_number
	\param payload_number
	\param generate_time
	\param packet_len
	\param lora_receive_time
	\param lora_rssi
	\param lora_snr
	\param lora_packet_rssi
*/
/*!
	\var typedef st_packet
	\brief End-Node로부터 수신한 패킷에 성능척도를 추가한 구조체(메시지큐에 넣기 위해서 객체->구조체로 변경)
	\param exp_id
	\param seq_number
	\param payload_number
	\param packet_len
	\param generate_time
	\param lora_receive_time
	\param lora_rssi
	\param lora_snr
	\param lora_packet_rssi
	\param st_payload_data
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
	unsigned short p_type; /*!< payload의 타입 */
	unsigned short p_length; /*!< payload의 길이 */	
} payload_hdr;

typedef struct {
	unsigned short p_type; /*!< payload의 타입 */
	unsigned short p_length; /*!< payload의 길이 */
	char value[]; /*!< payload의 값 */
} payload;

typedef struct {
	//unsigned short exp_id; /*!< 현재 진행중인 실험의 ID */
	char exp_id[10];
	unsigned short seq_number; /*!< 수신한 패킷의 순서번호 */
	unsigned short payload_number; /*!< 현재 진행중인 실험의 payload 갯수 */
	unsigned int generate_time; /*!< 수신한 패킷이 생성된 시간 */
} packet_hdr;

typedef struct {
	//unsigned short exp_id; /*!< 현재 진행중인 실험의 ID */
	char exp_id[10];
	unsigned short seq_number; /*!< 수신한 패킷의 순서번호 */
	unsigned short payload_number; /*!< 현재 진행중인 실험의 payload 갯수 */
	unsigned int generate_time; /*!< 수신한 패킷이 생성된 시간 */
	char payload_data[]; /*!< 패킷안에 들어있는 payload의 값 */
} packet;

typedef struct {
	//unsigned short exp_id; /*!< 현재 진행중인 실험의 ID */
	char exp_id[10];
	unsigned short seq_number; /*!< 수신한 패킷의 순서번호 */
	unsigned short payload_number; /*!< 현재 진행중인 실험의 payload 갯수 */
	unsigned short packet_len; /*!< 수신한 패킷의 길이 */
	unsigned int generate_time; /*!< 수신한 패킷이 생성된 시간 */
	unsigned int lora_receive_time; /*!< 패킷을 수신한 시간 */
	unsigned int lora_receive_generate_time;	/*!< 패킷을 수신 함수를 호출한 시간 */
	int8_t lora_rssi; /*!< 수신한 패킷의 채널에 대한 RSSI */
	int8_t lora_snr; /*!< 수신한 패킷의 SNR */
	int16_t lora_packet_rssi; /*!< 수신한 패킷에 대한 RSSI */
} st_packet_hdr;

typedef struct {
	//unsigned short exp_id; /*!< 현재 진행중인 실험의 ID */
	char exp_id[10];
	unsigned short seq_number; /*!< 수신한 패킷의 순서번호 */
	unsigned short payload_number; /*!< 현재 진행중인 실험의 payload 갯수 */
	unsigned short packet_len; /*!< 수신한 패킷의 길이 */
	unsigned int generate_time; /*!< 수신한 패킷이 생성된 시간 */
	unsigned int lora_receive_time;	/*!< 패킷을 수신한 시간 */
	unsigned int lora_receive_generate_time;	/*!< 패킷을 수신 함수를 호출한 시간 */
	int8_t lora_rssi; /*!< 수신한 패킷의 채널에 대한 RSSI */
	int8_t lora_snr; /*!< 수신한 패킷의 SNR */
	int16_t lora_packet_rssi; /*!< 수신한 패킷에 대한 RSSI */
	char st_payload_data[]; /*!< 수신한 패킷의 payload */
} st_packet;

class MyPacket {

		public:
				MyPacket(); ///< 동적으로 패킷의 메모리공간을 할당하고 패킷의 내용을 초기화하는 생성자
				MyPacket(uint8_t* data); ///< 수신한 데이터의 내용을 객체로 바꿔주는 생성자 

				~MyPacket(); ///< 동적으로 할당된 공간을 해제하는 파괴자

				inline void SetNicType(const char* _nictype) { nictype = _nictype; } ///< 사용된 통신기술을 정수로 저장해주는 함수입니다.
				inline const char* GetNicType() { return nictype; } ///< 사용된 통신기술을 표현한 정수를 반환하는 함수입니다.

				//inline void SetPacketExpId(unsigned short exp_id) { m_packet->exp_id = exp_id; } ///< 실험의 ID를 패킷에 저장해주는 함수입니다.
				inline void SetPacketExpId(char *exp_id) { memcpy(m_packet->exp_id, exp_id, 10); } ///<	실험의 ID를 패킷에 저장해주는 함수입니다.
				inline void SetSeqNumber(unsigned short seq_number) { m_packet->seq_number = seq_number; } ///<	패킷의 번호를 패킷에 저장해주는 함수입니다.
				inline void SetGenerateTime(unsigned int generate_time) { m_packet->generate_time = generate_time; } ///< 패킷생성시간을 패킷에 저장해주는 함수입니다.
				void AppendPayload(unsigned short p_type, unsigned short p_length, void* value); ///< payload를 추가해주는 함수입니다.  
				inline unsigned short GetPacketLen() { return sizeof(packet_hdr) + m_index; } ///< 실제 수신한 패킷의 길이를 반환하는 함수입니다.

				//inline unsigned short GetPacketExpId() { return m_packet->exp_id; } ///< 실험의 ID를 반환하는 함수입니다.
				inline char* GetPacketExpId() { return m_packet->exp_id; } ///<	실험의 ID를 반환하는 함수입니다.
				inline unsigned short GetSeqNumber() { return m_packet->seq_number; } ///<패킷의 번호를 반환하는 함수입니다.
				inline unsigned int GetGenerateTime() { return m_packet->generate_time; } ///<	패킷의 생성시각을 반환하는 함수입니다.
				inline void SetFirstPayloadCursor() { m_payload = (payload*)((char*)m_packet + sizeof(packet_hdr)); } ///< 포인터를 첫번째 payload의 메모리주소로 옮기는 함수입니다.
				inline payload* GetPayload() { return m_payload; } ///< 패킷의 payload 주소를 반환하는 함수입니다.
				inline void SetLoraRssi(int8_t rssi) { lora_rssi = rssi; } ///< 패킷의 rssi를 저장해주는 함수입니다.
				inline void SetLoraSnr(int8_t snr) { lora_snr = snr; } ///< 패킷의 snr을 저장해주는 함수입니다.
				inline void SetLoraPacketRssi(int16_t packetrssi) { lora_packet_rssi = packetrssi; } ///< 패킷의 packet rssi를 저장해주는 함수입니다.
				inline void SetReceiveTime(unsigned int receive_time) { lora_receive_time = receive_time; } ///< 패킷이 수신된 시간을 저장해주는 함수입니다.
				inline void SetReceiveGenerateTime(unsigned int receive_generate_time) { lora_receive_generate_time = receive_generate_time; } ///< 패킷이 수신된 시간을 저장해주는 함수입니다.

				inline void SetLoraPacketLen(unsigned short p_len) { packet_len = p_len; } ///< 수신한 패킷의 길이를 저장해주는 함수입니다.
				inline int8_t GetLoraRssi() { return lora_rssi; } ///< 패킷의 Lora rssi를 반환하는 함수입니다.
				inline int8_t GetLoraSnr() { return lora_snr; } ///< 패킷의 Lora snr을 반환하는 함수입니다.
				inline int16_t GetLoraPacketRssi() { return lora_packet_rssi; } ///< 패킷의 Lora packet rssi를 반환하는 함수입니다.
				inline unsigned int GetReceiveTime() { return lora_receive_time; } ///< 패킷을 수신한 시간을 반환하는 함수입니다.
				inline unsigned int GetReceiveGenerateTime() { return lora_receive_generate_time; }
				inline unsigned short GetLoraPacketLen() { return packet_len; } ///< LoRa 패킷의 길이를 반환하는 함수입니다.
				inline unsigned short GetPayloadNum() { return m_packet->payload_number; } ///< 패킷안에 들어있는 payload의 갯수를 반환하는 함수입니다.
				inline packet* GetMyPacket() { return m_packet; } ///< 패킷의 주소를 반환하는 함수입니다.

		private:
				char *packet_buffer;
				packet *m_packet;
				char *payload_buffer;
				payload *m_payload;
				unsigned short m_index;
				unsigned short m_realloc;

				unsigned int lora_receive_time;
				unsigned int lora_receive_generate_time;
				unsigned short packet_len;
				int8_t lora_rssi;
				int8_t lora_snr;
				int16_t lora_packet_rssi;
				const char *nictype;

};
