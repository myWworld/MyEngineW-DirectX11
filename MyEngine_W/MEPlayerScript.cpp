#include "MEPlayerScript.h"
#include "METransform.h"
#include "MEInput.h"
#include "MEGameObject.h"
#include "METime.h"
#include "MEAnimator3D.h"
#include "MERenderer.h"
#include "MEInputHandler.h"
#include "MEWeaponScript.h"
#include "MESwordScript.h"
#include "../MyEngine_Source/MERigidbody.h"
#include "../MyEngine_Source/MEFSMBrain.h"
#include "../MyEngine_Source/MEFSMState.h"

namespace ME
{
    PlayerScript::PlayerScript()
        :mPrevMousePos(Vector2(0, 0))
        , mMouseSpeed(0.5f)
        , mbIsMoving(false)
        , mPlayerType(PlayerType::Player)
		, mPlayerTransform(nullptr)
		, mInputHandler()
		, mState(State::Idle)
    {
    }
    PlayerScript::~PlayerScript()
    {
    }
    void PlayerScript::Initialize()
    {
        if (mAnimator == nullptr)
        {
            mAnimator = GetOwner()->GetComponent< Animator3D>();
        }



    }
    void PlayerScript::Update()
    {
        if(mState == State::Death)
        {
            return;
		}

        if (mAnimator == nullptr)
        {
            mAnimator = GetOwner()->GetComponent< Animator3D>();
        }

        if (mPlayerTransform == nullptr)
        {
            mPlayerTransform = GetOwner()->GetComponent<Transform>();
        }

        if (mbUseHands)
        {
            if (mLeftHandBone == nullptr && mRightHandBone == nullptr)
            {
                Skeleton* skeleton  = mAnimator->GetSkeletonPtr();
				if (skeleton == nullptr) return;

                mLeftHandBone = skeleton->GetLeftHandTransform();
                mRightHandBone = skeleton->GetRightHandTransform();
            }
        }

        ICommand* command = mInputHandler.HandleActionInput();
        if (command)
        {
            command->Execute(GetOwner());
        }

        if (mState == State::Attack)
        {
            Attack(); 
            return;   
        }



        if (mState != State::Attack)
        {
            Vector2 moveInput = mInputHandler.GetMovementAxis();
            Translate(moveInput);
        }

        switch (mState)
        {
        case ME::PlayerScript::State::Idle:
            Idle();
            break;
        case ME::PlayerScript::State::Walk:
            Move();
            break;
        default:
            break;
        }

        
      
      
    }
    void PlayerScript::LateUpdate()
    {
    }
    void PlayerScript::Render()
    {
    }


    void PlayerScript::OnPrimaryAction()
    {
        if (mbHoldingWeapon)
        {
            mState = State::Attack;
            mEquippedWeapon->Use();

        }
    }

    void PlayerScript::OnToggleWeapon()
    {
		ActorScript::OnToggleWeapon();
    }

    Bone* PlayerScript::GetWeaponSocketBone()
    {
        return mLeftHandBone;
    }

    Bone* PlayerScript::GetWeaponSocketBoneRight()
    {
        return mRightHandBone;
    }

    void  PlayerScript::Idle()
    {
        if (mEquippedWeapon)
            mAnimator->PlayAnimation(mEquippedWeapon->mIdleAnimName);
        else
            mAnimator->PlayAnimation(L"Idle");
    }
    void PlayerScript::Move()
    {
        if (mState == State::Walk)
        {
            if (mEquippedWeapon)
                mAnimator->PlayAnimation(mEquippedWeapon->mWalkAnimName);
            else
                mAnimator->PlayAnimation(L"FORWARDWALK");
        }
        
        if (mState == State::Run)
        {

        }
    }
    void PlayerScript::Attack()
    {

        if (mAnimator->IsAnimationComplete())
        {
            mState = State::Idle;
 
        }
        else
        {
            return;
        }
    }
    void PlayerScript::Die()
    {

    }

