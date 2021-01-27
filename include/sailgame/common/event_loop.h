#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <mutex>

#include <sailgame_pb/core/provider.pb.h>

#include "types.h"
#include "event.h"

namespace SailGame { namespace Common {

using Core::ProviderMsg;

class EventLoopSubscriber {
public:
    virtual void OnEventProcessed(const EventPtr &) = 0;
};

class EventLoop {
public:
    EventLoop() = default;

    static std::shared_ptr<EventLoop> Create() {
        return std::make_shared<EventLoop>();
    }

    void StartLoop() {
        while (!mShouldStop) {
            if (!mEventQueue.empty()) {
                mSubscriber->OnEventProcessed(mEventQueue.front());
                mEventQueue.pop();
            }
        }
    }

    void StopLoop() {
        mShouldStop = true;
    }

    void AppendEvent(const EventPtr &event) {
        // multiple threads may invoke this at the same time
        std::lock_guard<std::mutex> lock(mMutex);
        mEventQueue.push(event);
    }

    bool Empty() const { return mEventQueue.empty(); }

    void SetSubscriber(EventLoopSubscriber *subscriber) {
        mSubscriber = subscriber;
    }

private:
    std::queue<EventPtr> mEventQueue;
    std::mutex mMutex;
    EventLoopSubscriber *mSubscriber{nullptr};
    bool mShouldStop{false};
};
}}