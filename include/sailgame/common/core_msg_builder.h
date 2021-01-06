#pragma once

#include <sailgame_pb/core/types.pb.h>

namespace SailGame { namespace Common {

using ::Core::ListenArgs;

class CoreMsgBuilder {
public:
    static void SetToken(const std::string &token) { mToken = token; }

    static ListenArgs CreateListenArgs() 
    {
        ListenArgs args;
        args.set_token(mToken);
        return args;
    }

private:
    static std::string mToken;
};

}}