    void PlayerScript::Translate(math::Vector2 moveAxis)
    {
        directionChange();
 
        if (moveAxis.x == 0 && moveAxis.y == 0)
        {
            mState = State::Idle;
            mbIsMoving = false;
            return;
        }

        if (mPlayerTransform == nullptr) return;

        Vector3 forwardDir = renderer::mainCamera->GetForward();
        forwardDir.y = 0.0f; 
        forwardDir.Normalize();

		Vector3 rightDir = renderer::mainCamera->GetRight();
        rightDir.y = 0.0f;
        rightDir.Normalize();

        Vector3 moveDir = (forwardDir * moveAxis.y) + (rightDir * moveAxis.x);

        if (moveDir.LengthSquared() > 0.0f)
        {
            moveDir.Normalize();
        }

        Vector3 pos = mPlayerTransform->GetPosition();
        pos += moveDir * 150.0f * Time::DeltaTime();
        mPlayerTransform->SetPosition(pos);

        mbIsMoving = true;
        mState = State::Walk;
        


   
    }



    void PlayerScript::directionChange()
    {
		if (mPlayerTransform == nullptr)
        {
            mPlayerTransform = GetOwner()->GetComponent<Transform>();
        }
        
       

        Vector3 camForward = renderer::mainCamera->GetForward();
        camForward.y = 0.0f;
        camForward.Normalize();

        if (camForward.LengthSquared() > 0.0001f)
        {
            float yawRad = atan2f(camForward.x, camForward.z);
            float yawDeg = XMConvertToDegrees(yawRad);
            yawDeg += 180.0f;

            Vector3 rt = mPlayerTransform->GetRotation();
            rt.y = yawDeg;
            mPlayerTransform->SetRotation(rt);
        }


    
        
    }

    Vector3 PlayerScript::GetAimDirection()
    {
        return renderer::mainCamera->GetForward();
    }

    void PlayerScript::OnDeath()
    {
        mAnimator->PlayAnimation(L"SWORDADEATH");
		mState = State::Death;
    }

    void PlayerScript::OnCollisionEnter(Collider* other)
    {
        GameObject* owner = other->GetOwner();

        if (owner != nullptr &&
            (owner->GetLayerType() == enums::eLayerType::Weapon))
        {

            GameObject* attacker = owner;
            // DamageInfo 패킷 채우기
            DamageInfo damageInfo;
            WeaponScript* weaponScript = attacker->GetComponent<WeaponScript>();

            if (weaponScript)
            {

                if (weaponScript->CanAttack() == false || weaponScript->GetOwnerActor() == GetOwner())
                {
                    return;
                }

                damageInfo.damage = weaponScript->GetDamage();
                damageInfo.attacker = attacker;

                Vector3 attackerPos = attacker->GetComponent<Transform>()->GetPosition();
                Vector3 ownerPos = this->GetOwner()->GetComponent<Transform>()->GetPosition();
                Vector3 dir = (ownerPos - attackerPos);
                dir.Normalize();

                damageInfo.hitPoint = Vector3::Zero; // 피격 위치 raycast로 hit point 확장가능
                damageInfo.knockbackDir = dir; // 공격 반대 방향
                damageInfo.knockbackForce = 10.0f; // 기본값 세팅


                DamageProcess(damageInfo);

				if (mAnimator->GetActiveAnimation()->GetName() != L"SWORDHIT")
                    mAnimator->PlayAnimation(L"SWORDHIT");
            }
        }
    }

    void PlayerScript::OnCollisionStay(Collider* other)
    {
    }

    void PlayerScript::OnCollisionExit(Collider* other)
    {
    }

    void PlayerScript::DamageProcess(DamageInfo damageInfo)
    {

        this->SetHP(damageInfo.damage);

        if (damageInfo.knockbackDir != Vector3::Zero)
        {
            Rigidbody* rb = GetOwner()->GetComponent<Rigidbody>();

            if (rb)
                rb->AddForce(damageInfo.knockbackDir * damageInfo.knockbackForce, Rigidbody::eForceMode::Impulse);

        }

    }

    
}