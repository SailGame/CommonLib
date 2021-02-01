#include <sailgame/common/provider_msg_builder.h>

namespace SailGame { namespace Common {

ProviderMsg ProviderMsgBuilder::CreateRegisterArgs(int seqId, 
    const std::string &id, const std::string &gameName, 
    int maxUsers, int minUsers)
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

ProviderMsg ProviderMsgBuilder::CreateRegisterRet(
    int seqId, ErrorNumber err)
{
    ProviderMsg msg;
    msg.set_sequenceid(seqId);

    auto ret = msg.mutable_registerret();
    ret->set_err(err);

    return msg;
}

ProviderMsg ProviderMsgBuilder::CreateCloseGameArgs(int seqId, int roomId)
{
    ProviderMsg msg;
    msg.set_sequenceid(seqId);
    
    auto args = msg.mutable_closegameargs();
    args->set_roomid(roomId);

    return msg;
}
}}
