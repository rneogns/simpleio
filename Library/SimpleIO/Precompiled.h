#pragma once

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <atomic>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#ifdef _WINDOWS
#include <WinSock2.h>
#include <Mswsock.h>
#include <io.h>
#else //_WINDOWS
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#endif //_WINDOWS

using namespace std;

#include "CommonTypes.h"
#include "Log.h"