#include "MEPlayerScript.h"
#include "METransform.h"
#include "MEInput.h"
#include "MEGameObject.h"
#include "METime.h"
#include "MEAnimator3D.h"
#include "MERenderer.h"
#include "MEInputHandler.h"
#include "MEWeaponScript.h"

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
        if (mbHoldingWeapon)
        {
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
        directionChange();
 
        if (moveAxis.x == 0 && moveAxis.y == 0)
        {
            mState = State::Idle;
            mbIsMoving = false;
            return;
        }

        if (mPlayerTransform == nullptr) return;

        Vector3 forwardDir = mPlayerTransform->Forward();
        Vector3 rightDir = mPlayerTransform->Right();
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

    Vector3 PlayerScript::GetAimDirection()
    {
        return renderer::mainCamera->GetForward();
    }

    
}