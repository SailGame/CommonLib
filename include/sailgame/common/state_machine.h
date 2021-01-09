#pragma once

#include <memory>
#include <sailgame_pb/core/provider.pb.h>
#include <sailgame_pb/core/types.pb.h>

#include "types.h"
#include "event.h"

namespace SailGame { namespace Common {

using ::Core::BroadcastMsg;
using ::Core::CloseGameArgs;
using ::Core::OperationInRoomArgs;
using ::Core::ProviderMsg;
using ::Core::QueryStateArgs;
using ::Core::RegisterRet;
using ::Core::StartGameArgs;
using ::Core::UserOperationArgs;

class IState {
public:
    virtual ~IState() {}
};

class IStateMachine {
public:
    ProviderMsgs TransitionForProviderMsg(const ProviderMsg &msg) {
        // seqId is not used now
        switch (msg.Msg_case()) {
            case ProviderMsg::MsgCase::kRegisterRet:
                return Transition(msg.registerret());
            case ProviderMsg::MsgCase::kStartGameArgs:
                return Transition(msg.startgameargs());
            case ProviderMsg::MsgCase::kCloseGameArgs:
                return Transition(msg.closegameargs());
            case ProviderMsg::MsgCase::kQueryStateArgs:
                return Transition(msg.querystateargs());
            case ProviderMsg::MsgCase::kUserOperationArgs:
                return Transition(msg.useroperationargs());
        }
        throw std::runtime_error("Unsupported msg type");
        return {};
    }

    void TransitionForBroadcastMsg(const BroadcastMsg &msg) {
        Transition(msg);
    }

    virtual const IState &GetState() const = 0;

protected:
    virtual ProviderMsgs Transition(const RegisterRet &) { return {}; }

    virtual ProviderMsgs Transition(const StartGameArgs &) { return {}; }

    virtual ProviderMsgs Transition(const CloseGameArgs &) { return {}; }

    virtual ProviderMsgs Transition(const QueryStateArgs &) { return {}; }

    virtual ProviderMsgs Transition(const UserOperationArgs &) { return {}; }

    virtual void Transition(const BroadcastMsg &) {}
};

}}