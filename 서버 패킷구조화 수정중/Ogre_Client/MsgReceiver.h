#pragma once
#include "NetworkManager.h"
#include "Packet.h"

unsigned int WINAPI MsgReceiverThread(LPVOID param)
{
	while( NETWORK->MsgReceiver() );

	return 0;
}