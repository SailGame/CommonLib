#include "msg_builder.h"

namespace SailGame { namespace Game {

ProviderMsgPtr MsgBuilder::CreateRegisterArgs(int seqId, const std::string &id,
    const std::string &gameName, int maxUsers, int minUsers)
{
    auto msg = std::make_shared<ProviderMsg>();
    msg->set_sequenceid(seqId);

    auto registerArgs = msg->mutable_registerargs();
    registerArgs->set_id(id);
    registerArgs->set_gamename(gameName);

    auto gameSettings = registerArgs->mutable_gamesetting();
    gameSettings->set_maxusers(maxUsers);
    gameSettings->set_minusers(minUsers);

    return msg;
}

ProviderMsgPtr MsgBuilder::CreateRegisterRet(int seqId, ErrorNumber err)
{
    auto msg = std::make_shared<ProviderMsg>();
    msg->set_sequenceid(seqId);

    auto ret = msg->mutable_registerret();
    ret->set_err(err);

    return msg;
}

ProviderMsgPtr MsgBuilder::CreateStartGameArgs(int seqId, int roomId, 
    const std::vector<unsigned int> userIds, const StartGameSettings &custom)
{
    auto msg = std::make_shared<ProviderMsg>();
    msg->set_sequenceid(seqId);

    auto args = msg->mutable_startgameargs();
    args->set_roomid(roomId);
    args->mutable_userid()->CopyFrom(Util::ConvertVectorToGrpcRepeatedField<unsigned int>(userIds));
    args->mutable_custom()->PackFrom(custom);

    return msg;
}

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

ProviderMsgPtr MsgBuilder::CreateNotifyMsgArgs(int seqId, ErrorNumber err, 
    int roomId, int userId, const NotifyMsg &custom) 
{
    auto msg = std::make_shared<ProviderMsg>();
    msg->set_sequenceid(seqId);

    auto args = msg->mutable_notifymsgargs();
    args->set_err(err);
    args->set_roomid(roomId);
    args->set_userid(userId);
    args->mutable_custom()->PackFrom(custom);

    return msg;
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
    gameStart->set_firstplayerid(firstPlayer);
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