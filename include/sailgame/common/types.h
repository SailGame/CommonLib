#pragma once

#include <variant>
#include <memory>

namespace Core {
struct ProviderMsg;
}

namespace SailGame { namespace Common {

using ProviderMsgPtr = std::shared_ptr<Core::ProviderMsg>;
using ProviderMsgPtrs = std::vector<ProviderMsgPtr>;
}}