#include "ServerWorld.h"


#include "../MyEngine_Source/FSMFactory.h"
#include "../MyEngine_Source/FSMBrainCore.h"
#include "../MyEngine_Source/IFSMContext.h"



#include <algorithm>
#include <limits>
#include <random>
#include <vector>

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

    bool IsValidProjectileOrigin(
        const ServerPlayer& player,
        const ServerVec3& origin)
    {
        if (!std::isfinite(origin.x) ||
            !std::isfinite(origin.y) ||
            !std::isfinite(origin.z))
        {
            return false;
        }

        const float dx =
            origin.x - player.position.x;

        const float dy =
            origin.y - player.position.y;

        const float dz =
            origin.z - player.position.z;

        const float distanceSquared =
            dx * dx +
            dy * dy +
            dz * dz;

        constexpr float MaxMuzzleDistance = 350.0f;

        return distanceSquared <=
            MaxMuzzleDistance *
            MaxMuzzleDistance;
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

    Pkt_S_MonsterMove MakeMonsterMovePacket(
        const ServerMonster& monster)
    {
        Pkt_S_MonsterMove packet = {};

        packet.header.type =
            ePacketType::S_MONSTER_MOVE;

        packet.header.size =
            sizeof(Pkt_S_MonsterMove);

        packet.entityId = monster.entityId;

        packet.x = monster.position.x;
        packet.y = monster.position.y;
        packet.z = monster.position.z;
        packet.yaw = monster.yaw;

        return packet;
    }

    Pkt_S_MonsterState MakeMonsterStatePacket(
        const ServerMonster& monster)
    {
        Pkt_S_MonsterState packet = {};

        packet.header.type =
            ePacketType::S_MONSTER_STATE;

        packet.header.size =
            sizeof(Pkt_S_MonsterState);

        packet.entityId = monster.entityId;
        packet.state = monster.state;

        return packet;
    }

    Pkt_S_MonsterAttack MakeMonsterAttackPacket(
        const ServerMonster& monster)
    {
        Pkt_S_MonsterAttack packet = {};

        packet.header.type =
            ePacketType::S_MONSTER_ATTACK;

        packet.header.size =
            sizeof(Pkt_S_MonsterAttack);

        packet.entityId = monster.entityId;
        packet.targetEntityId =monster.attackTargetId;

        packet.attackIndex =monster.attackIndex;

        packet.dir_x =monster.attackDirection.x;
        packet.dir_y = monster.attackDirection.y;
        packet.dir_z = monster.attackDirection.z;

        return packet;
    }

    Pkt_S_MonsterDespawn MakeMonsterDespawnPacket(
        EntityId entityId)
    {
        Pkt_S_MonsterDespawn packet = {};

        packet.header.type =
            ePacketType::S_MONSTER_DESPAWN;

        packet.header.size =
            sizeof(Pkt_S_MonsterDespawn);

        packet.entityId = entityId;

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
    InitializeMonsterAnimationMeta();
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
        player.attackCooldown = (std::max)(0.0f,player.attackCooldown - deltaTime);
    }

    for (auto& [monsterId, monster] : mMonsters)
    {
        if (monster.actionDuration <= 0.0f)
            continue;

        monster.actionElapsedTime += deltaTime;

        if (monster.actionElapsedTime >
            monster.actionDuration)
        {
            monster.actionElapsedTime =
                monster.actionDuration;
        }

        const float normalizedTime =
            monster.actionElapsedTime /
            monster.actionDuration;

        if (!monster.attackHitProcessed &&
            normalizedTime >=
            monster.attackHitNormalizedTime)
        {
            monster.attackHitProcessed = true;

            // 이후 데미지 판정
            // ResolveMonsterMeleeAttack(monster);
        }
    }

    UpdateMonsters(deltaTime);
    UpdateProjectiles(deltaTime);

    FlushMonsterReplication(deltaTime);
    DespawnRequestedMonsters();
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

    // 서버 쿨타임 검증
    if (player.attackCooldown > 0.0f)
        return;

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

    // 무기별 서버 쿨타임
    switch (player.weaponType)
    {
    case eWeaponType::Gun:
        player.attackCooldown = 0.2f;
        break;

    case eWeaponType::Sword:
        player.attackCooldown = 0.5f;
        break;

    default:
        return;
    }

    // 원격 플레이어 공격 애니메이션
    Pkt_S_Attack attackPacket =
        MakeAttackPacket(
            player,
            normalizedCommand
        );

    BroadcastExcept(
        player.entityId,
        attackPacket
    );

    // 총이면 서버 총알 생성
    if (player.weaponType ==
        eWeaponType::Gun)
    {
        SpawnProjectile(
            player,
            normalizedCommand
        );
    }
}

