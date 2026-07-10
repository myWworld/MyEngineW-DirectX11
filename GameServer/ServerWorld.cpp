#include "ServerWorld.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>
#include <utility>

namespace
{
    bool IsFinite(float value)
    {
        return std::isfinite(value);
    }

    bool IsValidPosition(const ServerVec3& position)
    {
        return IsFinite(position.x)
            && IsFinite(position.y)
            && IsFinite(position.z);
    }

    bool IsValidWeaponType(eWeaponType weaponType)
    {
        switch (weaponType)
        {
        case eWeaponType::Gun:
        case eWeaponType::Sword:
            return true;

        default:
            return false;
        }
    }

    bool IsValidAttackIndex(
        eWeaponType weaponType,
        std::uint8_t attackIndex)
    {
        switch (weaponType)
        {
        case eWeaponType::Gun:
            return attackIndex == 0;

        case eWeaponType::Sword:
            return attackIndex < 3;

        default:
            return false;
        }
    }

    Pkt_S_Enter MakeEnterPacket(const ServerPlayer& player)
    {
        Pkt_S_Enter packet = {};

        packet.header.type = ePacketType::S_ENTER;
        packet.header.size = sizeof(Pkt_S_Enter);

        packet.entityId = player.entityId;
        packet.modelType = player.modelType;
        packet.weaponType = player.weaponType;
        packet.state = player.state;

        packet.x = player.position.x;
        packet.y = player.position.y;
        packet.z = player.position.z;
        packet.yaw = player.yaw;

        return packet;
    }

    Pkt_S_Move MakeMovePacket(const ServerPlayer& player)
    {
        Pkt_S_Move packet = {};

        packet.header.type = ePacketType::S_MOVE;
        packet.header.size = sizeof(Pkt_S_Move);

        packet.entityId = player.entityId;

        packet.x = player.position.x;
        packet.y = player.position.y;
        packet.z = player.position.z;
        packet.yaw = player.yaw;

        return packet;
    }

    Pkt_S_State MakeStatePacket(const ServerPlayer& player)
    {
        Pkt_S_State packet = {};

        packet.header.type = ePacketType::S_STATE;
        packet.header.size = sizeof(Pkt_S_State);

        packet.entityId = player.entityId;
        packet.state = player.state;

        return packet;
    }

    Pkt_S_WeaponChange MakeWeaponChangePacket(
        const ServerPlayer& player)
    {
        Pkt_S_WeaponChange packet = {};

        packet.header.type = ePacketType::S_WEAPON_CHANGE;
        packet.header.size = sizeof(Pkt_S_WeaponChange);

        packet.entityId = player.entityId;
        packet.weaponType = player.weaponType;

        return packet;
    }

    Pkt_S_Attack MakeAttackPacket(
        const ServerPlayer& player,
        const AttackCommand& command)
    {
        Pkt_S_Attack packet = {};

        packet.header.type = ePacketType::S_ATTACK;
        packet.header.size = sizeof(Pkt_S_Attack);

        packet.entityId = player.entityId;
        packet.weaponType = player.weaponType;
        packet.attackIndex = command.attackIndex;

        packet.dir_x = command.direction.x;
        packet.dir_y = command.direction.y;
        packet.dir_z = command.direction.z;

        return packet;
    }

    Pkt_S_Leave MakeLeavePacket(EntityId entityId)
    {
        Pkt_S_Leave packet = {};

        packet.header.type = ePacketType::S_LEAVE;
        packet.header.size = sizeof(Pkt_S_Leave);
        packet.entityId = entityId;

        return packet;
    }

    Pkt_S_MonsterSpawn MakeMonsterSpawnPacket(const ServerMonster& monster)
    {
        Pkt_S_MonsterSpawn packet = {};

        packet.header.type =
            ePacketType::S_MONSTER_SPAWN;

        packet.header.size =
            sizeof(Pkt_S_MonsterSpawn);

        packet.entityId = monster.entityId;

        packet.modelType = monster.modelType;
        packet.weaponType = monster.weaponType;
        packet.state = monster.state;

        packet.x = monster.position.x;
        packet.y = monster.position.y;
        packet.z = monster.position.z;
        packet.yaw = monster.yaw;

        packet.hp = monster.hp;
        packet.maxHp = monster.maxHp;

        return packet;
    }

    /*Pkt_S_MonsterMove MakeMonsterMovePacket(EntityId entityId)
    {
        Pkt_S_MonsterMove packet = {};

        packet.header.type = ePacketType::S_MONSTER_MOVE;
        packet.header.size = sizeof(Pkt_S_MonsterMove);

        packet.entityId = entityId;

        packet.x = player.position.x;
        packet.y = player.position.y;
        packet.z = player.position.z;
        packet.yaw = player.yaw;

        return packet;
    }*/

    Pkt_S_State MaketatePacket(const ServerPlayer& player)
    {
        Pkt_S_State packet = {};

        packet.header.type = ePacketType::S_STATE;
        packet.header.size = sizeof(Pkt_S_State);

        packet.entityId = player.entityId;
        packet.state = player.state;

        return packet;
    }
}

