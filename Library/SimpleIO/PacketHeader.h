#pragma once

#pragma pack(push, 1)

struct PacketHeader
{
	unsigned short packetVerifier;		// �׻� 0xCFDA ���Դϴ�. ��Ŷ�� ��ȿ������ ��Ŷ�� ������ ���� �뵵�Դϴ�.
	unsigned int packetSize;			// ��Ŷ ����� ������ ��ü ��Ŷ ũ��
	unsigned short packetNo;			// ��Ŷ ��ȣ

	PacketHeader()
		: packetVerifier(0xCFDA)
		, packetSize(0)
		, packetNo(0)
	{}

	PacketHeader(unsigned int packet_size, unsigned short packet_id)
		: packetVerifier(0xCFDA)
		, packetSize(packet_size)
		, packetNo(packet_id)
	{}

	unsigned short GetPacketVerifier() { return packetVerifier; }
	int GetPacketSize() { return packetSize; }
	unsigned short GetPacketNo() { return packetNo; }
};

#pragma pack(pop)
