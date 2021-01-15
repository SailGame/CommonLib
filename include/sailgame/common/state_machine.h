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

    /**
     * Implement below methods as throwing runtime error instead of pure virtual
     * because IStateMachine could be inherited by both Provider and Client,
     * derived class in Provider side needn't implement methods used by Client
     * and for Client side the condition is the same.
     * If we declare them as pure virtual, derived class will be abstract class.
     * Of course a better way is to derive ProviderStateMachine and 
     * ClientStateMachine from IStateMachine and then more derived ones.
     * Anyway, that can be done in the future.
     */
    virtual const IState &GetState() const {
        throw std::runtime_error("IStateMachine method invoked.");
    }

    virtual GameType GetType() const {
        throw std::runtime_error("IStateMachine method invoked.");
    }

    virtual void SwitchFrom(const IStateMachine &) {
        throw std::runtime_error("IStateMachine method invoked.");
    }

    virtual ~IStateMachine() = default;

protected:
    // state machine of provider side needs to implement
    virtual ProviderMsgs Transition(const RegisterRet &) {
        throw std::runtime_error("IStateMachine method invoked.");
    }

    virtual ProviderMsgs Transition(const StartGameArgs &) {
        throw std::runtime_error("IStateMachine method invoked.");
    }

    virtual ProviderMsgs Transition(const CloseGameArgs &) {
        throw std::runtime_error("IStateMachine method invoked.");
    }

    virtual ProviderMsgs Transition(const QueryStateArgs &) {
        throw std::runtime_error("IStateMachine method invoked.");
    }

    virtual ProviderMsgs Transition(const UserOperationArgs &) {
        throw std::runtime_error("IStateMachine method invoked.");
    }

    // state machine of client side needs to implement
    virtual void Transition(const BroadcastMsg &) {
        throw std::runtime_error("IStateMachine method invoked.");
    }
};
}}