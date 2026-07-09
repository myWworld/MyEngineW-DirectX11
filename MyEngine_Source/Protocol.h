#pragma once
#include <cstdint>


using EntityId = std::uint32_t;
#pragma pack(push, 1) //1바이트 단위로 메모리를 압축 (네트워크 낭비 방지 및 직렬화 필수)

//C_ 패킷: 클라이언트가 서버에게 요청/보고하는 패킷
//S_ 패킷 : 서버가 검증 / 확정해서 클라이언트들에게 뿌리는 패킷

enum class ePacketType : unsigned short
{
    C_ENTER = 1,
    S_ASSIGN_ID = 2,
    S_ENTER = 3,
    S_LEAVE = 4,

    C_MOVE = 5,
    S_MOVE = 6,

    C_STATE = 7,
    S_STATE = 8,

    S_MONSTER_STATE = 20,
};

enum class ePlayerState : unsigned char
{
    IDLE = 0,
    WALK = 1,
    ATTACK_1 = 2,
    ATTACK_2 = 3,
    ATTACK_3 = 4,
    DEATH = 5
};

enum class eMonsterState : unsigned char
{
    IDLE = 0,
    WALK = 1,
    ATTACK_1 = 2,
    ATTACK_2 = 3,
    DEATH = 4
};

enum class eModelType : std::uint32_t
{
    Character = 1,
    Mutant = 2,
    Alien = 3
};

enum class eWeaponType : std::uint32_t
{
    Sword = 1,
    Gun = 2,
};

struct PacketHeader
{
    std::uint16_t size;
    ePacketType type;
};

// 서버가 이 클라이언트에게 id가 무엇인지 알려주는 패킷
struct Pkt_S_AssignId
{
    PacketHeader header;
    EntityId entityId;
};

// 클라이언트 -> 서버: 입장
struct Pkt_C_Enter
{
    PacketHeader header;

    eModelType modelType;
    eWeaponType weaponType;

    float x;
    float y;
    float z;
};

// 서버 -> 클라이언트: 몇 번 플레이어가 입장
struct Pkt_S_Enter
{
    PacketHeader header;

    EntityId entityId;

    eModelType modelType;
    eWeaponType weaponType;

    float x;
    float y;
    float z;
};

// 클라이언트 -> 서버: 나 이 위치로 움직였음
// entityId 없다 -> 서버는 소켓/스레드로 누가 보냈는지 이미 알고 있기 때문
struct Pkt_C_Move
{
    PacketHeader header;

    float x;
    float y;
    float z;
};

// 서버 -> 클라이언트: 몇 번 플레이어가 이 위치로 움직였다
struct Pkt_S_Move
{
    PacketHeader header;

    EntityId entityId;

    float x;
    float y;
    float z;
};

// 클라이언트 -> 서버: 상태 바뀜
struct Pkt_C_State
{
    PacketHeader header;

    ePlayerState state;
};

// 서버 -> 클라이언트: 몇 번 플레이어 상태가 바뀜
struct Pkt_S_State
{
    PacketHeader header;

    EntityId entityId;
    ePlayerState state;
};

// 서버 -> 클라이언트: 몇 번 플레이어가 나감
struct Pkt_S_Leave
{
    PacketHeader header;

    EntityId entityId;
};

// 서버 -> 클라이언트: 몬스터 상태
struct Pkt_S_MonsterState
{
    PacketHeader header;

    EntityId entityId;
    eMonsterState state;
};

#pragma pack(pop)