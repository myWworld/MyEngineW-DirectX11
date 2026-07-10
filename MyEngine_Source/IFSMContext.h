#pragma once

#include <string>
#include <vector>

class IFSMContext //서버는 기존에 쓰던 FSM을 사용하지 못하기 때문에 interface형식으로 따로 분리해서 받고 이 내부 함수에서 다르게 구현
{
public:
    virtual ~IFSMContext() = default;

    virtual float GetDeltaTime() const = 0;

    // 타겟 관련
    virtual bool DetectTarget(float radius) = 0;
    virtual bool HasTarget() const = 0;
    virtual float GetTargetDistanceSquared() const = 0;

    // 이동 관련
    virtual void SelectRandomPatrolTarget(float radius) = 0;

    // 반환값:
    // true  = 이동 중
    // false = 목적지 도착 또는 이동 불가
    virtual bool MoveToPatrolTarget(
        float speed,
        float stoppingDistance) = 0;

    virtual bool MoveToTarget(
        float speed,
        float stoppingDistance) = 0;

    // 표현/상태 관련
    virtual void PlayAnimation(
        const std::string& animationName,
        bool loop) = 0;

    virtual bool IsAnimationFinished() const = 0;

    // 공격 관련
    virtual void BeginMeleeAttack(
        const std::vector<std::string>& animationNames) = 0;

    // 제거 관련
    virtual void DestroyOwner() = 0;
};