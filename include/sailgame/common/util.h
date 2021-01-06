#pragma once

#include <memory>
#include <vector>
#include <type_traits>
#include <google/protobuf/any.pb.h>
#include <google/protobuf/repeated_field.h>
#include <grpcpp/impl/codegen/sync_stream.h>

#include <sailgame_pb/core/types.pb.h>
#include <sailgame_pb/core/provider.pb.h>

#include "event.h"

namespace SailGame { namespace Common {

using google::protobuf::Any;
using google::protobuf::RepeatedField;
using google::protobuf::RepeatedPtrField;
using grpc::ClientReaderWriterInterface;
using grpc::ClientReaderInterface;
using Core::ProviderMsg;
using Core::BroadcastMsg;

class Util {
public:
    template<typename T>
    static std::vector<T> ConvertGrpcRepeatedFieldToVector(const RepeatedField<T> &field) {
        /// XXX: it seems also ok to move instead of copy
        static_assert(std::is_copy_constructible_v<T>);
        std::vector<T> v;
        for (const auto &element : field) {
            v.push_back(element);
        }
        return v;
    }

    template<typename T>
    static std::vector<T> ConvertGrpcRepeatedPtrFieldToVector(const RepeatedPtrField<T> &field) {
        static_assert(std::is_copy_constructible_v<T>);
        std::vector<T> v;
        for (const auto &element : field) {
            v.push_back(element);
        }
        return v;
    }

    template<typename T>
    static RepeatedField<T> ConvertVectorToGrpcRepeatedField(const std::vector<T> &v) {
        static_assert(std::is_copy_constructible_v<T>);
        RepeatedField<T> field;
        for (const auto &element : v) {
            field.Add(element);
        }
        return field;
    }

    template<typename T>
    static T UnpackGrpcAnyTo(const Any &any) {
        assert(any.Is<T>());
        T ret;
        any.UnpackTo(&ret);
        return ret;
    }

    static int Wrap(int numToWrap, int range) {
        int ret = numToWrap % range;
        if (ret < 0) {
            ret += range;
        }
        return ret;
    }
};

template<bool IsProvider>
struct get_network_type;

template<>
struct get_network_type<true> {
    using msg_type = ProviderMsg;
    using stream_type = ClientReaderWriterInterface<ProviderMsg, ProviderMsg>;
    using event_type = ProviderMsgEvent;
};

template<>
struct get_network_type<false> {
    using msg_type = BroadcastMsg;
    using stream_type = ClientReaderInterface<BroadcastMsg>;
    using event_type = BroadcastMsgEvent;
};

template<bool IsProvider>
using get_msg_t = typename get_network_type<IsProvider>::msg_type;

template<bool IsProvider>
using get_stream_t = typename get_network_type<IsProvider>::stream_type;

template <bool IsProvider>
using get_event_t = typename get_network_type<IsProvider>::event_type;

}}