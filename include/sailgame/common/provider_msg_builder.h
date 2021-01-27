#pragma once

#include <sailgame_pb/core/provider.pb.h>
#include <sailgame_pb/core/error.pb.h>
#include <sailgame_pb/core/types.pb.h>

#include "types.h"
#include "util.h"

namespace SailGame { namespace Common {

using Core::ErrorNumber;
using Core::ProviderMsg;
using Common::Util;

class ProviderMsgBuilder {
public:
    static ProviderMsg CreateRegisterArgs(int seqId, const std::string &id,
        const std::string &gameName, int maxUsers, int minUsers);

    template<typename NotifyMsgT>
    static ProviderMsg CreateNotifyMsgArgs(int seqId, ErrorNumber err, 
        int roomId, int userId, const NotifyMsgT &custom)
    {
        ProviderMsg msg;
        msg.set_sequenceid(seqId);

        auto args = msg.mutable_notifymsgargs();
        args->set_err(err);
        args->set_roomid(roomId);
        args->set_userid(userId);
        args->mutable_custom()->PackFrom(custom);

        return msg;
    }

    static ProviderMsg CreateRegisterRet(int seqId, ErrorNumber err);

    template<typename StartGameSettingsT>
    static ProviderMsg CreateStartGameArgs(int seqId, int roomId, 
        const std::vector<unsigned int> userIds, const StartGameSettingsT &custom)
    {
        ProviderMsg msg;
        msg.set_sequenceid(seqId);

        auto args = msg.mutable_startgameargs();
        args->set_roomid(roomId);
        args->mutable_userid()->CopyFrom(
            Util::ConvertVectorToGrpcRepeatedField<unsigned int>(userIds));
        args->mutable_custom()->PackFrom(custom);

        return msg;
    }
    
    template<typename UserOperationT>
    static ProviderMsg CreateUserOperationArgs(int seqId, int roomId,
        int userId, const UserOperationT &custom)
    {
        ProviderMsg msg;
        msg.set_sequenceid(seqId);

        auto args = msg.mutable_useroperationargs();
        args->set_roomid(roomId);
        args->set_userid(userId);
        args->mutable_custom()->PackFrom(custom);

        return msg;
    }

};
}}