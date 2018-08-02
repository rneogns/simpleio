#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <thread>

using namespace boost;


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


char recvbuffer[128];

int main()
{
	std::string raw_ip_address = "127.0.0.1";// "127.0.0.1";
	unsigned short port_num = 9999;

	try
	{
		asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address), port_num);

		asio::io_service ios;

		std::shared_ptr<asio::ip::tcp::socket> sock(new asio::ip::tcp::socket(ios, ep.protocol()
		));

		sock->async_connect(ep, [sock](const system::error_code& ec) {
			if (ec != 0)
			{
				if (ec == asio::error::operation_aborted)
				{
					std::cout << "operation cancelled!" << std::endl;
				}
				else
				{
					std::cout << "error occured! " << ec.message() << std::endl;
				}
			}

			GamePacket::LOGIN_REQUEST request;
			asio::async_write(*sock, asio::buffer(&request, sizeof(request)), [sock](const system::error_code& ec, size_t bytes_transferred) {
			});

			sock->async_read_some(boost::asio::buffer(recvbuffer, 128), [sock](const system::error_code& ec, size_t bytes_transferred) {
				GamePacket::LOGIN_RESPONSE sc_login;
				if (ec == 0)
				{
					memcpy(&sc_login, recvbuffer, bytes_transferred);
				}
			});

			return;
		});

		std::thread worker_thread([&ios]() {
			try {
				ios.run();
			}
			catch (system::system_error& e) {
				std::cout << "error occured!" << e.code() << std::endl;
			}
		});

		std::this_thread::sleep_for(std::chrono::seconds(2));

		//sock->cancel();

		worker_thread.join();

		std::string temp;
		std::getline(std::cin, temp);
	}
	catch (system::system_error &e)
	{
		std::cout << "error occured! " << e.code() << "message: " << e.what() << std::endl;

		return e.code().value();
	}

	return 0;
}