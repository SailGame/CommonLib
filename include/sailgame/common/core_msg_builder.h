#pragma once

#include <sailgame_pb/core/types.pb.h>
#include <sailgame_pb/core/error.pb.h>

namespace SailGame { namespace Common {

using namespace ::Core;

class CoreMsgBuilder {
public:
    static void SetToken(const std::string &token);

    static LoginArgs CreateLoginArgs(const std::string &username, 
        const std::string &password);

    static QueryAccountArgs CreateQueryAccountArgs(const std::string &username);

    static QueryAccountArgs CreateQueryAccountArgs();

    static CreateRoomArgs CreateCreateRoomArgs();

    template<typename GameSettingsT>
    static ControlRoomArgs CreateControlRoomArgs(int roomId, 
        const std::string &gameName, const std::string &roomPassword,
        const GameSettingsT &custom)
    {
        ControlRoomArgs args;
        args.set_token(mToken);
        args.set_roomid(roomId);
        args.set_gamename(gameName);
        args.set_roompassword(roomPassword);
        args.mutable_custom()->PackFrom(custom);
        return args;
    }

    static ListRoomArgs CreateListRoomArgs(const std::string &gameName);

    static JoinRoomArgs CreateJoinRoomArgs(int roomId);

    static ExitRoomArgs CreateExitRoomArgs();

    static QueryRoomArgs CreateQueryRoomArgs(int roomId);

    static OperationInRoomArgs CreateOperationInRoomArgs(Ready ready);

    /// XXX: handle the other case
    template<typename UserOperationT>
    static OperationInRoomArgs CreateOperationInRoomArgs(const UserOperationT &custom)
    {
        OperationInRoomArgs args;
        args.set_token(mToken);
        args.mutable_custom()->PackFrom(custom);
        return args;
    }

    static MessageArgs CreateMessageArgs(const std::string &message,
        const std::string &dstUser, int dstRoom);

    static ListenArgs CreateListenArgs();

    // msg builder of ret, for tests
    static LoginRet CreateLoginRet(ErrorNumber err, const std::string &token, 
        const Account &account);

    static QueryAccountRet CreateQueryAccountRet(ErrorNumber err,
        const Account &account);

    static CreateRoomRet CreateCreateRoomRet(ErrorNumber err, int roomId);

    template<typename GameSettingsT>
    static ControlRoomRet CreateControlRoomRet(ErrorNumber err, 
        const GameSettingsT &custom)
    {
        ControlRoomRet ret;
        ret.set_err(err);
        ret.mutable_custom()->PackFrom(custom);
        return ret;
    }

    static ListRoomRet CreateListRoomRet(ErrorNumber err, 
        const std::vector<Room> &rooms);

    static JoinRoomRet CreateJoinRoomRet(ErrorNumber err);

    static ExitRoomRet CreateExitRoomRet(ErrorNumber err);

    static QueryRoomRet CreateQueryRoomRet(ErrorNumber err, 
        const RoomDetails &roomDetails);

    static OperationInRoomRet CreateOperationInRoomRet(ErrorNumber err);

    static MessageRet CreateMessageRet(ErrorNumber err);

    static ListenRet CreateListenRet(ErrorNumber err);

    static Account CreateAccount(const std::string &username, int points);

    static Room CreateRoom(const std::string &gameName, int roomId,
        const std::vector<std::string> &userNames);

    static RoomUser CreateRoomUser(const std::string &userName, 
        RoomUser::RoomUserState state);

    template<typename GameSettingsT>
    static RoomDetails CreateRoomDetails(const std::string &gameName, int roomId,
        const std::vector<RoomUser> &users, const GameSettingsT &gameSetting)
    {
        RoomDetails details;
        details.set_gamename(gameName);
        details.set_roomid(roomId);
        for (const auto &user : users) {
            details.add_user()->CopyFrom(user);
        }
        details.mutable_gamesetting()->PackFrom(gameSetting);
        return details;
    }

    template<typename NotifyMsgT>
    static BroadcastMsg CreateBroadcastMsgByNotifyMsg(int seqId, 
        unsigned int fromUser, unsigned int toUser, const NotifyMsgT &custom)
    {
        BroadcastMsg msg;
        msg.set_sequenceid(seqId);
        msg.set_fromuser(fromUser);
        msg.set_touser(toUser);
        msg.mutable_custom()->PackFrom(custom);
        return msg;
    }

    static BroadcastMsg CreateBroadcastMsgByRoomDetails(
        int seqId, unsigned int fromUser, unsigned int toUser,
        const RoomDetails &roomDetails);

private:
    static std::string mToken;
};

}}