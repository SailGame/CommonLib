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

    template <typename Ret = NotifyMsg>
    static Ret CreateDraw(int number) {
        Ret msg;
        auto draw = msg.mutable_draw();
        draw->set_number(number);
        return msg;
    }

    static NotifyMsg CreateSkip(const Skip &skip);

    template <typename Ret = NotifyMsg>
    static Ret CreateSkip() {
        Ret msg;
        msg.mutable_skip();
        return msg;
    }

    static NotifyMsg CreatePlay(const Play &play);

    template <typename Ret = NotifyMsg>
    static Ret CreatePlay(Card card, CardColor color) {
        Ret msg;
        auto play = msg.mutable_play();
        play->mutable_card()->CopyFrom(card.ConvertToGrpcCard());
        play->set_nextcolor(color);
        return msg;
    }

    static NotifyMsg CreateDrawRsp(const std::vector<Card> &cards);
};
}}