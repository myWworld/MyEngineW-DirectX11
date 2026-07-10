#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class FSMState;
class IFSMContext;

class FSMBrainCore
{
public:
    void Update(IFSMContext& context);

    void ChangeState(FSMState* nextState);
    void SendFSMEvent(const std::string& eventName);

    void AddState(
        const std::string& name,
        std::unique_ptr<FSMState> state);

    void SetInitialState(
        const std::string& name,
        IFSMContext& context);

public:
    std::unordered_map<
        std::string,
        std::unique_ptr<FSMState>> mStates;

    FSMState* mActiveState = nullptr;

private:
    IFSMContext* mCurrentContext = nullptr;

};
