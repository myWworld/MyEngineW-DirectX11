#include <iostream>
#define WIN32_LEAN_AND_MEAN 
#include "PacketUtility.h"


void HandleClient(SOCKET clientSocket, EntityId clientId)
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
			Pkt_C_Enter* enterPacket = reinterpret_cast<Pkt_C_Enter*>(buffer);// reinterpret_cast 사용한 이유? -> 

			std::cout << "[수신] C_ENTER playerId: " << clientId << std::endl;

			HandleEnter(clientSocket, clientId, enterPacket);
			break;
		}

		case ePacketType::C_MOVE:
		{
			Pkt_C_Move* movePacket = reinterpret_cast<Pkt_C_Move*>(buffer);

			HandleMove(clientId, movePacket);
			break;
		}

		case ePacketType::C_STATE:
		{
			Pkt_C_State* statePacket = reinterpret_cast<Pkt_C_State*>(buffer);
			std::cout << "[상태변환] C_ENTER playerId: " << clientId << std::endl;
			HandleState(clientSocket, clientId, statePacket);
			break;
		}

		case ePacketType::C_ATTACK:
		{
			Pkt_C_Attack* attackPacket = reinterpret_cast<Pkt_C_Attack*>(buffer);
			std::cout << "[공격] C_ENTER playerId: " << clientId << std::endl;
			HandleAttack(clientSocket, clientId, attackPacket);
			break;
		}

		case ePacketType::C_WEAPON_CHANGE:
		{
			Pkt_C_WeaponChange* weaponChangePacket = reinterpret_cast<Pkt_C_WeaponChange*>(buffer);
			std::cout << "[무기변환] C_ENTER playerId: " << clientId << std::endl;
			HandleWeaponChange(clientSocket, clientId, weaponChangePacket);
			break;
		}

		default:
			std::cout << "[경고] 알 수 없는 패킷 type: "
				<< static_cast<int>(header->type) << std::endl;
			break;
		}

	
	}

	RemoveClient(clientSocket, clientId);
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

		{
			std::lock_guard<std::mutex> lock(g_clientMutex);

			ClientInfo clientInfo = {};
			clientInfo.entered = false;
			clientInfo.socket = clientSocket;
			clientInfo.playerId = newPlayerId;

			g_clients.push_back(clientInfo);
		}

		Pkt_S_AssignId assignPacket = MakeAssignIdPacket(newPlayerId); // 새 클라이언트에게 자기 ID 알려주기
		SendPacketTo(clientSocket, assignPacket);

		std::thread clientThread(HandleClient, clientSocket, newPlayerId); //접속이 확인되면, 새로운 스레드를 파서 패킷 수신


		// detach(): 메인 스레드와 완전히 분리하여 스스로 독립 구동되게 만듦
		// -> 메인 스레드가 바로 위로 올라가서 다음 accept()를 호출할 수 있습니다.
		clientThread.detach();
	}

	
	//리스닝 소켓 닫기 및 정리
	closesocket(listenSocket);
	WSACleanup();

	return 0;
}