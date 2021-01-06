#pragma once

#include <sailgame_pb/core/provider.pb.h>
#include <sailgame_pb/core/error.pb.h>
#include <sailgame_pb/uno/uno.pb.h>

#include "card.h"
#include "../common/types.h"
#include "../common/util.h"

namespace SailGame { namespace Uno {

using ::Uno::GameStart;
using ::Uno::NotifyMsg;
using ::Uno::UserOperation;
using ::Uno::StartGameSettings;
using ::Uno::Draw;
using ::Uno::Skip;
using ::Uno::Play;
using ::Uno::CardColor;
using ::Core::ErrorNumber;
using ::Core::NotifyMsgArgs;
using ::Core::ProviderMsg;
using Common::Util;

class MsgBuilder {
public:
    static StartGameSettings CreateStartGameSettings(bool isDraw2Consumed,
        bool canSkipRespond, bool hasWildSwapHandsCard, bool canDoubtDraw4, int roundTime);

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