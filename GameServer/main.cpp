#include <iostream>
#define WIN32_LEAN_AND_MEAN 
#include "ServerTypes.h"
#include "ServerWorld.h"
#include "PacketUtility.h"




void HandleClient(SOCKET clientSocket, EntityId clientId, ServerWorld& world)
{
	char buffer[1024]; //패킷을 임시로 받을 바구니 -> 1바이트로 직렬화 했기 때문

	while (true)
	{
		int recvBytes = recv(clientSocket, buffer, sizeof(buffer), 0); //recv: 데이터가 올 때까지 여기서 멈춤 (블로킹)
		
		if (recvBytes <= 0)
		{
			std::cout << "[연결 종료] 클라이언트가 나갔습니다. playerId: "
				<< clientId << " / socket: " << clientSocket << std::endl;

			break;
		}

		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

		switch (header->type)
		{
		case ePacketType::C_ENTER:
		{
			const Pkt_C_Enter* packet =
				reinterpret_cast<const Pkt_C_Enter*>(buffer);

			EnterCommand command = {};

			command.entityId = clientId;
			command.modelType = packet->modelType;
			command.weaponType = packet->weaponType;

			command.position =
			{
				packet->x,
				packet->y,
				packet->z
			};

			command.yaw = packet->yaw;

			world.EnqueueCommand(command);
			break;
		}

		case ePacketType::C_MOVE:
		{
			Pkt_C_Move* packet = reinterpret_cast<Pkt_C_Move*>(buffer);

			MoveCommand command = {};

			command.entityId = clientId;
			command.position =
			{
				packet->x,
				packet->y,
				packet->z
			};

			command.yaw = packet->yaw;

			world.EnqueueCommand(command);
			break;
		}

		case ePacketType::C_STATE:
		{
			Pkt_C_State* packet = reinterpret_cast<Pkt_C_State*>(buffer);
			
			StateCommand command = {};

			command.entityId = clientId;
			command.state = packet->state;

			world.EnqueueCommand(command);
			break;
		}

		case ePacketType::C_ATTACK:
		{
			Pkt_C_Attack* packet = reinterpret_cast<Pkt_C_Attack*>(buffer);
			
			AttackCommand command = {};

			command.entityId = clientId;

			command.origin =
			{
				packet->origin_x,
				packet->origin_y,
				packet->origin_z
			};

			command.direction =
			{
				packet->dir_x,
				packet->dir_y,
				packet->dir_z
			};

			command.attackIndex = packet->attackIndex;

			world.EnqueueCommand(command);
			break;
		}

		case ePacketType::C_WEAPON_CHANGE:
		{
			Pkt_C_WeaponChange* packet = reinterpret_cast<Pkt_C_WeaponChange*>(buffer);
			
			WeaponChangeCommand command= {};

			command.entityId = clientId;
			command.weaponType = packet->weaponType;

			world.EnqueueCommand(command);
			break;
		}

		default:
			std::cout << "[경고] 알 수 없는 패킷 type: "
				<< static_cast<int>(header->type) << std::endl;
			break;
		}

	
	}

	MarkSessionEntered(clientId, false);    // 이후 브로드캐스트 대상에서 즉시 제외

	closesocket(clientSocket);
	RemoveSession(clientId);

	world.EnqueueCommand(
		LeaveCommand{ clientId } //->서버 월드쪽 클라이언 세션 보관에서 제거해야함
	);
}

int main()
{
	WSADATA wsaData;//win socket 초기화
	int startUpResult = WSAStartup(MAKEWORD(2, 2), &wsaData); //2.2버젼

	if (startUpResult != 0)
	{
		std::cerr << "WSAStartup 초기화 실패! 에러 코드: " << startUpResult << std::endl;
		return 1; // 실패 시 프로그램 종료
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);//ipv2, tcp 프로토콜 사용
	if (listenSocket == INVALID_SOCKET)
	{
		std::cerr << "리스닝 소켓 생성 실패! 에러 코드: " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	SOCKADDR_IN serverAddr = {}; //서버 세팅 ip주소와 포트 번호
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //INADDR_ANY: 내 PC의 어떤 IP로 들어오든 다 받음 , htonl -> 빅 에디안 to 리틀 ( 빅에디안은 사람이 읽기 편한것 리틀은 컴퓨터 관점에서 연산등 친화적
	serverAddr.sin_port = htons(7777);

	if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) //소켓에 주소와 포트 결합 (Bind)
	{
		std::cerr << "Bind 실패! 에러 코드: " << WSAGetLastError() << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	if(listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) //소켓 열어놓고 대기 , SOMAXCONN: OS가 허용하는 최대 접속 대기 큐 크기 적용
	{
		std::cerr << "Listen 실패! 에러 코드: " << WSAGetLastError() << std::endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	std::cout << "서버가 7777 포트에서 클라이언트를 기다리는 중입니다..." << std::endl;

	ServerWorld world;
	world.SetNetworkCallbacks(SendRawToEntity, BroadcastRawExcept, MarkSessionEntered);
	world.SetAllocateEntityIdCallback([]() -> EntityId { return g_nextEntityId.fetch_add(1); });

	//서버 월드도 항상 백그라운드 쓰레드에서 돌아가며 클라이언트 요청에 대한 처리 및 서버 월드 업데이트(몬스터, 총알, 충돌처리 등...) 담당해야함
	std::thread worldThread(&ServerWorld::Run, &world);//어떤 ServerWorld 객체의 Run인지 객체주소도 보내야함

	while (true) //여러 클라이언트로 부터 accept를 받을 수 있도록 한다
	{
		SOCKADDR_IN clientAddr = {}; //클라이언트 접속 수락 (Accept)
		//  accept는 클라이언트가 들어올 때까지 이 라인에서 코드 실행을 멈추고 대기g한다
		int clientAddrLen = sizeof(clientAddr);

		SOCKET clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &clientAddrLen);

		if (clientSocket == INVALID_SOCKET)
		{
			std::cerr << "Accept 에러 " << std::endl;
			continue;
		}

		std::cout << "새 클라이언트 접속 완료!IP: " << clientSocket << std::endl;

		EntityId newPlayerId = g_nextEntityId.fetch_add(1);//새로 들어오는 클라이언트 마다 고유 id 부여(서버 역할)
		AddSession(newPlayerId, clientSocket);


		Pkt_S_AssignId assignPacket = {}; // 새 클라이언트에게 자기 ID 알려주기

		assignPacket.header.type =
			ePacketType::S_ASSIGN_ID;

		assignPacket.header.size =
			sizeof(Pkt_S_AssignId);

		assignPacket.entityId =
			newPlayerId;

		SendRawToEntity(
			newPlayerId,
			&assignPacket,
			assignPacket.header.size
		);
	

		std::thread clientThread(HandleClient, clientSocket, newPlayerId, std::ref(world)); //접속이 확인되면, 새로운 스레드를 파서 패킷 수신


		// detach(): 메인 스레드와 완전히 분리하여 스스로 독립 구동되게 만듦
		// -> 메인 스레드가 바로 위로 올라가서 다음 accept()를 호출할 수 있습니다.
		clientThread.detach();
	}

	world.Stop(); //서버 닫힐 시 서버 월드 종료후 백그라운드에서 재생되고 있는 쓰레드 종료시켜야함
	
	if (worldThread.joinable())
		worldThread.join();
	
	//리스닝 소켓 닫기 및 정리
	closesocket(listenSocket);
	WSACleanup();

	return 0;
}