#include "MECameraScript.h"
#include "METransform.h"
#include "MEInput.h"
#include "MEGameObject.h"
#include "METime.h"


namespace ME
{
    CameraScript::CameraScript()
        :mPrevMousePos(Vector2(0, 0))
        ,mMouseSpeed(0.005f)
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

        mCurMousePos = Input::GetMousePos();
        float curMP = mCurMousePos.Length();
        float prevMP = mPrevMousePos.Length();

        if (abs(curMP - prevMP) > 1.0f)
        {

            Vector2 delta = mCurMousePos - mPrevMousePos;
            mPrevMousePos = mCurMousePos;

            float angle = delta.x * mMouseSpeed;
  
            Vector4 curPos = Vector4(pos.x, pos.y,pos.z, 1.0f);
            Matrix rotation = Matrix::CreateRotationY(angle);
            Vector4 newPos = Vector4::Transform(curPos, rotation);
            pos = Vector3(newPos.x, newPos.y, newPos.z);

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