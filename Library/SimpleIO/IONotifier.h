#pragma once

#include <boost/asio.hpp>

class IONotifier
{
private:
	boost::asio::io_service* _io_service;

public:
	IONotifier();
	~IONotifier();

	boost::asio::io_service* get_io_service();

	void run();
};

