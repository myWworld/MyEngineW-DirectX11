#include "MEPlayerScript.h"
#include "METransform.h"
#include "MEInput.h"
#include "MEGameObject.h"
#include "METime.h"
#include "MEAnimator3D.h"


namespace ME
{
    PlayerScript::PlayerScript()
        :mPrevMousePos(Vector2(0, 0))
        , mMouseSpeed(0.5f)
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
        mAnimator->PlayAnimation(L"Idle");
    }
    void PlayerScript::Move()
    {
        if (mState == State::Walk)
        {
            mAnimator->PlayAnimation(L"FORWARDWALK");
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

        if (Input::GetKey(eKeyCode::Left))
        {
            pos += 20.0f * -tr->Right() * Time::DeltaTime();
        }
        if (Input::GetKey(eKeyCode::Right))
        {
            pos += 20.0f * tr->Right() * Time::DeltaTime();
        }
        if (Input::GetKey(eKeyCode::Space))
        {
            pos += 20.0f * tr->Forward() * Time::DeltaTime();
            mState = State::Walk;
        }
        if (Input::GetKey(eKeyCode::Down))
        {
            pos += 20.0f * -tr->Forward() * Time::DeltaTime();
        }
        // if (Input::GetKey(eKeyCode::E))
        // {
        //     pos += 20.0f * tr->Up() * Time::DeltaTime();
        // }
        // if (Input::GetKey(eKeyCode::Q))
        // {
        //     pos += 20.0f * -tr->Up() * Time::DeltaTime();
        // }
       
        //tr->SetPosition(pos);

        if (Input::GetKeyUp(eKeyCode::Left) &&
            Input::GetKeyUp(eKeyCode::Right) &&
            Input::GetKeyUp(eKeyCode::Up) &&
            Input::GetKeyUp(eKeyCode::Down))
        {
            mState = State::Idle;
        }

    }
}