#pragma once
#include "../MyEngine_Source/Protocol.h"
#define WIN32_LEAN_AND_MEAN 
#include <WinSock2.h>

#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib") //빌드할 때 알아서 네트워크 관련 .lib 파일을 찾아 연결

struct ClientInfo
{
	SOCKET socket;
	EntityId playerId;

	bool entered = false;

	eModelType modelType;
	eWeaponType weaponType;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

// 접속한 모든 클라이언트의 정보를 기록
std::vector<ClientInfo> g_clients;
std::mutex g_clientMutex;
std::mutex g_sendMutex;

std::atomic<EntityId> g_nextEntityId{ 1 };




bool SendAll(SOCKET targetSocket, const char* data, int size)
{
	int totalSent = 0;

	while (totalSent < size)
	{
		int sent = send(targetSocket, data + totalSent, size - totalSent, 0); //데이터를 다 못 보낼 경우 전체크기에서 보낸만큼 빼고 , buff는 보낸 만큼 오프셋으로 이동하여 첫 주소로 사용

		if (sent == SOCKET_ERROR || sent <= 0)
		{
			return false;
		}

		totalSent += sent;
	}

	return true;
}


template <typename T>
void SendPacketTo(SOCKET targetSocket, const T& packet)
{
	std::lock_guard<std::mutex> lock(g_sendMutex);
	SendAll(targetSocket, reinterpret_cast<const char*>(&packet), packet.header.size);
}

template <typename T>
void SendPacketToMany(const std::vector<SOCKET>& targets, const T& packet)
{
	for (SOCKET target : targets)
	{
		SendPacketTo(target, packet);
	}
}


Pkt_S_AssignId MakeAssignIdPacket(EntityId entityId)
{
	Pkt_S_AssignId pkt = {};

	pkt.header.size = sizeof(Pkt_S_AssignId);
	pkt.header.type = ePacketType::S_ASSIGN_ID;
	pkt.entityId = entityId;

	return pkt;
}

Pkt_S_Enter MakeEnterPacket(const ClientInfo& client)
{
	Pkt_S_Enter pkt = {};

	pkt.header.size = sizeof(Pkt_S_Enter);
	pkt.header.type = ePacketType::S_ENTER;

	pkt.entityId = client.playerId;
	pkt.modelType = client.modelType;
	pkt.weaponType = client.weaponType;

	pkt.x = client.x;
	pkt.y = client.y;
	pkt.z = client.z;

	return pkt;
}

Pkt_S_Move MakeMovePacket(EntityId entityId, float x, float y, float z)
{
	Pkt_S_Move pkt = {};

	pkt.header.size = sizeof(Pkt_S_Move);
	pkt.header.type = ePacketType::S_MOVE;

	pkt.entityId = entityId;
	pkt.x = x;
	pkt.y = y;
	pkt.z = z;

	return pkt;
}

Pkt_S_Leave MakeLeavePacket(EntityId entityId)
{
	Pkt_S_Leave pkt = {};

	pkt.header.size = sizeof(Pkt_S_Leave);
	pkt.header.type = ePacketType::S_LEAVE;
	pkt.entityId = entityId;

	return pkt;
}



void HandleEnter(SOCKET clientSocket, EntityId clientId, const Pkt_C_Enter* enterPacket)
{
	std::vector<Pkt_S_Enter> existingPlayers;

	{//내 정보 저장 + 기존 플레이어 스냅샷 만들기

		std::lock_guard<std::mutex> lock(g_clientMutex);

		ClientInfo* me = nullptr;

		for (ClientInfo& client : g_clients)
		{
			if (client.playerId == clientId)
			{
				me = &client;
				break;
			}
		}

		if (me == nullptr) return;

		me->modelType = enterPacket->modelType;
		me->weaponType = enterPacket->weaponType;
		me->x = enterPacket->x;
		me->y = enterPacket->y;
		me->z = enterPacket->z;

		me->entered = false;

		for (const ClientInfo& other : g_clients)
		{
			if (other.playerId == clientId)
				continue;

			if (!other.entered)
				continue;

			existingPlayers.push_back(MakeEnterPacket(other)); //현재 접속해있는 클라이언트 목록 -> 새로 들어온 나의 정보를 알려야 하기 때문에
		}
	}

	//새 클라이언트에게 기존 플레이어들 먼저 보내기
	for (const Pkt_S_Enter& pkt : existingPlayers)
	{
		SendPacketTo(clientSocket, pkt);
	}

	std::vector<SOCKET> otherSockets;
	Pkt_S_Enter myEnterPacket = {};

	{
		std::lock_guard<std::mutex> lock(g_clientMutex);

		ClientInfo* me = nullptr;

		for (ClientInfo& client : g_clients)
		{
			if (client.playerId == clientId)
			{
				me = &client;
				break;
			}
		}

		if (me == nullptr)
			return;

		me->entered = true;
		myEnterPacket = MakeEnterPacket(*me);

		for (const ClientInfo& other : g_clients)
		{
			if (other.playerId == clientId)
				continue;

			if (!other.entered)
				continue;

			otherSockets.push_back(other.socket);
		}
	}

	SendPacketToMany(otherSockets, myEnterPacket);

}

void HandleMove(SOCKET clientSocket, EntityId clientId, const Pkt_C_Move* movePacket)
{
	std::vector<SOCKET> targets;
	bool canBroadcast = false;

	Pkt_S_Move sendPacket = MakeMovePacket(
		clientId,
		movePacket->x,
		movePacket->y,
		movePacket->z
	);

	{//내 정보 저장 + 기존 플레이어 스냅샷 만들기

		std::lock_guard<std::mutex> lock(g_clientMutex);

		for (ClientInfo& client : g_clients)
		{
			if (client.playerId == clientId)
			{
				if (!client.entered)
					return;

				client.x = movePacket->x;
				client.y = movePacket->y;
				client.z = movePacket->z;

				canBroadcast = true;
				break;
			}
		}

		if (!canBroadcast)
			return;


		for (const ClientInfo& other : g_clients)
		{
			if (other.playerId == clientId)
				continue;

			if (!other.entered)
				continue;

			targets.push_back(other.socket);
		}
	}


	SendPacketToMany(targets, sendPacket);
}
void RemoveClient(SOCKET clientSocket, EntityId clientId)
{
	std::vector<SOCKET> targets;
	bool shouldSendLeave = false;

	{
		std::lock_guard<std::mutex> lock(g_clientMutex);

		auto iter = std::find_if(
			g_clients.begin(),
			g_clients.end(),
			[clientId](const ClientInfo& info)
			{
				return info.playerId == clientId;
			}
		);

		if (iter != g_clients.end())
		{
			shouldSendLeave = iter->entered;
			g_clients.erase(iter);
		}

		if (shouldSendLeave)
		{
			for (const ClientInfo& client : g_clients)
			{
				if (client.entered)
				{
					targets.push_back(client.socket);
				}
			}
		}

	}

	if (shouldSendLeave)
	{
		Pkt_S_Leave leavePacket = MakeLeavePacket(clientId);
		SendPacketToMany(targets, leavePacket);
	}

	closesocket(clientSocket);
}