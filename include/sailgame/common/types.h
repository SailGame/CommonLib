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
// using ProviderMsgPtr = std::shared_ptr<Core::ProviderMsg>;
// using ProviderMsgPtrs = std::vector<ProviderMsgPtr>;
// using UserOperationPtr = std::shared_ptr<Core::UserOperation>;
// using UserOperationPtrs = std::vector<UserOperationPtr>;
using EventPtr = std::shared_ptr<Event>;
// using OperationInRoomArgsPtr = std::shared_ptr<Core::OperationInRoomArgs>;
}}