#include "FSMBrainCore.h"
#include "MEFSMTask.h"
#include "MEFSMState.h"

using namespace ME;

void FSMBrainCore::Update(IFSMContext& context)
{
    if (mActiveState == nullptr)
        return;

    mCurrentContext = &context;

    mActiveState->UpdateTask(
        this,
        context
    );

    mActiveState->CheckDecision(
        this,
        context
    );
}

void FSMBrainCore::ChangeState(
    FSMState* nextState)
{
    if (nextState == nullptr)
        return;

    if (mActiveState == nextState)
        return;

    if (mCurrentContext == nullptr)
        return;

    if (mActiveState != nullptr)
    {
        mActiveState->ExitState(
            this,
            *mCurrentContext
        );
    }

    mActiveState = nextState;

    mActiveState->EnterState(
        this,
        *mCurrentContext
    );
}