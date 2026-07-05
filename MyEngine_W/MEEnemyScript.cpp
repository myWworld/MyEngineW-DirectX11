#include "MEEnemyScript.h"
#include "METransform.h"
#include "MEInput.h"
#include "MEGameObject.h"
#include "METime.h"
#include "MEAnimator3D.h"
#include "MERenderer.h"
#include "MEWeaponScript.h"
#include "../MyEngine_Source/MERigidbody.h"
#include "../MyEngine_Source/MEFSMBrain.h"
#include "../MyEngine_Source/MEFSMState.h"
#include "MEBulletScript.h"

namespace ME
{
    EnemyScript::EnemyScript()
        :mPrevMousePos(Vector2(0, 0))
        , mMouseSpeed(0.5f)
        , mbIsMoving(false)
        ,translateTimer(2.5f)
		
    {
    }
    EnemyScript::~EnemyScript()
    {
    }
    void EnemyScript::Initialize()
    {

    }
    void EnemyScript::Update()
    {
        if (mAnimator == nullptr)
        {
            mAnimator = GetOwner()->GetComponent< Animator3D>();
        }
        
        translateTime += Time::DeltaTime();

        OnToggleWeapon();

        if (mbUseHands)
        {
            if (mLeftHandBone == nullptr && mRightHandBone == nullptr)
            {
                Skeleton* skeleton = mAnimator->GetSkeletonPtr();
                mLeftHandBone = skeleton->GetLeftHandTransform();
                mRightHandBone = skeleton->GetRightHandTransform();
            }
        }

        //if (translateTime > translateTimer)
        //{
        //    translateTime = 0.0f;
        //    randomState();
        //}


        //switch (mState)
        //{
        //case ME::EnemyScript::State::Idle:
        //    Idle();
        //    break;
        //case ME::EnemyScript::State::Patrol:
        //    Translate();
        //    Move();
        //    break;
        //case ME::EnemyScript::State::Run:
        //    Move();
        //    break;
        //case ME::EnemyScript::State::Attack:
        //    Attack();
        //    break;
        //default:
        //    break;
        //}




    }
    void EnemyScript::LateUpdate()
    {
    }
    void EnemyScript::Render()
    {
    }

    void EnemyScript::OnPrimaryAction()
    {
        if (mbHoldingWeapon)
        {
            mEquippedWeapon->Use();
        }
    }

    void EnemyScript::OnToggleWeapon()
    {
        if(!mbHoldingWeapon)
            ActorScript::OnToggleWeapon();


    }
  
    void  EnemyScript::Idle()
    {
        mAnimator->PlayAnimation(L"PISTOLIDLE");
    }
    void EnemyScript::Move()
    {
        if (mState == State::Patrol)
        {
            mAnimator->PlayAnimation(L"PISTOLWALK");
        }

        if (mState == State::Run)
        {

        }
    }
    void EnemyScript::Attack()
    {

    }
    void EnemyScript::Die()
    {

    }

    void EnemyScript::Translate()
    {


    }



    //void EnemyScript::directionChange(Vector3 newDir)
    //{


    //    Transform* tr = GetOwner()->GetComponent<Transform>();

    //    Vector3 curDir = tr->Forward();

    //    float dotProduct = curDir.Dot(newDir);
    //    if (dotProduct > 1.0f) dotProduct = 1.0f;
    //    if (dotProduct < -1.0f) dotProduct = -1.0f;

    //    float angle = Degree((acos(curDir.Dot(newDir))));

    //    auto degClamp = [](int newDeg)  -> int {

    //        if (newDeg > 360)
    //        {
    //            return  newDeg -= 360;
    //        }

    //        return newDeg;
    //    };

    //    angle = degClamp(angle);

    //    if (angle > 5.0f)
    //    {
    //        Vector3 rt = tr->GetRotation();
    //        
    //        rt.y = degClamp(rt.y + angle);

    //        
    //        
    //        tr->SetRotation(rt);
    //    }

    // 
    //}
    //void EnemyScript::randomState()
    //{
  
    //        int prob = rand() % 10 + 1;

    //        if (prob <= 3)
    //        {
    //            mState = State::Idle;
    //        }
    //        else if (prob <= 7)
    //        {
    //            mState = State::Patrol;

    //            int randDir = rand() % (int)Direction::End;
    //            mTargetDirection = (Direction)randDir;
    //            mbTurn = false;
    //        }
    //        else
    //        {
    //            mState = State::Attack;
    //        }
    //    
    //}


    Vector3 EnemyScript::GetAimDirection()
    {
        return GetOwner()->GetComponent<Transform>()->Forward();
    }

    void EnemyScript::OnDeath()
    {
        auto brain = GetOwner()->GetComponent<FSMBrain>();

        if (brain)
        {
            brain->SendFSMEvent("DEATH");
        }
    
    }

    void EnemyScript::OnCollisionEnter(Collider* other)
    {
        GameObject* owner = other->GetOwner();

        if (owner != nullptr &&
            (owner->GetLayerType() == enums::eLayerType::Bullet
                || owner->GetLayerType() == enums::eLayerType::Weapon))
        {
            // 공격자 객체 찾기 (Bullet이라면 주인을 찾음)
            GameObject* attacker = owner;
            if (owner->GetLayerType() == enums::eLayerType::Bullet)
            {
                BulletScript* bulletScript = owner->GetComponent<BulletScript>();
                bulletScript->BackToPool();
                attacker = bulletScript->GetGun(); 
            }

            // DamageInfo 패킷 채우기
            DamageInfo damageInfo;
            WeaponScript* weaponScript = attacker->GetComponent<WeaponScript>();

            if (weaponScript)
            {
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

                auto brain = GetOwner()->GetComponent<FSMBrain>();
                if (brain)
                {
                    std::string currentState = brain->mActiveState->GetStateName();

                    if (currentState != "DAMAGE" && this->GetHP() > 0)
                    {
                        brain->SendFSMEvent("DAMAGE");
                    }
                }
            }
        }
    }

    void EnemyScript::OnCollisionStay(Collider* other)
    {
    }

    void EnemyScript::OnCollisionExit(Collider* other)
    {
    }

    void EnemyScript::DamageProcess(DamageInfo damageInfo)
    {

		this->SetHP(damageInfo.damage);

		if (damageInfo.knockbackDir != Vector3::Zero)
		{
			Rigidbody* rb = GetOwner()->GetComponent<Rigidbody>();

            if(rb)
	    		rb->AddForce(damageInfo.knockbackDir * damageInfo.knockbackForce, Rigidbody::eForceMode::Impulse);

		}
        
    }


}