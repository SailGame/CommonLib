#include <sailgame/common/core_msg_builder.h>

namespace SailGame { namespace Common {

std::string CoreMsgBuilder::mToken;

void CoreMsgBuilder::SetToken(const std::string &token) { mToken = token; }

LoginArgs CoreMsgBuilder::CreateLoginArgs(
    const std::string &username, const std::string &password)
{
    LoginArgs args;
    args.set_username(username);
    args.set_password(password);
    return args;
}

QueryAccountArgs CoreMsgBuilder::CreateQueryAccountArgs(
    const std::string &username)
{
    QueryAccountArgs args;
    args.set_username(username);
    return args;
}

QueryAccountArgs CoreMsgBuilder::CreateQueryAccountArgs()
{
    QueryAccountArgs args;
    args.set_token(mToken);
    return args;
}

CreateRoomArgs CoreMsgBuilder::CreateCreateRoomArgs()
{
    CreateRoomArgs args;
    args.set_token(mToken);
    return args;
}

ListRoomArgs CoreMsgBuilder::CreateListRoomArgs(const std::string &gameName)
{
    ListRoomArgs args;
    args.set_gamename(gameName);
    return args;
}

JoinRoomArgs CoreMsgBuilder::CreateJoinRoomArgs(int roomId)
{
    JoinRoomArgs args;
    args.set_token(mToken);
    args.set_roomid(roomId);
    return args;
}

ExitRoomArgs CoreMsgBuilder::CreateExitRoomArgs()
{
    ExitRoomArgs args;
    args.set_token(mToken);
    return args;
}

QueryRoomArgs CoreMsgBuilder::CreateQueryRoomArgs(int roomId)
{
    QueryRoomArgs args;
    args.set_token(mToken);
    args.set_roomid(roomId);
    return args;
}

OperationInRoomArgs CoreMsgBuilder::CreateOperationInRoomArgs(Ready ready)
{
    OperationInRoomArgs args;
    args.set_token(mToken);
    args.set_ready(ready);
    return args;
}

MessageArgs CoreMsgBuilder::CreateMessageArgs(const std::string &message,
    const std::string &dstUser, int dstRoom)
{
    MessageArgs args;
    args.set_token(mToken);
    args.set_message(message);
    args.set_dstuser(dstUser);
    args.set_dstroom(dstRoom);
    return args;
}

ListenArgs CoreMsgBuilder::CreateListenArgs() 
{
    ListenArgs args;
    args.set_token(mToken);
    return args;
}

// msg builder of ret, for tests
LoginRet CoreMsgBuilder::CreateLoginRet(
    ErrorNumber err, const std::string &token, const Account &account)
{
    LoginRet ret;
    ret.set_err(err);
    ret.set_token(token);
    ret.mutable_account()->CopyFrom(account);
    return ret;
}

QueryAccountRet CoreMsgBuilder::CreateQueryAccountRet(
    ErrorNumber err, const Account &account)
{
    QueryAccountRet ret;
    ret.set_err(err);
    ret.mutable_account()->CopyFrom(account);
    return ret;
}

CreateRoomRet CoreMsgBuilder::CreateCreateRoomRet(
    ErrorNumber err, int roomId)
{
    CreateRoomRet ret;
    ret.set_err(err);
    ret.set_roomid(roomId);
    return ret;
}

ListRoomRet CoreMsgBuilder::CreateListRoomRet(
    ErrorNumber err, const std::vector<Room> &rooms)
{
    ListRoomRet ret;
    ret.set_err(err);
    for (const auto &room : rooms) {
        ret.add_room()->CopyFrom(room);
    }
    return ret;
}

JoinRoomRet CoreMsgBuilder::CreateJoinRoomRet(ErrorNumber err)
{
    JoinRoomRet ret;
    ret.set_err(err);
    return ret;
}

ExitRoomRet CoreMsgBuilder::CreateExitRoomRet(ErrorNumber err)
{
    ExitRoomRet ret;
    ret.set_err(err);
    return ret;
}

QueryRoomRet CoreMsgBuilder::CreateQueryRoomRet(ErrorNumber err, 
    const RoomDetails &roomDetails)
{
    QueryRoomRet ret;
    ret.set_err(err);
    ret.mutable_room()->CopyFrom(roomDetails);
    return ret;
}

OperationInRoomRet CoreMsgBuilder::CreateOperationInRoomRet(ErrorNumber err)
{
    OperationInRoomRet ret;
    ret.set_err(err);
    return ret;
}

MessageRet CoreMsgBuilder::CreateMessageRet(ErrorNumber err)
{
    MessageRet ret;
    ret.set_err(err);
    return ret;
}

ListenRet CoreMsgBuilder::CreateListenRet(ErrorNumber err)
{
    ListenRet ret;
    ret.set_err(err);
    return ret;
}

Account CoreMsgBuilder::CreateAccount(const std::string &username, int points)
{
    Account account;
    account.set_username(username);
    account.set_points(points);
    return account;
}

Room CoreMsgBuilder::CreateRoom(const std::string &gameName, int roomId,
    const std::vector<std::string> &userNames)
{
    Room room;
    room.set_gamename(gameName);
    room.set_roomid(roomId);
    for (const auto &userName : userNames) {
        room.add_username(userName);
    }
    return room;
}

RoomUser CoreMsgBuilder::CreateRoomUser(const std::string &userName, 
    RoomUser::RoomUserState state)
{
    RoomUser user;
    user.set_username(userName);
    user.set_userstate(state);
    return user;
}

BroadcastMsg CoreMsgBuilder::CreateBroadcastMsgByRoomDetails(int seqId, 
    unsigned int fromUser, unsigned int toUser, 
    const RoomDetails &roomDetails)
{
    BroadcastMsg msg;
    msg.set_sequenceid(seqId);
    msg.set_fromuser(fromUser);
    msg.set_touser(toUser);
    msg.mutable_roomdetails()->CopyFrom(roomDetails);
    return msg;
}

}}