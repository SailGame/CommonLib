#pragma once

#include <variant>
#include <memory>

namespace Core {
struct ProviderMsg;
struct UserOperation;
struct OperationInRoomArgs;
}

namespace SailGame { namespace Common {

struct Event;
using ProviderMsgs = std::vector<Core::ProviderMsg>;
using EventPtr = std::shared_ptr<Event>;
}}