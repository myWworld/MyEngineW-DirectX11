#pragma once

#include "MEFSMState.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace ME
{
    class BlackBoard;
    class IFSMContext;

    class FSMBrainCore
    {
    public:
        FSMBrainCore();
        ~FSMBrainCore();

        FSMBrainCore(const FSMBrainCore&) = delete;
        FSMBrainCore& operator=(const FSMBrainCore&) = delete;

        void Update(IFSMContext& context);

        void ChangeState(FSMState* nextState);
        void SendFSMEvent(const std::string& eventName);

        void AddState(
            const std::string& name,
            std::unique_ptr<FSMState> state);

        bool SetInitialState(const std::string& name);

        FSMState* FindState(const std::string& name) const;
        FSMState* GetActiveState() const;

        std::string GetActiveStateName() const;
        float GetStateElapsedTime() const;

        BlackBoard* GetBlackboard() const;

        void Clear();

    private:
        std::unordered_map<std::string, std::unique_ptr<FSMState>> mStates;

        FSMState* mActiveState = nullptr;
        IFSMContext* mCurrentContext = nullptr;

        std::unique_ptr<BlackBoard> mBlackboard;

        float mStateElapsedTime = 0.0f;
        bool mbStarted = false;
    };
}