void ServerWorld::SetNetworkCallbacks(
    SendToCallback sendTo,
    BroadcastExceptCallback broadcastExcept,
    MarkEnteredCallback markEntered)
{
    mSendToCallback = std::move(sendTo);
    mBroadcastExceptCallback = std::move(broadcastExcept);
    mMarkEnteredCallback = std::move(markEntered);
}

void ServerWorld::SetAllocateEntityIdCallback(AllocateEntityIdCallback allocator)
{
    mAllocateEntityIdCallback = std::move(allocator);
}

void ServerWorld::EnsureWorldInitialization()
{
    if (mbInitialized.load()) return;

    mbInitialized.store(true);

    SpawnMonster(eModelType::Mutant, eWeaponType::Gauntlet, { 500.0f, 0.0f, 0.0f }, 0, true);


}

void ServerWorld::EnqueueCommand(WorldCommand command)
{
    std::lock_guard<std::mutex> lock(mCommandMutex);

    mCommandQueue.push(std::move(command));
}

void ServerWorld::Run()
{
    if (mbRunning.exchange(true))
    {
        // 이미 실행 중
        return;
    }

    using Clock = std::chrono::steady_clock;

    constexpr float FixedDeltaTime = 1.0f / 60.0f;
    constexpr auto TickDuration =
        std::chrono::microseconds(16667);

    auto nextTick = Clock::now();

    while (mbRunning.load())
    {
        nextTick += TickDuration;

        ProcessCommands();
        Tick(FixedDeltaTime);

        const auto now = Clock::now();

        if (now < nextTick)
        {
            std::this_thread::sleep_until(nextTick);
        }
        else
        {
            // 서버 처리가 너무 늦어진 경우 무한 따라잡기 방지
            nextTick = now;
        }
    }
}

void ServerWorld::Stop()
{
    mbRunning.store(false);
}

void ServerWorld::ProcessCommands()
{
    std::queue<WorldCommand> localCommands;

    {
        std::lock_guard<std::mutex> lock(mCommandMutex);

        // 잠금 상태에서 실제 처리하지 않고
        // 로컬 큐로 한 번에 옮긴다.
        std::swap(localCommands, mCommandQueue);
    }

    while (!localCommands.empty())
    {
        WorldCommand command =
            std::move(localCommands.front());

        localCommands.pop();

        std::visit(
            [this](const auto& concreteCommand)
            {
                HandleCommand(concreteCommand);
            },
            command
        );
    }
}

void ServerWorld::Tick(float deltaTime)
{
    for (auto& [entityId, player] : mPlayers)
    {
        if (player.attackCooldown > 0.0f)
        {
            player.attackCooldown -= deltaTime;

            if (player.attackCooldown < 0.0f)
            {
                player.attackCooldown = 0.0f;
            }
        }
    }

    UpdateMonsters(deltaTime);
    UpdateProjectiles(deltaTime);
}

void ServerWorld::HandleCommand(const EnterCommand& command)
{
    if (command.entityId == 0) return;

    if (!IsValidPosition(command.position)) return;


    if (!IsValidWeaponType(command.weaponType))
        return;

    if (mPlayers.find(command.entityId) != mPlayers.end())
    {
        // 중복 C_ENTER 방지
        return;
    }


    ServerPlayer newPlayer = {};

    newPlayer.entityId = command.entityId;
    newPlayer.modelType = command.modelType;
    newPlayer.weaponType = command.weaponType;
    newPlayer.state = ePlayerState::IDLE;

    newPlayer.position = command.position;
    newPlayer.yaw = command.yaw;

    EnsureWorldInitialization();


    for (const auto& [existingId, existingPlayer] : mPlayers)  //새 클라이언트 들에게 새로 들어온 애 알림
    {
        Pkt_S_Enter packet =
            MakeEnterPacket(existingPlayer);

        SendTo(command.entityId, packet);
    }


    SendMonsterSnapshotTo(newPlayer.entityId);  // 새 클라이언트에게 현재 몬스터 스냅샷

    mPlayers.emplace(newPlayer.entityId, newPlayer); // 새로운 클라이언트 서버 월드 맵에 등록

    if (mMarkEnteredCallback)
    {
        mMarkEnteredCallback(command.entityId, true);
    }


    Pkt_S_Enter newPlayerPacket =
        MakeEnterPacket(newPlayer);

    BroadcastExcept(
        command.entityId,
        newPlayerPacket
    );

    std::cout
        << "[World] 플레이어 입장 id: "
        << command.entityId
        << " / 현재 인원: "
        << mPlayers.size()
        << '\n';
}

void ServerWorld::HandleCommand(const LeaveCommand& command)
{
    auto iter = mPlayers.find(command.entityId);

    if (iter == mPlayers.end()) //이미 존재 안하는 클라이언트면 삭제
        return;

    mPlayers.erase(iter);

    Pkt_S_Leave packet =
        MakeLeavePacket(command.entityId);

    BroadcastExcept(
        command.entityId,
        packet
    );

    std::cout
        << "[World] 플레이어 퇴장 id: "
        << command.entityId
        << " / 현재 인원: "
        << mPlayers.size()
        << '\n';
}

