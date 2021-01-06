#pragma once

#include <sailgame_pb/core/types.pb.h>

namespace SailGame { namespace Common {

using ::Core::ListenArgs;
using ::Core::OperationInRoomArgs;

class CoreMsgBuilder {
public:
    static void SetToken(const std::string &token) { mToken = token; }

    static ListenArgs CreateListenArgs() 
    {
        ListenArgs args;
        args.set_token(mToken);
        return args;
    }

    /// XXX: handle the other case
    template<typename UserOperationT>
    static OperationInRoomArgs CreateOperationInRoomArgs(const UserOperationT &custom)
    {
        OperationInRoomArgs args;
        args.set_token(mToken);
        args.mutable_custom()->PackFrom(custom);
        return args;
    }

private:
    static std::string mToken;
};

}}