#pragma once

#include <sailgame_pb/core/provider.pb.h>
#include <sailgame_pb/core/error.pb.h>
#include <sailgame_pb/codenames/codenames.pb.h>

#include "sailgame/codenames/word.h"
#include "sailgame/common/types.h"

namespace SailGame { namespace Game {
using ::Codenames::NotifyMsg;
using ::Codenames::CardInfo;
using ::Codenames::State;
using ::Codenames::Turn;
using ::Codenames::StartGameSettings;
using Core::ErrorNumber;
using Core::NotifyMsgArgs;
using Core::ProviderMsg;
using Common::ProviderMsgPtr;
using Game::Card;

class MsgBuilder
{
public:
    static ProviderMsgPtr CreateStartGameArgs(int seqId, int roomId, 
        const std::vector<unsigned int> userIds, const StartGameSettings &custom);

    static StartGameSettings CreateStartGameSettings(int hourglass);
    
    static ProviderMsgPtr CreateRegisterArgs(int seqId, const std::string &id,
        const std::string &gameName, int maxUsers, int minUsers);
    
    static ProviderMsgPtr CreateNotifyMsgArgs(int seqId, ErrorNumber err,
        int roomId, int userId, const NotifyMsg &custom);
    
    static NotifyMsg CreateGameStart(const std::vector<Card> &cards, const Turn &role, 
        const Turn &turn);
    
    static NotifyMsg CreateProcessState(const ::Codenames::Word &word, const State &state, 
        const Turn &turn);
};
}}