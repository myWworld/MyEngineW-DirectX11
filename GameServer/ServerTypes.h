#pragma once

#include "../MyEngine_Source/Protocol.h"

#include <cstdint>
#include <variant>

struct ServerVec3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct AnimationActionMeta
{
    float duration = 0.0f;
    float hitNormalizedTime = 0.0f;
};

struct ServerPlayer
{
    EntityId entityId = 0;

    ServerVec3 position;
    float yaw = 0.0f;

    eModelType modelType =
        eModelType::Character;

    eWeaponType weaponType =
        eWeaponType::Gun;

    ePlayerState state =
        ePlayerState::IDLE;

    float hp = 100.0f;
    bool alive = true;

    float colliderRadius = 25.0f;
    float colliderHalfHeight = 125.0f;

    float attackCooldown = 0.0f;
};

struct ServerMonster
{
    EntityId entityId = 0;

    eModelType modelType =
        eModelType::Mutant;

    eWeaponType weaponType =
        eWeaponType::Gauntlet;

    eMonsterState state =
        eMonsterState::IDLE;

    ServerVec3 position;
    float yaw = 0.0f;

    float hp = 100.0f;
    float maxHp = 100.0f;
    bool alive = true;

    EntityId targetPlayerId = 0;

    float colliderRadius = 35.0f;
    float colliderHalfHeight = 125.0f;

    // 순찰
    ServerVec3 patrolOrigin;
    ServerVec3 patrolTarget;
    bool hasPatrolTarget = false;

    std::string currentActionAnimation;

    float actionElapsedTime = 0.0f;
    float actionDuration = 0.0f;

    float attackHitNormalizedTime = 0.35f;
    bool attackHitProcessed = false;

    // 복제
    bool transformDirty = false;
    bool stateDirty = false;

    float moveReplicationTimer = 0.0f;

    // 공격 이벤트
    bool attackEventPending = false;

    std::uint8_t attackIndex = 0;

    EntityId attackTargetId = 0;
    ServerVec3 attackDirection;

    // DESTROY 상태
    bool destroyRequested = false;
};

struct EnterCommand
{
    EntityId entityId = 0;

    eModelType modelType =
        eModelType::Character;

    eWeaponType weaponType =
        eWeaponType::Gun;

    ServerVec3 position;
    float yaw = 0.0f;
};

struct LeaveCommand
{
    EntityId entityId = 0;
};

struct MoveCommand
{
    EntityId entityId = 0;

    ServerVec3 position;
    float yaw = 0.0f;
};

struct StateCommand
{
    EntityId entityId = 0;

    ePlayerState state =
        ePlayerState::IDLE;
};

struct WeaponChangeCommand
{
    EntityId entityId = 0;

    eWeaponType weaponType =
        eWeaponType::Gun;
};

struct AttackCommand
{
    EntityId entityId = 0;

    std::uint8_t attackIndex = 0;

    ServerVec3 direction;
};

using WorldCommand = std::variant<
    EnterCommand,
    LeaveCommand,
    MoveCommand,
    StateCommand,
    WeaponChangeCommand,
    AttackCommand
>;