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
        const std::string &gameName, int maxUsers, int minUsers)
    {
        ProviderMsg msg;
        msg.set_sequenceid(seqId);

        auto registerArgs = msg.mutable_registerargs();
        registerArgs->set_id(id);
        registerArgs->set_gamename(gameName);

        auto gameSettings = registerArgs->mutable_gamesetting();
        gameSettings->set_maxusers(maxUsers);
        gameSettings->set_minusers(minUsers);

        return msg;
    }

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
        
    static ProviderMsg CreateRegisterRet(int seqId, ErrorNumber err)
    {
        ProviderMsg msg;
        msg.set_sequenceid(seqId);

        auto ret = msg.mutable_registerret();
        ret->set_err(err);

        return msg;
    }

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