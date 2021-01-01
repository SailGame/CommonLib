#pragma once

#include <functional>
#include <thread>

#include <sailgame_pb/core/provider.pb.h>

#include "event_loop.h"
#include "network_interface.h"
#include "state_machine.h"

namespace SailGame { namespace Common {

using Core::ProviderMsg;

template<typename StateT, bool IsProvider>
class GameManager : 
    public EventLoopSubscriber, 
    public NetworkInterfaceSubscriber {
public:
    GameManager(const std::shared_ptr<EventLoop> &eventLoop, 
        const std::shared_ptr<StateMachine<StateT>> &stateMachine,
        const std::shared_ptr<NetworkInterface<IsProvider>> &networkInterface)
        : mEventLoop(eventLoop), mStateMachine(stateMachine),
        mNetworkInterface(networkInterface)
    {
        mEventLoop->SetSubscriber(this);
        mNetworkInterface->SetSubscriber(this);
    }

    void Start() {
        mNetworkInterface->Connect();
        mEventLoop->StartLoop();
    }

    void Stop() {
        mEventLoop->StopLoop();
        // mNetworkInterface->Stop();
    }

    bool HasEventToProcess() const { return !mEventLoop->Empty(); }

    void StartWithRegisterArgs(const ProviderMsgPtr &msg) {
        mNetworkInterface->AsyncListen();
        mNetworkInterface->AsyncSendMsg(*msg);
        mEventLoop->StartLoop();
    }

    void OnEventHappens(const EventPtr &event) override {
        mEventLoop->AppendEvent(event);
    }

    void OnEventProcessed(const EventPtr &event) override {
        switch (event->mType) {
            case EventType::PROVIDER_MSG: {
                auto msg = std::dynamic_pointer_cast<ProviderMsgEvent>(event)->mMsg;
                auto notifyMsgs = mStateMachine->TransitionForProviderMsg(msg);
                for (const auto &msg : notifyMsgs) {
                    mNetworkInterface->AsyncSendMsg(*msg);
                }
                break;
            }
            case EventType::BROADCAST_MSG: {
                break;
            }
            case EventType::USER_INPUT: {
                break;
            }
            default:
                throw std::runtime_error("Unsupported event type.");
        }
    }


private:
    std::shared_ptr<EventLoop> mEventLoop;
    std::shared_ptr<StateMachine<StateT>> mStateMachine;
    std::shared_ptr<NetworkInterface<IsProvider>> mNetworkInterface;
};
}}