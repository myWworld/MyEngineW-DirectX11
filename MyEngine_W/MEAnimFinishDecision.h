#pragma once

#include "../MyEngine_Source/MEFSMDecision.h"

namespace ME
{
    class AnimFinishDecision final
        : public FSMDecision
    {
    protected:
        bool CheckDecision(
            FSMBrainCore* brain,
            IFSMContext& context) override;
    };
}