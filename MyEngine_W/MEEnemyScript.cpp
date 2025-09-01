#include "MEEnemyScript.h"
#include "METransform.h"
#include "MEInput.h"
#include "MEGameObject.h"
#include "METime.h"
#include "MEAnimator3D.h"
#include "MERenderer.h"

namespace ME
{
    EnemyScript::EnemyScript()
        :mPrevMousePos(Vector2(0, 0))
        , mMouseSpeed(0.5f)
        , mbIsMoving(false)
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

        if (mbUseHands)
        {
            if (mLeftHandBone == nullptr && mRightHandBone == nullptr)
            {
                Skeleton* skeleton = mAnimator->GetSkeleton();
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

        int randDir = rand() % (int)Direction::End;
        mTargetDirection = (Direction)randDir;
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

        dir.Normalize();
        pos += dir * 30.0f;

            mbIsMoving = true;
            mState = State::Patrol;

            mDirection = mTargetDirection;

            tr->SetPosition(pos);
        

     //   directionChange();


    }



    void EnemyScript::directionChange()
    {


        Transform* tr = GetOwner()->GetComponent<Transform>();

        Vector3 camForward = renderer::mainCamera->GetForward();
        camForward.y = 0.0f;
        camForward.Normalize();

        if (camForward.LengthSquared() > 0.0001f)
        {
            float yawRad = atan2f(camForward.x, camForward.z);
            float yawDeg = XMConvertToDegrees(yawRad);

            Vector3 rt = tr->GetRotation();
            rt.y = yawDeg;
            tr->SetRotation(rt);
        }


        //if (mDirection == Direction::Forward && mTargetDirection == Direction::Back
        //    || mDirection == Direction::Back && mTargetDirection == Direction::Forward
        //    || mDirection == Direction::Left && mTargetDirection == Direction::Right
        //    || mDirection == Direction::Right && mTargetDirection == Direction::Left)
        //{
        //    curRotation.y += 180.0f;
        //    tr->SetRotation(curRotation);
        //}
        //else if (mDirection == Direction::Forward && mTargetDirection == Direction::Left
        //    || mDirection == Direction::Left && mTargetDirection == Direction::Back
        //    || mDirection == Direction::Back && mTargetDirection == Direction::Right
        //    || mDirection == Direction::Right && mTargetDirection == Direction::Forward)
        //{
        //    curRotation.y -= 90.0f;
        //    tr->SetRotation(curRotation);
        //}
        //else if (mDirection == Direction::Forward && mTargetDirection == Direction::Right
        //    || mDirection == Direction::Right && mTargetDirection == Direction::Back
        //    || mDirection == Direction::Back && mTargetDirection == Direction::Left
        //    || mDirection == Direction::Left && mTargetDirection == Direction::Forward)
        //{
        //    curRotation.y += 90.0f;
        //    tr->SetRotation(curRotation);
        //}
        //else if (mDirection == mTargetDirection)
        //    return;

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
            }
            else
            {
                mState = State::Attack;
            }
        
    }
}