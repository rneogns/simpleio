#include "Precompiled.h"
#include "IONotifier.h"
#include "Acceptor.h"
#include "Session.h"
#include "SessionManager.h"

IONotifier::IONotifier()
	: _io_service(new boost::asio::io_service)
{
}

IONotifier::~IONotifier()
{
	_io_service->stop();
}

boost::asio::io_service* IONotifier::get_io_service()
{
	return _io_service;
}

void IONotifier::run()
{
	_io_service->run();
}