void ServerWorld::UpdateMonsters(float deltaTime)
{
    for (auto& [monsterId, monster] : mMonsters)
    {
        if (!monster.alive &&
            monster.state != eMonsterState::DEATH)
        {
            auto brainIter = mMonsterBrains.find(monsterId);

            if (brainIter != mMonsterBrains.end())
            {
                brainIter->second->SendFSMEvent("DEATH");
            }
        }

        auto brainIter = mMonsterBrains.find(monsterId);

        if (brainIter == mMonsterBrains.end())
            continue;

        ServerMonsterFSMContext context(
            *this,
            monster,
            deltaTime
        );

        brainIter->second->Update(context);
    }
}

void ServerWorld::UpdateProjectiles(float deltaTime)
{
    std::vector<ProjectileId> expiredProjectiles;

    for (auto& [projectileId, projectile] : mProjectiles)
    {
        projectile.previousPosition =
            projectile.position;

        projectile.position.x +=
            projectile.velocity.x *
            deltaTime;

        projectile.position.y +=
            projectile.velocity.y *
            deltaTime;

        projectile.position.z +=
            projectile.velocity.z *
            deltaTime;

        projectile.remainingLife -=
            deltaTime;

        if (projectile.remainingLife <= 0.0f)
        {
            expiredProjectiles.push_back(projectileId);
        }
    }

    for (ProjectileId projectileId :  expiredProjectiles)
    {
        auto iter = mProjectiles.find(projectileId);

        if (iter == mProjectiles.end())
            continue;

        Pkt_S_ProjectileEnd packet = {};

        packet.header.type =
            ePacketType::S_PROJECTILE_END;

        packet.header.size =
            sizeof(Pkt_S_ProjectileEnd);

        packet.projectileId =
            projectileId;

        packet.reason =
            eProjectileEndReason::Expired;

        packet.hitEntityId = 0;

        packet.end_x =
            iter->second.position.x;

        packet.end_y =
            iter->second.position.y;

        packet.end_z =
            iter->second.position.z;

        BroadcastExcept(0, packet);

        mProjectiles.erase(iter);
    }
}

void ServerWorld::SpawnProjectile(
    const ServerPlayer& player,
    const AttackCommand& command)
{
    constexpr float BulletSpeed = 2000.0f;
    constexpr float BulletLifeTime = 8.0f;
    constexpr float MuzzleHeight = 120.0f;
    constexpr float MuzzleForwardOffset = 60.0f;

    ServerVec3 direction = command.direction;
    ServerVec3 spawnPosition = command.origin;


    if (!IsValidProjectileOrigin(
        player,
        spawnPosition))
    {
        // 이상한 총구 위치면 서버 기준으로 대체
        constexpr float MuzzleHeight = 70.0f;
        constexpr float MuzzleForwardOffset = 60.0f;

        spawnPosition =
        {
            player.position.x +
                command.direction.x *
                MuzzleForwardOffset,

            player.position.y +
                MuzzleHeight +
                command.direction.y *
                MuzzleForwardOffset,

            player.position.z +
                command.direction.z *
                MuzzleForwardOffset
        };
    }

    // HandleCommand에서 이미 정규화했지만
    // 방어적으로 다시 검사
    const float lengthSquared =
        direction.x * direction.x +
        direction.y * direction.y +
        direction.z * direction.z;

    if (lengthSquared < 0.000001f)
        return;

    const ProjectileId projectileId = mNextProjectileId++;

    ServerProjectile projectile = {};

    projectile.projectileId =
        projectileId;

    projectile.ownerEntityId =
        player.entityId;

    projectile.previousPosition =
        spawnPosition;

    projectile.position =
        spawnPosition;

    projectile.velocity =
    {
        direction.x * BulletSpeed,
        direction.y * BulletSpeed,
        direction.z * BulletSpeed
    };

    projectile.radius = 3.0f;
    projectile.damage = 5.0f;
    projectile.remainingLife =
        BulletLifeTime;

    mProjectiles.emplace(
        projectileId,
        projectile
    );

    Pkt_S_ProjectileSpawn packet = {};

    packet.header.type = ePacketType::S_PROJECTILE_SPAWN;

    packet.header.size = sizeof(Pkt_S_ProjectileSpawn);

    packet.projectileId = projectile.projectileId;

    packet.ownerEntityId = projectile.ownerEntityId;

    packet.start_x =projectile.position.x;
    packet.start_y =projectile.position.y;
    packet.start_z = projectile.position.z;

    packet.velocity_x =projectile.velocity.x;
    packet.velocity_y =projectile.velocity.y;
    packet.velocity_z = projectile.velocity.z;

    packet.lifeTime = projectile.remainingLife;

    // 발사한 클라이언트를 포함한 모든 클라이언트에게 보낸다.
    BroadcastExcept(0, packet);
}

