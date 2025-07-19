#include "MEPlayerScript.h"
#include "METransform.h"
#include "MEInput.h"
#include "MEGameObject.h"
#include "METime.h"
#include "MEAnimator3D.h"
#include "MERenderer.h"

namespace ME
{
    PlayerScript::PlayerScript()
        :mPrevMousePos(Vector2(0, 0))
        , mMouseSpeed(0.5f)
        , mbIsMoving(false)
    {
    }
    PlayerScript::~PlayerScript()
    {
    }
    void PlayerScript::Initialize()
    {
    }
    void PlayerScript::Update()
    {
        if (mAnimator == nullptr)
        {
            mAnimator = GetOwner()->GetComponent< Animator3D>();
        }

        if (mbUseHands)
        {
            if (mLeftHandBone == nullptr && mRightHandBone == nullptr)
            {
                Skeleton* skeleton  = mAnimator->GetSkeleton();
                mLeftHandBone = skeleton->GetLeftHandTransform();
                mRightHandBone = skeleton->GetRightHandTransform();
            }
        }

        Translate();
   
        switch (mState)
        {
        case ME::PlayerScript::State::Idle:
            Idle();
            break;
        case ME::PlayerScript::State::Walk:
            Move();
            break;
        case ME::PlayerScript::State::Run:
            Move();
            break;
        case ME::PlayerScript::State::Attack:
            Attack();
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

    void  PlayerScript::Idle()
    {
        mAnimator->PlayAnimation(L"PISTOLIDLE");
    }
    void PlayerScript::Move()
    {
        if (mState == State::Walk)
        {
            mAnimator->PlayAnimation(L"PISTOLWALK");
        }
        
        if (mState == State::Run)
        {

        }
    }
    void PlayerScript::Attack()
    {

    }
    void PlayerScript::Die()
    {

    }

    void PlayerScript::Translate()
    {

        Transform* tr = GetOwner()->GetComponent<Transform>();
        Vector3 pos = tr->GetPosition();

        bool forward = Input::GetKey(eKeyCode::Up) || Input::GetKey(eKeyCode::W);
        bool back = Input::GetKey(eKeyCode::Down) || Input::GetKey(eKeyCode::S);
        bool left = Input::GetKey(eKeyCode::Left) || Input::GetKey(eKeyCode::A);
        bool right = Input::GetKey(eKeyCode::Right) || Input::GetKey(eKeyCode::D);

        if ( !forward&& !back && !left && !right)
        {
            mState = State::Idle;
            mbIsMoving = false;
        }
        else
        {
 
            if (left)
            {
                pos += 20.0f * tr->Forward() * Time::DeltaTime();
                mTargetDirection = Direction::Left;
            }
            if (Input::GetKey(eKeyCode::Right) || Input::GetKey(eKeyCode::D))
            {
                pos += 20.0f * tr->Forward() * Time::DeltaTime();
                mTargetDirection = Direction::Right;
            }
            if (Input::GetKey(eKeyCode::Up) || Input::GetKey(eKeyCode::W))
            {
                pos += 20.0f * tr->Forward() * Time::DeltaTime();
                mTargetDirection = Direction::Forward;

            }
            if (Input::GetKey(eKeyCode::Down) || Input::GetKey(eKeyCode::S))
            {
                mTargetDirection = Direction::Back;
                pos += 20.0f * tr->Forward() * Time::DeltaTime();
            }

        
          
            mbIsMoving = true;
            mState = State::Walk;
       
            mDirection = mTargetDirection;
      
            tr->SetPosition(pos);
        }

        directionChange();

        if (Input::GetKeyDown(eKeyCode::T))
        {
            if (mbHoldingGun == false)
                mbHoldingGun = true;
            else
                mbHoldingGun = false;
        }

   
    }



    void PlayerScript::directionChange()
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
}