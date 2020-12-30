#pragma once

#include <core/provider.pb.h>
#include <core/error.pb.h>
#include <uno/uno.pb.h>

#include "card.h"
#include "../common/types.h"
#include "../common/util.h"

namespace SailGame { namespace Game {

using ::Uno::GameStart;
using ::Uno::NotifyMsg;
using ::Uno::UserOperation;
using ::Uno::StartGameSettings;
using ::Uno::Draw;
using ::Uno::Skip;
using ::Uno::Play;
using ::Uno::CardColor;
using Core::ErrorNumber;
using Core::NotifyMsgArgs;
using Core::ProviderMsg;
using Common::ProviderMsgPtr;
using Common::Util;

class MsgBuilder {
public:
    static ProviderMsgPtr CreateRegisterArgs(int seqId, const std::string &id,
        const std::string &gameName, int maxUsers, int minUsers);

    static ProviderMsgPtr CreateRegisterRet(int seqId, ErrorNumber err);

    static ProviderMsgPtr CreateStartGameArgs(int seqId, int roomId, 
        const std::vector<unsigned int> userIds, const StartGameSettings &custom);
    
    static StartGameSettings CreateStartGameSettings(bool isDraw2Consumed,
        bool canSkipRespond, bool hasWildSwapHandsCard, bool canDoubtDraw4, int roundTime);

    static ProviderMsgPtr CreateNotifyMsgArgs(int seqId, ErrorNumber err, 
        int roomId, int userId, const NotifyMsg &custom);

    static ProviderMsgPtr CreateUserOperationArgs(int seqId, int roomId,
        int userId, const UserOperation &custom);

    static NotifyMsg CreateGameStart(const InitHandcardsT &initHandcards, 
        Card flippedCard, int firstPlayer);

    static NotifyMsg CreateDraw(const Draw &draw);

    static UserOperation CreateDraw(int number);

    static NotifyMsg CreateSkip(const Skip &skip);

    static UserOperation CreateSkip();

    static NotifyMsg CreatePlay(const Play &play);

    static UserOperation CreatePlay(Card card, CardColor color);

    static NotifyMsg CreateDrawRsp(const std::vector<Card> &cards);
};
}}