void ServerWorld::EndProjectile(
    ProjectileId projectileId,
    eProjectileEndReason reason,
    EntityId hitEntityId,
    const ServerVec3& endPosition)
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

    auto brain = std::make_unique<ME::FSMBrainCore>();

    const bool fsmLoaded =
        ME::FSMFactory::MakeFSMWithJsonFile(brain.get(),"..\\Resources\\EnemyFSMJson.json");

    if (!fsmLoaded)
    {
        std::cout
            << "[World] 몬스터 FSM 로딩 실패 id: "
            << monsterId
            << '\n';

        mMonsters.erase(monsterId);
        return 0;
    }

    iter->second.patrolOrigin =
        iter->second.position;

    iter->second.patrolTarget =
        iter->second.position;

    mMonsterBrains.emplace(
        monsterId,
        std::move(brain)
    );

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


EntityId ServerWorld::FindClosestAlivePlayer(
    const ServerVec3& position,
    float maxDistance) const
{
    EntityId closestId = 0;

    float closestDistanceSquared =
        maxDistance * maxDistance;

    for (const auto& [playerId, player] : mPlayers)
    {
        if (!player.alive)
            continue;

        const float distanceSquared =
            DistanceSquaredXZ(
                position,
                player.position
            );

        if (distanceSquared > closestDistanceSquared)
        {
            continue;
        }

        closestDistanceSquared =
            distanceSquared;

        closestId = playerId;
    }

    return closestId;
}

ServerPlayer* ServerWorld::FindAlivePlayer(EntityId entityId)
{
    auto iter = mPlayers.find(entityId);

    if (iter == mPlayers.end() ||
        !iter->second.alive)
    {
        return nullptr;
    }

    return &iter->second;
}

const ServerPlayer* ServerWorld::FindAlivePlayer(EntityId entityId) const
{
    auto iter = mPlayers.find(entityId);

    if (iter == mPlayers.end() ||
        !iter->second.alive)
    {
        return nullptr;
    }

    return &iter->second;
}

float ServerWorld::DistanceSquaredXZ(const ServerVec3& lhs, const ServerVec3& rhs) const
{
    const float dx = rhs.x - lhs.x;
    const float dz = rhs.z - lhs.z;

    return dx * dx + dz * dz;
}

void ServerWorld::SelectRandomPatrolTarget(ServerMonster& monster, float radius)
{
    if (!monster.hasPatrolTarget)
    {
        monster.patrolOrigin =
            monster.position;
    }

    std::uniform_real_distribution<float>
        distribution(-radius, radius);

    monster.patrolTarget =
    {
        monster.patrolOrigin.x +
            distribution(mRandomEngine),
        monster.patrolOrigin.y,
        monster.patrolOrigin.z +
            distribution(mRandomEngine)
    };

    monster.hasPatrolTarget = true; //도착 여부 판단을 위한 flag
}

