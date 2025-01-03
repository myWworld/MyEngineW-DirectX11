#include "MECameraScript.h"
#include "METransform.h"
#include "MEInput.h"
#include "MEGameObject.h"
#include "METime.h"

namespace ME
{
    CameraScript::CameraScript()
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