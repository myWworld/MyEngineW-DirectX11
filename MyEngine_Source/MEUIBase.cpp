#include "MEUIBase.h"
#include "METransform.h"
#include "MEGameObject.h"

namespace ME
{
    UIBase::UIBase(enums::eUIType type)
        :mType(type)
        ,mbMouseOn(false)
        ,mbFollowingOwner(false)
    {
    }
    UIBase::~UIBase()
    {
    }
    void UIBase::Initialize()
    {
        OnInit();
    }
    void UIBase::Render( )
    {
        if (mbEnabled)
            OnRender();
    }
    void UIBase::Active()
    {
        mbEnabled = true;
        OnActive();
    }
    void UIBase::InActive()
    {
        mbEnabled = false;
        OnInActive();
    }
    void UIBase::Update()
    {
        if (mbEnabled)
            OnUpdate();
    }
    void UIBase::LateUpdate()
    {

        if (mbEnabled)
            OnLateUpdate();
    }
    void UIBase::UIClear()
    {
        OnClear();
    }
    void UIBase::OnInit()
    {
    }
    void UIBase::OnActive()
    {
    }
    void UIBase::OnInActive()
    {
    }
    void UIBase::OnUpdate()
    {
        if (mbFollowingOwner == true)
        {
            if (mOwner != nullptr)
            {
                Transform* tr = mOwner->GetComponent<Transform >();
                Vector3 pos = tr->GetPosition();
                SetPos(Vector2(pos.x, pos.y));


            }
        }
    }
    void UIBase::OnLateUpdate()
    {
    }
    void UIBase::OnRender( )
    {
    }
    void UIBase::OnClear()
    {
    }
}