bool ServerWorld::MoveMonsterToward(
    ServerMonster& monster,
    const ServerVec3& target,
    float speed,
    float stoppingDistance,
    float deltaTime)
{
    float dx = target.x - monster.position.x;

    float dz = target.z - monster.position.z;

    const float distanceSquared =
        dx * dx + dz * dz;

    if (distanceSquared <=
        stoppingDistance * stoppingDistance)
    {
        return false;
    }

    const float distance =
        std::sqrt(distanceSquared);

    if (distance < 0.0001f)
        return false;

    dx /= distance;
    dz /= distance;

    const float remainingDistance =
        (std::max)(
            0.0f,
            distance - stoppingDistance
        );

    const float movement =
        (std::min)(
            speed * deltaTime,
            remainingDistance
        );

    monster.position.x += dx * movement;
    monster.position.z += dz * movement;

    constexpr float RadToDegree =
        57.295779513f;

    monster.yaw =
        std::atan2(dx, dz)
        * RadToDegree
        + 180.0f;

    monster.transformDirty = true; //한번에 업데이트 하기위한 flag

    return true;
}

void ServerWorld::ApplyMonsterAnimation(
    ServerMonster& monster,
    const std::string& animationName,
    bool loop)
{
    eMonsterState nextState =
        monster.state;

    if (animationName == "MONSTER_IDLE")
        nextState = eMonsterState::IDLE;

    else if (animationName == "MONSTER_WALK")
        nextState = eMonsterState::WALK;

    else if (animationName == "MONSTER_RUN")
        nextState = eMonsterState::RUN;

    else if (animationName == "MONSTER_ATTACK")
        nextState = eMonsterState::ATTACK_1;

    else if (animationName == "MONSTER_ATTACK2")
        nextState = eMonsterState::ATTACK_2;

    else if (animationName == "MONSTER_ATTACK3")
        nextState = eMonsterState::ATTACK_3;

    else if (animationName == "MONSTER_HIT")
        nextState = eMonsterState::HIT;

    else if (animationName == "MONSTER_DEATH")
        nextState = eMonsterState::DEATH;

    if (monster.state != nextState)
    {
        monster.state = nextState;
        monster.stateDirty = true;
    }

    if (!loop)
    {
       // monster.actionRemainingTime = GetMonsterAnimationDuration(animationName);
    }
}

float ServerWorld::GetMonsterAnimationDuration(const std::string& animationName) const
{
    if (animationName == "MONSTER_ATTACK")
        return 0.9f;

    if (animationName == "MONSTER_ATTACK2")
        return 1.0f;

    if (animationName == "MONSTER_ATTACK3")
        return 1.1f;

    if (animationName == "MONSTER_HIT")
        return 0.6f;

    if (animationName == "MONSTER_DEATH")
        return 2.0f;

    return 0.0f;
}

const AnimationActionMeta* ServerWorld::FindMonsterAnimationMeta(const std::string& animationName) const
{
    auto iter =
        mMonsterAnimationMeta.find(
            animationName
        );

    if (iter == mMonsterAnimationMeta.end())
        return nullptr;

    return &iter->second;
}

