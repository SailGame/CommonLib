#pragma once

#include <string>
#include <thread>
#include <memory>
#include <functional>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <sailgame_pb/core/provider.pb.h>
#include <sailgame_pb/core/types.pb.h>
#include <sailgame_pb/core/core.grpc.pb.h>

#include "types.h"
#include "event.h"
#include "util.h"
#include "core_msg_builder.h"
#include "provider_msg_builder.h"

namespace SailGame { namespace Common {

using Core::GameCore;
using Core::OperationInRoomArgs;
using Core::OperationInRoomRet;
using Core::ProviderMsg;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientReaderWriterInterface;
using grpc::ClientWriter;
using grpc::Status;

class NetworkInterfaceSubscriber {
public:
    virtual void OnEventHappens(const EventPtr &) = 0;
};

class INetworkInterface {
public:
    INetworkInterface(const std::shared_ptr<GameCore::StubInterface> &stub)
        : mStub(stub) {}

    static std::shared_ptr<GameCore::StubInterface> CreateStub(const std::string &endpoint) {
        auto channel = grpc::CreateChannel(endpoint, grpc::InsecureChannelCredentials());
        return GameCore::NewStub(channel);
    }

    virtual void Connect() = 0;

    virtual bool IsConnected() const = 0;

    virtual void AsyncSendMsg(const ProviderMsg &msg) {
        throw std::runtime_error("INetworkInterface's AsyncSendMsg invoked.");
    }

    void SetSubscriber(NetworkInterfaceSubscriber *subscriber) {
        mSubscriber = subscriber;
    }

    void AsyncListen() {
        // spdlog::info("[INetworkInterface] AsyncListen invoked.");
        Connect();
        mListenThread = std::make_unique<std::thread>(mListenFunc);
        // spdlog::info("[INetworkInterface] Listen Thread started.");
    }

    void Stop() { 
        if (mListenThread) {
            mListenThread->join(); 
        }
    }

    std::function<void()> mListenFunc;

protected:
    // this context is for Listen streaming
    // NOTE that in grpc, context cannot be reused.
    ClientContext mContext;
    std::shared_ptr<GameCore::StubInterface> mStub;
    NetworkInterfaceSubscriber *mSubscriber{nullptr};

private:
    std::unique_ptr<std::thread> mListenThread;
};

class ProviderNetworkInterface : public INetworkInterface {
public:
    ProviderNetworkInterface(const std::shared_ptr<GameCore::StubInterface> &stub)
        : INetworkInterface(stub)
    {
        mListenFunc = [this] {
            while (true) {
                OnEventHappens(ReceiveMsg());
            }
        };
    }

    ~ProviderNetworkInterface() {
        mStream->WritesDone();
    }

    static std::shared_ptr<ProviderNetworkInterface> Create(
        const std::shared_ptr<GameCore::StubInterface> &stub) 
    {
        return std::make_shared<ProviderNetworkInterface>(stub);
    }

    virtual void Connect() override {
        // spdlog::info("[INetworkInterface] Connect invoked.");
        mStream = mStub->Provider(&mContext);
    }

    virtual bool IsConnected() const override {
        return mStream != nullptr;
    }

    virtual void AsyncSendMsg(const ProviderMsg &msg) override {
        mStream->Write(msg);
    }

    void OnEventHappens(const ProviderMsg &msg) {
        mSubscriber->OnEventHappens(std::make_shared<ProviderMsgEvent>(msg));
    }

    ProviderMsg ReceiveMsg()
    {
        ProviderMsg msg;
        if (mStream->Read(&msg)) {
            return msg;
        }
        if (!mStream->Finish().ok()) {
            std::cout << "rpc failed." << std::endl;
            std::exit(-1);
        }
        auto error_msg = "Stream ends normally, which indicates error in core side.";
        throw std::runtime_error(error_msg);
        return msg;
    }

private:
    std::shared_ptr<ClientReaderWriterInterface<ProviderMsg, ProviderMsg>> mStream;
};

}}