void ServerWorld::HandleCommand(const MoveCommand& command)
{
    if (!IsValidPosition(command.position))
        return;

    if (!IsFinite(command.yaw))
        return;

    auto iter = mPlayers.find(command.entityId);

    if (iter == mPlayers.end())
        return;

    ServerPlayer& player = iter->second;

    if (!player.alive)
        return;

    player.position = command.position;
    player.yaw = command.yaw;

    Pkt_S_Move packet = MakeMovePacket(player);

    
    BroadcastExcept(player.entityId, packet);

}

void ServerWorld::HandleCommand(const StateCommand& command)
{
    if (command.state != ePlayerState::IDLE &&
        command.state != ePlayerState::WALK) //hit, death는 서버데미지 판정쪽에서
    {
        return;
    }


    auto iter = mPlayers.find(command.entityId);

    if (iter == mPlayers.end())
        return;

    ServerPlayer& player = iter->second;

    if (!player.alive)
        return;

    if (player.state == command.state)
        return;

    player.state = command.state;

    Pkt_S_State packet = MakeStatePacket(player);


    BroadcastExcept(player.entityId, packet);
}

void ServerWorld::HandleCommand(const WeaponChangeCommand& command)
{
    if (!IsValidWeaponType(command.weaponType))
        return;

    auto iter = mPlayers.find(command.entityId);

    if (iter == mPlayers.end())
        return;

    ServerPlayer& player = iter->second;

    if (!player.alive)
        return;

    if (player.weaponType == command.weaponType)
        return;

    player.weaponType = command.weaponType;

    Pkt_S_WeaponChange packet = MakeWeaponChangePacket(player);


    BroadcastExcept(player.entityId, packet);
}

void ServerWorld::HandleCommand(const AttackCommand& command)
{
    auto iter = mPlayers.find(command.entityId);

    if (iter == mPlayers.end())
        return;

    ServerPlayer& player = iter->second;

    if (!player.alive)
        return;

    if (!IsValidAttackIndex(
        player.weaponType,
        command.attackIndex))
    {
        return;
    }

    ServerVec3 direction = command.direction;

    const float lengthSquared =
        direction.x * direction.x +
        direction.y * direction.y +
        direction.z * direction.z;

    if (!IsFinite(lengthSquared) ||
        lengthSquared < 0.000001f)
    {
        return;
    }

    const float inverseLength =
        1.0f / std::sqrt(lengthSquared);

    direction.x *= inverseLength;
    direction.y *= inverseLength;
    direction.z *= inverseLength;

    AttackCommand normalizedCommand = command;
    normalizedCommand.direction = direction;

    Pkt_S_Attack packet =
        MakeAttackPacket(
            player,
            normalizedCommand
        );

    BroadcastExcept(
        player.entityId,
        packet
    );

    // 다음 단계:
    // if (player.weaponType == eWeaponType::Gun)
    //     SpawnProjectile(player, normalizedCommand);
}

void ServerWorld::UpdateMonsters(float deltaTime)
{
    for(const auto&)
}

void ServerWorld::UpdateProjectiles(float deltaTime)
{
}


void ServerWorld::SendMonsterSnapshotTo( //클라이언트마다 몬스터 생성하게 함
    EntityId targetPlayerId)
{
    for (const auto& [monsterId, monster] : mMonsters)
    {
        if (!monster.alive)
            continue;

        const Pkt_S_MonsterSpawn packet =
            MakeMonsterSpawnPacket(monster);

        SendTo(
            targetPlayerId,
            packet
        );
    }
}

EntityId ServerWorld::SpawnMonster(
    eModelType modelType,
    eWeaponType weaponType,
    const ServerVec3& position,
    float yaw,
    bool broadcast)
{
    if (!mAllocateEntityIdCallback)
    {
        std::cout
            << "[World] EntityId allocator가 없습니다."
            << '\n';

        return 0;
    }



    const EntityId monsterId =
        mAllocateEntityIdCallback();

    ServerMonster monster = {};

    monster.entityId = monsterId;
    monster.modelType = modelType;
    monster.weaponType = weaponType;
    monster.state = eMonsterState::IDLE;

    monster.position = position;
    monster.yaw = yaw;

    monster.maxHp = 100.0f;
    monster.hp = monster.maxHp;
    monster.alive = true;

    auto [iter, inserted] =
        mMonsters.emplace(
            monsterId,
            monster
        );

    if (!inserted)
        return 0;

    if (broadcast)
    {
        const Pkt_S_MonsterSpawn packet =
            MakeMonsterSpawnPacket(iter->second);

        // EntityId 0은 사용하지 않으므로
        // exceptId = 0이면 모든 entered 클라이언트에게 전송
        BroadcastExcept(0, packet);
    }

    std::cout
        << "[World] 몬스터 생성 id: "
        << monsterId
        << " / 위치: "
        << position.x << ", "
        << position.y << ", "
        << position.z
        << '\n';

    return monsterId;
}
