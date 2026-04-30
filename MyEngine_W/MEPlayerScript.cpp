#include "MEPlayerScript.h"
#include "METransform.h"
#include "MEInput.h"
#include "MEGameObject.h"
#include "METime.h"
#include "MEAnimator3D.h"
#include "MERenderer.h"
#include "MEInputHandler.h"

namespace ME
{
    PlayerScript::PlayerScript()
        :mPrevMousePos(Vector2(0, 0))
        , mMouseSpeed(0.5f)
        , mbIsMoving(false)
        , mPlayerType(PlayerType::Player)
		, mPlayerTransform(nullptr)
		, mAnimator(nullptr)
		, mInputHandler()
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

        if (mPlayerTransform == nullptr)
        {
            mPlayerTransform = GetOwner()->GetComponent<Transform>();
        }

        if (mbUseHands)
        {
            if (mLeftHandBone == nullptr && mRightHandBone == nullptr)
            {
                Skeleton* skeleton  = mAnimator->GetSkeletonPtr();
                mLeftHandBone = skeleton->GetLeftHandTransform();
                mRightHandBone = skeleton->GetRightHandTransform();
            }
        }

		ICommand* command = mInputHandler.HandleActionInput();
        if(command)
        {
            command->Execute(GetOwner());
		}

        Vector2 moveInput = mInputHandler.GetMovementAxis();

        Translate(moveInput);

   
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

    void PlayerScript::OnCollisionEnter(Collider* other)
    {
        GameObject* owner = other->GetOwner();

        if (owner != nullptr && owner->GetLayerType() == enums::eLayerType::Bullet)
        {
            mAnimator->PlayAnimation(L"HIT");
        }
    }

    void PlayerScript::OnCollisionStay(Collider* other)
    {
    }

    void PlayerScript::OnCollisionExit(Collider* other)
    {
    }

    void PlayerScript::OnPrimaryAction()
    {
        if (mbHoldingGun)
        {

        }
    }

    void PlayerScript::OnToggleWeapon()
    {
		ActorScript::OnToggleWeapon();
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

    void PlayerScript::Translate(math::Vector2 moveAxis)
    {
       
 
        if (moveAxis.x == 0 && moveAxis.y == 0)
        {
            mState = State::Idle;
            mbIsMoving = false;
            return;
        }

        if (mPlayerTransform == nullptr) return;

        if (moveAxis.y > 0) mTargetDirection = Direction::Forward;
        else if (moveAxis.y < 0) mTargetDirection = Direction::Back;
        else if (moveAxis.x > 0) mTargetDirection = Direction::Right;
        else if (moveAxis.x < 0) mTargetDirection = Direction::Left;

            
        Vector3 pos = mPlayerTransform->GetPosition();
        pos += 150.0f * mPlayerTransform->Forward() * Time::DeltaTime();
        mPlayerTransform->SetPosition(pos);

        mbIsMoving = true;
        mState = State::Walk;
        mDirection = mTargetDirection;

        directionChange();
        


   
    }

    void PlayerScript::randomAction()
    {
        int prob = rand() % 10 + 1;

        if (prob <= 3)
        {
            mState = State::Idle;
        }
        else if (prob <= 7)
        {
            mState = State::Walk;
        }
        else
        {
            mState = State::Attack;
        }
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

            Vector3 rt = mPlayerTransform->GetRotation();
            rt.y = yawDeg;
            mPlayerTransform->SetRotation(rt);
        }


    
        
    }

    
}