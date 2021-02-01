#pragma once

#include <memory>

#include <sailgame_pb/core/provider.pb.h>
#include <sailgame_pb/core/types.pb.h>

namespace SailGame { namespace Common {

using Core::BroadcastMsg;
using Core::ProviderMsg;

enum class EventType {
    PROVIDER_MSG,
    BROADCAST_MSG
};

struct Event {
    Event(EventType type) : mType(type) {}

    virtual ~Event() {}

    EventType mType;
};

struct ProviderMsgEvent : public Event {
    ProviderMsgEvent(const ProviderMsg &msg) 
        : Event(EventType::PROVIDER_MSG), mMsg(msg) {}
    
    ProviderMsg mMsg;
};

struct BroadcastMsgEvent : public Event {
    BroadcastMsgEvent(const BroadcastMsg &msg)
        : Event(EventType::BROADCAST_MSG), mMsg(msg) {}

    BroadcastMsg mMsg;
};

}}