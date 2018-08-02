#pragma once

#include "PacketHeader.h"

#pragma pack(push, 1)

namespace GamePacket
{
	namespace PacketNo
	{
		enum ePktID
		{
			RESULT_INFO,
			LOGIN_REQUEST,
			LOGIN_RESPONSE,			
			MAX,
		};
	}

	struct RESULT_INFO : public PacketHeader
	{
		RESULT_INFO()
			: PacketHeader(sizeof(RESULT_INFO), PacketNo::RESULT_INFO)
		{}

		enum eResultCode
		{
			SERVER_ERROR,				// �Ϲ����� ����
			WRONG_CHANNEL_PASSWORD,		// �߸��� ä�� ���� ��й�ȣ.
			NOT_FOUND_CHANNEL,			// ���� ���� �ʴ� ä��
		};

		unsigned int result;
	};

	struct LOGIN_REQUEST : public PacketHeader
	{
		LOGIN_REQUEST()
			: PacketHeader(sizeof(LOGIN_REQUEST), PacketNo::LOGIN_REQUEST)
		{
			memset(userName, 0, sizeof(userName));
		}

		char userName[32];
	};

	struct LOGIN_RESPONSE : public PacketHeader
	{
		LOGIN_RESPONSE()
			: PacketHeader(sizeof(LOGIN_RESPONSE), PacketNo::LOGIN_RESPONSE)
			, result(0)
			, m_id(0)
		{
		}

		int result;
		unsigned int m_id;
	};
}

#pragma pack(pop)
