#include "MECameraScript.h"
#include "METransform.h"
#include "MEInput.h"
#include "MEGameObject.h"
#include "METime.h"


namespace ME
{
    CameraScript::CameraScript()
        :mPrevMousePos(Vector2(0, 0))
        ,mMouseSpeed(0.5f)
    {
    }
    CameraScript::~CameraScript()
    {
    }
    void CameraScript::Initialize()
    {
    }
    void CameraScript::Update()
    {
        Transform* tr = GetOwner()->GetComponent<Transform>();
        Vector3 pos = tr->GetPosition();
        Vector3 rotation = tr->GetRotation();

        mCurMousePos = Input::GetMousePos();
        float curMP = mCurMousePos.Length();
        float prevMP = mPrevMousePos.Length();

        if (abs(curMP - prevMP) > 1.0f)
        {

            Vector2 delta = mCurMousePos - mPrevMousePos;
            mPrevMousePos = mCurMousePos;

            float angle = delta.x * mMouseSpeed;
  
            rotation.y += angle; // Y축(위로 도는 방향) 회전값만 증가

            tr->SetRotation(rotation); // 최종 회전값 반영

        }

        if (Input::GetKey(eKeyCode::A))
        {
            pos += 20.0f * -tr->Right() * Time::DeltaTime();
        }
        if (Input::GetKey(eKeyCode::D))
        {
            pos += 20.0f * tr->Right() * Time::DeltaTime();
        }
        if (Input::GetKey(eKeyCode::W))
        {
            pos += 20.0f* tr->Forward() * Time::DeltaTime();
        }
        if (Input::GetKey(eKeyCode::S))
        {
            pos += 20.0f * - tr->Forward() *Time::DeltaTime();
        }
        if (Input::GetKey(eKeyCode::E))
        {
            pos += 20.0f * tr->Up() * Time::DeltaTime();
        }
        if (Input::GetKey(eKeyCode::Q))
        {
            pos += 20.0f * -tr->Up() * Time::DeltaTime();
        }

        tr->SetPosition(pos);
    }
    void CameraScript::LateUpdate()
    {
    }
    void CameraScript::Render()
    {
    }
}