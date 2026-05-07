#include "MEEnemyScript.h"
#include "METransform.h"
#include "MEInput.h"
#include "MEGameObject.h"
#include "METime.h"
#include "MEAnimator3D.h"
#include "MERenderer.h"
#include "MEWeaponScript.h"

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

        if (translateTime > translateTimer)
        {
            translateTime = 0.0f;
            randomState();
        }


        switch (mState)
        {
        case ME::EnemyScript::State::Idle:
            Idle();
            break;
        case ME::EnemyScript::State::Patrol:
            Translate();
            Move();
            break;
        case ME::EnemyScript::State::Run:
            Move();
            break;
        case ME::EnemyScript::State::Attack:
            Attack();
            break;
        default:
            break;
        }




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

        Transform* tr = GetOwner()->GetComponent<Transform>();
        Vector3 pos = tr->GetPosition();

       // int randDir = rand() % (int)Direction::End;
       // mTargetDirection = (Direction)randDir;
        Vector3 dir = Vector3::Zero;

        switch (mTargetDirection)
        {
        case ME::EnemyScript::Direction::Forward:
            dir = tr->Forward();
            break;
        case ME::EnemyScript::Direction::ForwardLeft:
            dir = tr->Forward() - tr->Right();
            break;
        case ME::EnemyScript::Direction::ForwardRight:
            dir = tr->Forward() + tr->Right();
            break;
        case ME::EnemyScript::Direction::Left:
            dir = -tr->Right();
            break;
        case ME::EnemyScript::Direction::Right:
            dir = tr->Right();
            break;
        case ME::EnemyScript::Direction::Back:
            dir = -tr->Forward() ;
            break;
        case ME::EnemyScript::Direction::BackLeft:
            dir = -tr->Forward() - tr->Right();
            break;
        case ME::EnemyScript::Direction::BackRight:
            dir = -tr->Forward() + tr->Right();
            break;
        default:
            break;
        }
        if (dir.LengthSquared() > 0.0f)
        {
            dir.Normalize();
            pos += dir * 150.0f * Time::DeltaTime();

            mbIsMoving = true;
            mState = State::Patrol;

            mDirection = mTargetDirection;

            tr->SetPosition(pos);

            if (mbTurn == false)
            {
                directionChange(dir);
                mbTurn = true;
            }
        }

    }



    void EnemyScript::directionChange(Vector3 newDir)
    {


        Transform* tr = GetOwner()->GetComponent<Transform>();

        Vector3 curDir = tr->Forward();

        float dotProduct = curDir.Dot(newDir);
        if (dotProduct > 1.0f) dotProduct = 1.0f;
        if (dotProduct < -1.0f) dotProduct = -1.0f;

        float angle = Degree((acos(curDir.Dot(newDir))));

        auto degClamp = [](int newDeg)  -> int {

            if (newDeg > 360)
            {
                return  newDeg -= 360;
            }

            return newDeg;
        };

        angle = degClamp(angle);

        if (angle > 5.0f)
        {
            Vector3 rt = tr->GetRotation();
            
            rt.y = degClamp(rt.y + angle);

            
            
            tr->SetRotation(rt);
        }

     
    }
    void EnemyScript::randomState()
    {
  
            int prob = rand() % 10 + 1;

            if (prob <= 3)
            {
                mState = State::Idle;
            }
            else if (prob <= 7)
            {
                mState = State::Patrol;

                int randDir = rand() % (int)Direction::End;
                mTargetDirection = (Direction)randDir;
                mbTurn = false;
            }
            else
            {
                mState = State::Attack;
            }
        
    }


    Vector3 EnemyScript::GetAimDirection()
    {
        return GetOwner()->GetComponent<Transform>()->Forward();
    }

    void EnemyScript::OnCollisionEnter(Collider* other)
    {
        GameObject* owner = other->GetOwner();

        if (owner != nullptr && owner->GetLayerType() == enums::eLayerType::Bullet)
        {
            mAnimator->PlayAnimation(L"HIT");
        }

    }

    void EnemyScript::OnCollisionStay(Collider* other)
    {
    }

    void EnemyScript::OnCollisionExit(Collider* other)
    {
    }


}