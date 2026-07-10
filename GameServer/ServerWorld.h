#pragma once

#include "ServerTypes.h"

#include <atomic>
#include <cstdint>
#include <functional>
#include <mutex>
#include <queue>
#include <unordered_map>

class ServerWorld
{
public:
    using SendToCallback =
        std::function<void(
            EntityId targetId,
            const void* packetData,
            std::uint16_t packetSize)>;

    using BroadcastExceptCallback =
        std::function<void(
            EntityId exceptId,
            const void* packetData,
            std::uint16_t packetSize)>;

    using MarkEnteredCallback =
        std::function<void(
            EntityId entityId,
            bool entered)>;

    using AllocateEntityIdCallback =
        std::function<EntityId()>;
public:
    ServerWorld() = default;
    ~ServerWorld() = default;

    void SetNetworkCallbacks(
        SendToCallback sendTo,
        BroadcastExceptCallback broadcastExcept,
        MarkEnteredCallback markEntered);

    void SetAllocateEntityIdCallback(AllocateEntityIdCallback allocator);

    void EnqueueCommand(WorldCommand command);

    void EnsureWorldInitialization();

    void Run();
    void Stop();

    EntityId SpawnMonster(
        eModelType modelType,
        eWeaponType weaponType,
        const ServerVec3& position,
        float yaw,
        bool broadcast);

private:
    void ProcessCommands();
    void Tick(float deltaTime);

    void HandleCommand(const EnterCommand& command);
    void HandleCommand(const LeaveCommand& command);
    void HandleCommand(const MoveCommand& command);
    void HandleCommand(const StateCommand& command);
    void HandleCommand(const WeaponChangeCommand& command);
    void HandleCommand(const AttackCommand& command);


    void UpdateMonsters(float deltaTime);
    void UpdateProjectiles(float deltaTime);

    template <typename T>
    void SendTo(EntityId targetId, const T& packet)
    {
        if (!mSendToCallback)
            return;

        mSendToCallback(
            targetId,
            &packet,
            packet.header.size
        );
    }

    void SendMonsterSnapshotTo(EntityId targetPlayerId);



    template <typename T>
    void BroadcastExcept(EntityId exceptId, const T& packet)
    {
        if (!mBroadcastExceptCallback)
            return;

        mBroadcastExceptCallback(
            exceptId,
            &packet,
            packet.header.size
        );
    }

private:
    // 이 맵은 ServerWorld 스레드만 수정
    std::unordered_map<EntityId, ServerPlayer> mPlayers;
    std::unordered_map<EntityId, ServerMonster> mMonsters;

    std::queue<WorldCommand> mCommandQueue;
    std::mutex mCommandMutex;

    std::atomic<bool> mbRunning = false;

    SendToCallback mSendToCallback;
    BroadcastExceptCallback mBroadcastExceptCallback;
    MarkEnteredCallback mMarkEnteredCallback;
    AllocateEntityIdCallback  mAllocateEntityIdCallback;

    std::atomic<bool> mbInitialized = false;
};