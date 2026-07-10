#pragma once

#include "../MyEngine_Source/MEFSMTask.h"

namespace ME
{
    class DestroyTask final : public FSMTask
    {
    protected:
        void OnEnter(
            FSMBrainCore* brain,
            IFSMContext& context) override;

        void OnExecute(
            FSMBrainCore* brain,
            IFSMContext& context) override;
    };
}