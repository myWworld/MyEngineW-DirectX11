#include "MECameraScript.h"
#include "METransform.h"
#include "MEInput.h"
#include "MEGameObject.h"
#include "METime.h"
#include "MECamera.h"
#include "MEPlayerScript.h"


namespace ME
{
    CameraScript::CameraScript()
        :mPrevMousePos(Vector2(0, 0))
        ,mMouseSpeed(0.01f)
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
        if (mtarget == nullptr)
        {
            if (GetOwner()->GetComponent<Camera>()->GetTarget() != nullptr)
            {
                mtarget = GetOwner()->GetComponent<Camera>()->GetTarget();
            }
        }

        Transform* tr = GetOwner()->GetComponent<Transform>();
        Vector3 pos = tr->GetPosition();
        Vector3 rotation = tr->GetRotation();

        mTargetPos = mtarget->GetComponent<Transform>()->GetPosition();
       
        mCurMousePos = Input::GetMousePos();
        Vector2 delta = mCurMousePos - mPrevMousePos;
        
        cameraYaw += delta.x * mMouseSpeed;
        cameraPitch += delta.y * mMouseSpeed;
       
    
        Camera* camera = GetOwner()->GetComponent<Camera>();
        camera->SetYaw(cameraYaw);
        camera->SetPitch(cameraPitch);
        mPrevMousePos = mCurMousePos;

 
    }
    void CameraScript::LateUpdate()
    {
    }
    void CameraScript::Render()
    {
    }
}