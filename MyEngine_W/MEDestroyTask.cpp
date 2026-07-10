#include "MEDestroyTask.h"

#include "../MyEngine_Source/IFSMContext.h"

namespace ME
{
    void DestroyTask::OnEnter(
        FSMBrainCore* brain,
        IFSMContext& context)
    {
        context.DestroyOwner();
    }

    void DestroyTask::OnExecute(
        FSMBrainCore* brain,
        IFSMContext& context)
    {
    }
}