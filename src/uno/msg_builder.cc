#include <sailgame/uno/msg_builder.h>

namespace SailGame { namespace Uno {

StartGameSettings MsgBuilder::CreateStartGameSettings(bool isDraw2Consumed,
    bool canSkipRespond, bool hasWildSwapHandsCard, bool canDoubtDraw4, int roundTime)
{
    StartGameSettings settings;
    settings.set_isdraw2consumed(isDraw2Consumed);
    settings.set_canskiprespond(canSkipRespond);
    settings.set_haswildswaphandscard(hasWildSwapHandsCard);
    settings.set_candoubtdraw4(canDoubtDraw4);
    settings.set_roundtime(roundTime);
    return settings;
}

NotifyMsg MsgBuilder::CreateGameStart(const InitHandcardsT &initHandcards, 
    Card flippedCard, int firstPlayer) 
{
    NotifyMsg notifyMsg;
    auto gameStart = notifyMsg.mutable_gamestart();
    for (auto card : initHandcards) {
        gameStart->add_inithandcards()->CopyFrom(card.ConvertToGrpcCard());
    }
    gameStart->mutable_flippedcard()->CopyFrom(
        flippedCard.ConvertToGrpcCard());
    gameStart->set_firstplayer(firstPlayer);
    return notifyMsg;
}

NotifyMsg MsgBuilder::CreateDraw(const Draw &draw)
{
    NotifyMsg msg;
    msg.mutable_draw()->CopyFrom(draw);
    return msg;
}

NotifyMsg MsgBuilder::CreateSkip(const Skip &skip)
{
    NotifyMsg msg;
    msg.mutable_skip()->CopyFrom(skip);
    return msg;
}

NotifyMsg MsgBuilder::CreatePlay(const Play &play)
{
    NotifyMsg msg;
    msg.mutable_play()->CopyFrom(play);
    return msg;
}

NotifyMsg MsgBuilder::CreateDrawRsp(const std::vector<Card> &cards)
{
    NotifyMsg msg;
    auto drawRsp = msg.mutable_drawrsp();
    for (auto card : cards) {
        drawRsp->add_cards()->CopyFrom(card.ConvertToGrpcCard());
    }
    return msg;
}
}}