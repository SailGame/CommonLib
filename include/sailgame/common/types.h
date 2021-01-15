#pragma once

#include <memory>

namespace Core {
struct ProviderMsg;
}

namespace SailGame { namespace Common {

struct Event;
using ProviderMsgs = std::vector<Core::ProviderMsg>;
using EventPtr = std::shared_ptr<Event>;

enum class GameType {
    NoGame,  // which means in the room
    Uno
};

}}