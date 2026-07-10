#include "MERemoteMonsterScript.h"

#include "MEGameObject.h"
#include "METransform.h"
#include "MEAnimator3D.h"
#include "MEWeaponScript.h"

namespace ME
{
    RemoteMonsterScript::RemoteMonsterScript()
    {
    }

    RemoteMonsterScript::~RemoteMonsterScript()
    {
    }

    void RemoteMonsterScript::Initialize()
    {
        ActorScript::Initialize();

        CacheComponents();
        CacheBones();
    }

    void RemoteMonsterScript::Update()
    {
        // 원격 플레이어는 입력 처리 X
        // 원격 플레이어는 NetworkManager::SendPacket X
        // 원격 플레이어는 카메라 기준 회전 X

        CacheComponents();
        CacheBones();
    }

    void RemoteMonsterScript::LateUpdate()
    {
    }

    void RemoteMonsterScript::Render()
    {
        ActorScript::Render();
    }

    void RemoteMonsterScript::OnCollisionEnter(Collider* other)
    {
        // 원격 플레이어는 로컬 충돌로 데미지 처리하지 않는 걸 추천.
        // 데미지는 S_DAMAGE, S_STATE 같은 서버 패킷으로 반영.
    }

    void RemoteMonsterScript::OnCollisionStay(Collider* other)
    {
    }

    void RemoteMonsterScript::OnCollisionExit(Collider* other)
    {
    }

    void RemoteMonsterScript::OnPrimaryAction()
    {
        // 원격 플레이어는 내 입력으로 공격하지 않음.
    }

    void RemoteMonsterScript::OnToggleWeapon()
    {
        // 원격 플레이어는 내 입력으로 무기 전환하지 않음.
        // 무기 전환도 나중에 S_WEAPON_CHANGE 같은 패킷으로 처리.
    }

    void RemoteMonsterScript::OnDeath()
    {
        ApplyState(eMonsterState::DEATH);
    }

    void RemoteMonsterScript::DamageProcess(DamageInfo damageInfo)
    {
        // 원격 플레이어의 HP/데미지는 로컬 충돌로 계산하지 않는 게 좋음.
        // 나중에 서버가 S_DAMAGE 또는 S_STATE로 알려주는 값만 반영.
    }

    Bone* RemoteMonsterScript::GetWeaponSocketBone()
    {
        CacheBones();
        return mLeftHandBone;
    }

    Bone* RemoteMonsterScript::GetWeaponSocketBoneRight()
    {
        CacheBones();
        return mRightHandBone;
    }

    Vector3 RemoteMonsterScript::GetAimDirection()
    {
        // 원격 플레이어는 renderer::mainCamera를 쓰면 안 됨.
        // 일단 기본 전방값 반환.
        // 엔진에 Transform::GetForward()가 있으면 그걸 쓰는 게 더 좋음.
        return Vector3::Forward;
    }

    void RemoteMonsterScript::ApplyMove(float x, float y, float z, float yaw)
    {
        CacheComponents();

        if (mTransform == nullptr)
            return;

        mTransform->SetPosition(x, y, z);

        Vector3 rot = mTransform->GetRotation();
        rot.y = yaw;
        mTransform->SetRotation(rot);
    }



    void RemoteMonsterScript::ApplyAttack(
        eWeaponType weaponType,
        std::uint8_t attackIndex,
        const Vector3& direction)
    {
     

    }

    void RemoteMonsterScript::RegisterWeapon(
        eWeaponType type,
        WeaponScript* weapon)
    {
        if (weapon == nullptr)
            return;

        mWeaponMap[type] = weapon;

        weapon->WeaponOnOff(true);
    }


    void RemoteMonsterScript::ApplyState(eMonsterState state, bool forced)
    {
        CacheComponents();

  
    }

    void RemoteMonsterScript::CacheComponents()
    {
        if (mAnimator == nullptr)
        {
            mAnimator = GetOwner()->GetComponent<Animator3D>();
        }

        if (mTransform == nullptr)
        {
            mTransform = GetOwner()->GetComponent<Transform>();
        }
    }

    void RemoteMonsterScript::CacheBones()
    {
        CacheComponents();

        if (mAnimator == nullptr)
            return;

        if (mLeftHandBone != nullptr && mRightHandBone != nullptr)
            return;

        Skeleton* skeleton = mAnimator->GetSkeletonPtr();

        if (skeleton == nullptr)
            return;

        mLeftHandBone = skeleton->GetLeftHandTransform();
        mRightHandBone = skeleton->GetRightHandTransform();
    }
}