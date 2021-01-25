#pragma once

#include <functional>
#include <thread>

#include <sailgame_pb/core/provider.pb.h>

#include "event_loop.h"
#include "network_interface.h"
#include "state_machine.h"

namespace SailGame { namespace Common {

using Core::ProviderMsg;

class IGameManager : 
    public EventLoopSubscriber, 
    public NetworkInterfaceSubscriber {
public:
    IGameManager(const std::shared_ptr<EventLoop> &eventLoop)
        : mEventLoop(eventLoop)
    {
        mEventLoop->SetSubscriber(this);
    }

    virtual void Start() {
        mEventLoop->StartLoop();
    }

    virtual void Stop() {
        mEventLoop->StopLoop();
    }

    virtual void OnEventHappens(const EventPtr &event) override {
        mEventLoop->AppendEvent(event);
    }

    bool HasEventToProcess() const { return !mEventLoop->Empty(); }

private:
    std::shared_ptr<EventLoop> mEventLoop;
};

class ProviderGameManager : public IGameManager {
public:
    ProviderGameManager(const std::shared_ptr<EventLoop> &eventLoop, 
        const std::shared_ptr<ProviderStateMachine> &stateMachine,
        const std::shared_ptr<ProviderNetworkInterface> &networkInterface)
        : IGameManager(eventLoop), mStateMachine(stateMachine),
        mNetworkInterface(networkInterface)
    {
        mNetworkInterface->SetSubscriber(this);
    }

    virtual void Start() override {
        mNetworkInterface->Connect();
        IGameManager::Start();
    }

    virtual void Stop() override { 
        mNetworkInterface->Stop();
        IGameManager::Stop();
    }

    virtual void OnEventProcessed(const EventPtr &event) override {
        assert(event->mType == EventType::PROVIDER_MSG);
        auto msg = std::dynamic_pointer_cast<ProviderMsgEvent>(event)->mMsg;
        auto notifyMsgs = mStateMachine->TransitionForProviderMsg(msg);
        for (const auto &msgToSend : notifyMsgs) {
            mNetworkInterface->AsyncSendMsg(msgToSend);
        }
    }

    void StartWithRegisterArgs(const ProviderMsg &msg) {
        mNetworkInterface->AsyncListen();
        mNetworkInterface->AsyncSendMsg(msg);
        IGameManager::Start();
    }

private:
    std::shared_ptr<ProviderStateMachine> mStateMachine;
    std::shared_ptr<ProviderNetworkInterface> mNetworkInterface;
};

}}