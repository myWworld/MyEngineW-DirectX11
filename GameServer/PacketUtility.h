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

	eModelType modelType = eModelType::Character;
	eWeaponType weaponType = eWeaponType::Gun;
	ePlayerState state = ePlayerState::IDLE;

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float yaw = 0.0f; //캐릭터는 현재 y축 기준 회전만 하기 때문에 
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
	pkt.state = client.state;

	pkt.x = client.x;
	pkt.y = client.y;
	pkt.z = client.z;

	pkt.yaw = client.yaw;

	return pkt;
}

Pkt_S_Move MakeMovePacket(EntityId entityId, float x, float y, float z, float yaw)
{
	Pkt_S_Move pkt = {};

	pkt.header.size = sizeof(Pkt_S_Move);
	pkt.header.type = ePacketType::S_MOVE;

	pkt.entityId = entityId;
	
	pkt.x = x;
	pkt.y = y;
	pkt.z = z;

	pkt.yaw = yaw;

	return pkt;
}

Pkt_S_State MakeStatePacket(EntityId entityId, ePlayerState state)
{
	Pkt_S_State pkt = {};

	pkt.header.size = sizeof(Pkt_S_State);
	pkt.header.type = ePacketType::S_STATE;

	pkt.entityId = entityId;
	pkt.state = state;

	return pkt;
}

Pkt_S_Attack MakeAttackPacket(const ClientInfo& client, std::uint8_t attackIndex , float x, float y, float z)
{
	Pkt_S_Attack pkt = {};

	pkt.header.size = sizeof(Pkt_S_Attack);
	pkt.header.type = ePacketType::S_ATTACK;

	pkt.entityId = client.playerId;
	pkt.weaponType = client.weaponType;
	pkt.attackIndex = attackIndex;

	pkt.dir_x = x;
	pkt.dir_y = y;
	pkt.dir_z = z;

	return pkt;
}

Pkt_S_WeaponChange MakeWeaponChangePacket(EntityId entityId, eWeaponType type)
{
	Pkt_S_WeaponChange pkt = {};

	pkt.header.size = sizeof(Pkt_S_WeaponChange);
	pkt.header.type = ePacketType::S_WEAPON_CHANGE;

	pkt.entityId = entityId;
	pkt.weaponType = type; //새로운 타입

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

	if (enterPacket == nullptr) return;

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
		me->state = enterPacket->state;

		me->x = enterPacket->x;
		me->y = enterPacket->y;
		me->z = enterPacket->z;

		me->state = ePlayerState::IDLE;

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

void HandleAttack(SOCKET clientSocket, EntityId clientId, const Pkt_C_Attack* attackPacket)
{

	if (attackPacket == nullptr) return;
	//현재 접속해 있는 클라이언트들에게 clientSocket이 어떤 공격 액션을 실행시킬지 알려야 됨.
	std::vector<SOCKET> targets;

	Pkt_S_Attack sAttackPacket = {};

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

		sAttackPacket = MakeAttackPacket(*me, attackPacket->attackIndex, attackPacket->dir_x, attackPacket->dir_y, attackPacket->dir_z);


		for (const ClientInfo& client : g_clients)
		{
			if (client.playerId == clientId) continue;

			if (client.entered == false) continue;

			targets.push_back(client.socket);
		}

	}

	SendPacketToMany(targets, sAttackPacket);
}

void HandleState(SOCKET clientSocket, EntityId clientId, const Pkt_C_State* statePacket)
{

	if (statePacket == nullptr)
		return;

	std::vector<SOCKET> targets;
	Pkt_S_State sendPacket = {};

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

		if (me == nullptr || !me->entered)
			return;

		// 똑같은 상태를 반복 브로드캐스트하지 않음
		if (me->state == statePacket->state)
			return;

		me->state = statePacket->state;

		sendPacket = MakeStatePacket(
			clientId,
			me->state
		);

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

void HandleWeaponChange(SOCKET clientSocket, EntityId clientId, const Pkt_C_WeaponChange* weaponChangePacket)
{
	std::vector<SOCKET> targets;

	if (weaponChangePacket == nullptr) return;

	Pkt_S_WeaponChange sWeaponChangePacket = {};

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

		if (me == nullptr || !me->entered)
			return;

		if (me->weaponType == weaponChangePacket->weaponType) //동일한 무기 교체 요청시 무시
			return;

		me->weaponType = weaponChangePacket->weaponType; //서버가 갖고 있는 클라이언트의 무기 정보도 업데이트 필요

		sWeaponChangePacket = MakeWeaponChangePacket(clientId, me->weaponType);


		for (const ClientInfo& client : g_clients)
		{
			if (client.playerId == clientId) continue;

			if (client.entered == false) continue;

			targets.push_back(client.socket);
		}

	}

	SendPacketToMany(targets, sWeaponChangePacket);
}


void HandleMove(EntityId clientId, const Pkt_C_Move* movePacket)
{
	if (movePacket == nullptr)
		return;

	std::vector<SOCKET> targets;
	Pkt_S_Move sendPacket = {};

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

		if (me == nullptr || !me->entered)
			return;

		me->x = movePacket->x;
		me->y = movePacket->y;
		me->z = movePacket->z;
		me->yaw = movePacket->yaw;

		sendPacket = MakeMovePacket(
			clientId,
			me->x,
			me->y,
			me->z,
			me->yaw
		);

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