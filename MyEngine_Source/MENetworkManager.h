#pragma once

#include "CommonInclude.h"
#include "../MyEngine_Source/Protocol.h"

namespace ME
{

	class NetworkManager
	{
	public:
		static bool Initialize();
		static void Update();
		static void Release();

		static SOCKET GetSocket() { return mClientSocket; }
		
		template<typename T>
		static void SendPacket(T* packet)
		{
			if (mClientSocket != INVALID_SOCKET)
			{
				// 보내려는 패킷의 진짜 구조체 크기
				int packetSize = sizeof(T);

				packet->header.size = packetSize;
				send(mClientSocket, (char*)packet, sizeof(T), 0);

			}
		}

		static bool IsHost() { return mbIsHost; }

		static UINT GetMyEntityId() { return mMyEntityId; }

	private:

		static void RecvThread();

		static SOCKET mClientSocket;
		static EntityId mMyEntityId;
		static std::thread mRecvThread; // 스레드 객체
		static bool mbIsConnected;       // 스레드 종료를 위한 플래그

		static std::queue<std::vector<char>> mPacketQueue;
		static std::mutex mPacketMutex;

		static bool mbIsHost;
	};
}