void ServerWorld::BeginMonsterMeleeAttack(ServerMonster& monster, const std::vector<std::string>& animationNames)
{
    if (animationNames.empty())
        return;

    const ServerPlayer* target =
        FindAlivePlayer(
            monster.targetPlayerId
        );

    if (target == nullptr)
        return;

    std::uniform_int_distribution<std::size_t>
        distribution(
            0,
            animationNames.size() - 1
        );

    const std::size_t attackIndex =
        distribution(mRandomEngine);

    const std::string& animationName =
        animationNames[attackIndex];

    const AnimationActionMeta* animationMeta =
        FindMonsterAnimationMeta(
            animationName
        );

    if (animationMeta == nullptr ||
        animationMeta->duration <= 0.0f)
    {
        std::cerr
            << "[Monster FSM] 애니메이션 메타데이터 없음: "
            << animationName
            << '\n';

        return;
    }

    monster.attackIndex = static_cast<std::uint8_t>(attackIndex);

    monster.attackTargetId = target->entityId;

    monster.currentActionAnimation = animationName;

    monster.actionElapsedTime = 0.0f;

    monster.actionDuration = animationMeta->duration;

    monster.attackHitNormalizedTime =
        animationMeta->hitNormalizedTime;

    monster.attackHitProcessed =
        false;


    monster.state = attackIndex == 0 ? eMonsterState::ATTACK_1: attackIndex == 1 ? eMonsterState::ATTACK_2 : eMonsterState::ATTACK_3;

    float dirX =
        target->position.x -
        monster.position.x;

    float dirY =
        target->position.y -
        monster.position.y;

    float dirZ =
        target->position.z -
        monster.position.z;

    const float lengthSquared =
        dirX * dirX +
        dirY * dirY +
        dirZ * dirZ;

    if (lengthSquared > 0.0001f)
    {
        const float inverseLength =
            1.0f / std::sqrt(lengthSquared);

        dirX *= inverseLength;
        dirY *= inverseLength;
        dirZ *= inverseLength;
    }

    monster.attackDirection =
    {
        dirX,
        dirY,
        dirZ
    };

 //   monster.actionRemainingTime = GetMonsterAnimationDuration( animationName);

    monster.attackEventPending = true;

    // S_MONSTER_STATE와 S_MONSTER_ATTACK이
    // 중복으로 공격을 재생하지 않도록 한다.
    monster.stateDirty = false;
}

void ServerWorld::FlushMonsterReplication(float deltaTime) //몬스터 이동, 공격때마다 바로바로 하지않고 flag를 이용하여 한번에 업데이트
{
    constexpr float MoveSendInterval =
        1.0f / 20.0f;

    for (auto& [monsterId, monster] : mMonsters)
    {
        monster.moveReplicationTimer += deltaTime;

        if (monster.stateDirty) //상태 변경이 있는지
        {
            const Pkt_S_MonsterState packet =
                MakeMonsterStatePacket(monster);

            BroadcastExcept(0, packet);

            monster.stateDirty = false;
        }

        if (monster.attackEventPending) //공격 이벤트가 발생했는지
        {
            const Pkt_S_MonsterAttack packet =
                MakeMonsterAttackPacket(monster);

            BroadcastExcept(0, packet);

            monster.attackEventPending = false;
        }

        if (monster.transformDirty &&
            monster.moveReplicationTimer
            >= MoveSendInterval) //움직임이 있는지
        {
            const Pkt_S_MonsterMove packet =
                MakeMonsterMovePacket( monster );

            BroadcastExcept(0, packet);

            monster.transformDirty = false;
            monster.moveReplicationTimer = 0.0f;
        }
    }
}

void ServerWorld::DespawnRequestedMonsters()
{
    std::vector<EntityId> destroyIds;

    for (const auto& [monsterId, monster] :
        mMonsters)
    {
        if (monster.destroyRequested)
        {
            destroyIds.push_back(monsterId);
        }
    }

    for (EntityId monsterId : destroyIds) //특정 몬스터 나간다고 서버내 모든 클라이언트(플레이어)에게 알림
    {
        const Pkt_S_MonsterDespawn packet =
            MakeMonsterDespawnPacket(monsterId);

        BroadcastExcept(0, packet);

        mMonsterBrains.erase(monsterId);
        mMonsters.erase(monsterId);
    }
}

void ServerWorld::InitializeMonsterAnimationMeta()
{
    mMonsterAnimationMeta["MONSTER_ATTACK"] =
    {
        1.1f,
        0.2f
    };

    mMonsterAnimationMeta["MONSTER_ATTACK2"] =
    {
        4.63f,
        0.42f
    };

    mMonsterAnimationMeta["MONSTER_ATTACK3"] =
    {
        3.7f,
        0.30f
    };

    mMonsterAnimationMeta["MONSTER_HIT"] =
    {
        2.73f,
        0.0f
    };

    mMonsterAnimationMeta["MONSTER_DEATH"] =
    {
        4.6f,
        0.0f
    };
}