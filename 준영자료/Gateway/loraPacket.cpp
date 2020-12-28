#include "loraPacket.h"

MyPacket::MyPacket() {
		packet_buffer = new char[sizeof(packet_hdr)];
		m_packet = (packet*)packet_buffer;

		payload_buffer = new char[10];
		memset(payload_buffer, 0, sizeof(char) * 10);

		m_index = 0;
		m_realloc = sizeof(packet_hdr);
        lora_receive_generate_time = 0;
		lora_receive_time = 0;
		packet_len = 0;
		lora_rssi = 0;
		lora_snr = 0;
		lora_packet_rssi = 0;
		nictype = '\0';

		//m_packet->exp_id = 0;
		m_packet->seq_number = 0;
		m_packet->payload_number = 0;
		m_packet->generate_time = 0;
}

MyPacket::MyPacket(uint8_t* data){
		unsigned short packet_length = sx1272.packet_received.length -5;

		packet_buffer = new char[packet_length];
		m_packet = (packet*)packet_buffer;
		memcpy(m_packet, (packet*)data, packet_length);

		payload_buffer = new char[packet_length - sizeof(packet_hdr)];
		m_payload = (payload*)payload_buffer;
		memcpy(m_payload, (payload*)(m_packet->payload_data), packet_length - sizeof(packet_hdr));

		SetLoraPacketLen(packet_length);
		// SetLoraPacketRssi(sx1272._RSSIpacket);		
}

MyPacket::~MyPacket() { delete[] packet_buffer; delete[] payload_buffer;}

void MyPacket::AppendPayload(unsigned short p_type, unsigned short p_length, void* value){
	
	m_realloc += sizeof(payload_hdr) + p_length;
	packet_buffer = (char*)realloc(packet_buffer, m_realloc );
//	m_packet = (packet*)packet_buffer;
	
	payload_buffer = new char[sizeof(payload_hdr) + p_length];
	m_payload = (payload*)payload_buffer;
	m_payload->p_type = p_type;
	m_payload->p_length = p_length;
	memcpy(m_payload->value, value, p_length);

	memcpy(m_packet->payload_data + m_index, (char*) m_payload, sizeof(payload_hdr) + p_length);
	m_index += sizeof(payload_hdr) + p_length;

	++(m_packet->payload_number);
}
