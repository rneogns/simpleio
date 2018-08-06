#include "Precompiled.h"
#include "ByteBuffer.h"
#include "MatchingServerDispatcher.h"
#include "Stream.h"
#include "GamePacket.h"
#include "Log.h"
#include "SimpleIO.h"
#include "Application.h"
#include "InterPacket.h"
#include "GameManager.h"
#include "User.h"
#include "UserManager.h"
#include "Timer.h"
#include "TimerQueue.h"
#include "TimeUtil.h"

using namespace GamePacket;
using namespace InterPacket;

MatchingServerDispatcher::MatchingServerDispatcher()
{
	RegisterDispatcher(PacketNo::LOGIN_REQUEST, DISPATCHER_LOGIN_REQUEST);
}

MatchingServerDispatcher::~MatchingServerDispatcher()
{
	
}

void MatchingServerDispatcher::OnConnected(Session* session)
{
	LOG_INFO("OnConnected!");
}

void MatchingServerDispatcher::OnDisconnected(Session* session, int err)
{
//	USER_MANAGER()->RemoveUser(USER_MANAGER()->FindUser(session->GetID()));

	LOG_INFO("OnDisconnected(%d)", err);
}

int MatchingServerDispatcher::DISPATCHER_LOGIN_REQUEST(Session* session, Packet* packet)
{
	LOGIN_RESPONSE sc_login;
	sc_login.result = 0;
	sc_login.m_id = 1;
	session->Send(&sc_login);

	/*
	char buffer[256] = { 0, };
	int strlen = 0;	

	PacketHeader* packet_header = packet->GetHeader();
	strlen = packet->GetString(buffer);
	
	DB_LOGIN_REQ db_login_req(session);
	strncpy_s(db_login_req.userName, buffer, strlen);
	MESSAGE_QUEUE()->Push(DB_Q, &db_login_req);

	User* user = USER_MANAGER()->CreateUser(session);
	USER_MANAGER()->AddUser(user);
	*/
	return 0;
}

