//
// Copyright 2017 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <algorithm>
#include <iterator>

#include "mqtt/protocol/common/field.h"

namespace mqtt
{

namespace protocol
{

namespace v5
{

namespace field
{

using FieldBase = mqtt::protocol::common::field::FieldBase;

using ProtocolVersionVal = mqtt::protocol::common::field::ProtocolVersionVal;

using ProtocolVersion =
    mqtt::protocol::common::field::ProtocolVersion<ProtocolVersionVal::v5>;

enum class PropertyIdVal : std::uint8_t
{
    PayloadFormatIndicator = 1,
    PublishExpiryInterval = 2,
    ContentType = 3,
    ResponseTopic = 8,
    CorrelationData = 9,
    SubscriptionIdentifier = 11,
    SessionExpiryInterval = 17,
    AssignedClientIdentifier = 18,
    ServerKeepAlive = 19,
    AuthMethod = 21,
    AuthData = 22,
    RequestProblemInformation = 23,
    WillDelayInterval = 24,
    RequestResponseInformation = 25,
    ResponseInformation = 26,
    ServerReference = 28,
    ReasonString = 31,
    ReceiveMaximum = 33,
    TopicAliasMaximum = 34,
    TopicAlias = 35,
    MaximumQoS = 36,
    RetainAvailable = 37,
    UserProperty = 38,
    MaximumPacketSize = 39,
    WildcardSubscriptionAvailable = 40,
    SubscriptionIdentifierAvailable = 41,
    SharedSubscriptionAvailable = 42
};

template <PropertyIdVal TVal>
using PropertyId =
    comms::field::EnumValue<
        FieldBase,
        PropertyIdVal,
        comms::option::DefaultNumValue<(int)TVal>,
        comms::option::ValidNumValueRange<(int)TVal, (int)TVal>,
        comms::option::FailOnInvalid<>
    >;

template <PropertyIdVal TId, typename TValueField>
struct Property : public
        comms::field::Bundle<
            FieldBase,
            std::tuple<
                PropertyId<TId>,
                TValueField
            >
        >
{
    COMMS_FIELD_MEMBERS_ACCESS(id, value);
};


enum class PayloadFormatIndicatorVal : std::uint8_t
{
    Unspecified,
    Utf8CharData,
    NumOfValues
};

using PayloadFormatIndicator =
    comms::field::EnumValue<
        FieldBase,
        PayloadFormatIndicatorVal,
        comms::option::ValidNumValueRange<0, (int)PayloadFormatIndicatorVal::NumOfValues - 1>
    >;

using PayloadFormatIndicatorProp =
    Property<PropertyIdVal::PayloadFormatIndicator, PayloadFormatIndicator>;

using PublishExpiryInterval =
    comms::field::IntValue<
        FieldBase,
        std::uint32_t,
        comms::option::UnitsSeconds
    >;

using PublishExpiryIntervalProp =
    Property<PropertyIdVal::PublishExpiryInterval, PublishExpiryInterval>;

using ContentType = common::field::String<>;

using ContentTypeProp =
    Property<PropertyIdVal::ContentType, ContentType>;

using ResponseTopic = common::field::String<>;

using ResponseTopicProp =
    Property<PropertyIdVal::ResponseTopic, ResponseTopic>;

using CorrelationData = common::field::BinData<>;

using CorrelationDataProp =
    Property<PropertyIdVal::CorrelationData, CorrelationData>;

using SubscriptionIdentifier = common::field::VarByteInt;

using SubscriptionIdentifierProp =
    Property<PropertyIdVal::SubscriptionIdentifier, SubscriptionIdentifier>;

using SessionExpiryInterval =
    comms::field::IntValue<
        FieldBase,
        std::uint32_t,
        comms::option::UnitsSeconds
    >;

using SessionExpiryIntervalProp =
    Property<PropertyIdVal::SessionExpiryInterval, SessionExpiryInterval>;

using AssignedClientIdentifier = common::field::String<>;

using AssignedClientIdentifierProp =
    Property<PropertyIdVal::AssignedClientIdentifier, AssignedClientIdentifier>;

using ServerKeepAlive =
    comms::field::IntValue<
        FieldBase,
        std::uint16_t,
        comms::option::UnitsSeconds
    >;

using ServerKeepAliveProp =
    Property<PropertyIdVal::ServerKeepAlive, ServerKeepAlive>;

using AuthMethod = common::field::String<>;

using AuthMethodProp =
    Property<PropertyIdVal::AuthMethod, AuthMethod>;

using AuthData = common::field::BinData<>;

using AuthDataProp =
    Property<PropertyIdVal::AuthData, AuthData>;

enum class BoolPropVal : std::uint8_t
{
    False,
    True,
    NumOfValues
};

using BoolPropValue =
    comms::field::EnumValue<
        FieldBase,
        BoolPropVal,
        comms::option::ValidNumValueRange<0, (int)BoolPropVal::NumOfValues - 1>
    >;

using RequestProblemInformation = BoolPropValue;

using RequestProblemInformationProp =
    Property<PropertyIdVal::RequestProblemInformation, RequestProblemInformation>;

using WillDelayInterval =
    comms::field::IntValue<
        FieldBase,
        std::uint32_t,
        comms::option::UnitsSeconds
    >;

using WillDelayIntervalProp =
    Property<PropertyIdVal::WillDelayInterval, WillDelayInterval>;

using RequestResponseInformation = BoolPropValue;

using RequestResponseInformationProp =
    Property<PropertyIdVal::RequestResponseInformation, RequestResponseInformation>;

using ResponseInformation = common::field::String<>;

using ResponseInformationProp =
    Property<PropertyIdVal::ResponseInformation, ResponseInformation>;

using ServerReference = common::field::String<>;

using ServerReferenceProp =
    Property<PropertyIdVal::ServerReference, ServerReference>;

using ReasonString = common::field::String<>;

using ReasonStringProp =
    Property<PropertyIdVal::ReasonString, ReasonString>;

using ReceiveMaximum =
    comms::field::IntValue<
        FieldBase,
        std::uint16_t
    >;

using ReceiveMaximumProp =
    Property<PropertyIdVal::ReceiveMaximum, ReceiveMaximum>;

using TopicAliasMaximum =
    comms::field::IntValue<
        FieldBase,
        std::uint16_t
    >;

using TopicAliasMaximumProp =
    Property<PropertyIdVal::TopicAliasMaximum, TopicAliasMaximum>;

using TopicAlias =
    comms::field::IntValue<
        FieldBase,
        std::uint16_t
    >;

using TopicAliasProp =
    Property<PropertyIdVal::TopicAlias, TopicAlias>;

using MaximumQoS =
    comms::field::EnumValue<
        FieldBase,
        common::field::QosVal,
        comms::option::ValidNumValueRange<0, (int)common::field::QosVal::AtLeastOnceDelivery>,
        comms::option::FailOnInvalid<>
    >;

using MaximumQosProp =
    Property<PropertyIdVal::MaximumQoS, MaximumQoS>;

using RetainAvailable = BoolPropValue;

using RetainAvailableProp =
    Property<PropertyIdVal::RetainAvailable, RetainAvailable>;

struct UserProperty : public
        comms::field::Bundle<
            FieldBase,
            std::tuple<
                common::field::String<>,
                common::field::String<>
            >
        >
{
    COMMS_FIELD_MEMBERS_ACCESS(key, value);
};

using UserPropertyProp =
    Property<PropertyIdVal::UserProperty, UserProperty>;

using MaximumPacketSize =
    comms::field::IntValue<
        FieldBase,
        std::uint32_t
    >;

using MaximumPacketSizeProp =
    Property<PropertyIdVal::MaximumPacketSize, MaximumPacketSize>;

using WildcardSubscriptionAvailable = BoolPropValue;

using WildcardSubscriptionAvailableProp =
    Property<PropertyIdVal::WildcardSubscriptionAvailable, WildcardSubscriptionAvailable>;

using SubscriptionIdentifierAvailable = BoolPropValue;

using SubscriptionIdentifierAvailableProp =
    Property<PropertyIdVal::SubscriptionIdentifierAvailable, SubscriptionIdentifierAvailable>;

using SharedSubscriptionAvailable = BoolPropValue;

using SharedSubscriptionAvailableProp =
    Property<PropertyIdVal::SharedSubscriptionAvailable, SharedSubscriptionAvailable>;

struct PropertyVar : public
    comms::field::Variant<
        FieldBase,
        std::tuple<
            PayloadFormatIndicatorProp,
            PublishExpiryIntervalProp,
            ContentTypeProp,
            ResponseTopicProp,
            CorrelationDataProp,
            SubscriptionIdentifierProp,
            SessionExpiryIntervalProp,
            AssignedClientIdentifierProp,
            ServerKeepAliveProp,
            AuthMethodProp,
            AuthDataProp,
            RequestProblemInformationProp,
            WillDelayIntervalProp,
            RequestResponseInformationProp,
            ResponseInformationProp,
            ServerReferenceProp,
            ReasonStringProp,
            ReceiveMaximumProp,
            TopicAliasMaximumProp,
            TopicAliasProp,
            MaximumQosProp,
            RetainAvailableProp,
            UserPropertyProp,
            MaximumPacketSizeProp,
            WildcardSubscriptionAvailableProp,
            SubscriptionIdentifierAvailableProp,
            SharedSubscriptionAvailableProp
        >
    >
{
    COMMS_VARIANT_MEMBERS_ACCESS(
        payloadFormatIndicator,
        publishExpiryInterval,
        contentType,
        responseTopic,
        correlationData,
        subscriptionIdentifier,
        sessionExpiryInterval,
        assignedClientIdentifier,
        serverKeepAlive,
        authMethod,
        authData,
        requestProblemInformation,
        willDelayInterval,
        requestResponseInformation,
        responseInformation,
        serverReference,
        reasonString,
        receiveMaximum,
        topicAliasMaximum,
        topicAlias,
        maximumQoS,
        retainAvailable,
        userProperty,
        maximumPacketSize,
        wildcardSubscriptionAvailable,
        subscriptionIdentifierAvailable,
        sharedSubscriptionAvailable
    );
};

using Properties =
    comms::field::ArrayList<
        FieldBase,
        PropertyVar,
        comms::option::SequenceSerLengthFieldPrefix<
            mqtt::protocol::common::field::VarByteInt
        >
    >;

enum class ResponseCodeVal : std::uint8_t
{
    Success = 0,
    GrantedQoS0 = Success,
    GrantedQoS1 = 1,
    GrantedQoS2 = 2,
    DisconnectWithWill = 4,
    NoMatchingSubscribers = 16,
    NoSubscriptionExisted = 17,
    ContinueAuthentication = 24,
    Reauthenticate = 25,
    UnspecifiedError = 128,
    MalformedPacket = 129,
    ProtocolError = 130,
    ImplementationSpecificError = 131,
    UnsupportedProtocolVersion = 132,
    ClientIdNotValid = 133,
    BadAuth = 134,
    NotAuthorised = 135,
    ServerUnavailable = 136,
    ServerBusy = 137,
    Banned = 138,
    ServerShuttingDown = 139,
    BadAuthMethod = 140,
    KeepAliveTimeout = 141,
    SessionTakenOver = 142,
    TopicFilterInvalid = 143,
    TopicNameInvalid = 144,
    PacketIdInUse = 145,
    PacketIdNotFound = 146,
    ReceiveMaxExceeded = 147,
    TopicAliasInvalid = 148,
    PacketTooLarge = 149,
    MessageRateTooHigh = 150,
    QuotaExceeded = 151,
    AdministrativeAction = 152,
    PayloadFormatInvalid = 153,
    RetainNotSupported = 154,
    QosNotSupported = 155,
    UseAnotherServer = 156,
    ServerMoved = 157,
    SharedSubNotSupported = 158,
    ConnectionRateExceeded = 159,
    MaxConnectTime = 160,
    SubIdNotSupported = 161,
    WildcardSubNotSupported = 162,
};

struct ResponseCode : public
    comms::field::EnumValue<
        FieldBase,
        ResponseCodeVal
    >
{
    bool valid() const
    {
        using Base = typename std::decay<decltype(comms::field::toFieldBase(*this))>::type;

        static const ResponseCodeVal Values[] = {
            ResponseCodeVal::Success,
            ResponseCodeVal::GrantedQoS1,
            ResponseCodeVal::GrantedQoS2,
            ResponseCodeVal::DisconnectWithWill,
            ResponseCodeVal::NoMatchingSubscribers,
            ResponseCodeVal::NoSubscriptionExisted,
            ResponseCodeVal::ContinueAuthentication,
            ResponseCodeVal::Reauthenticate,
            ResponseCodeVal::UnspecifiedError,
            ResponseCodeVal::MalformedPacket,
            ResponseCodeVal::ProtocolError,
            ResponseCodeVal::ImplementationSpecificError,
            ResponseCodeVal::UnsupportedProtocolVersion,
            ResponseCodeVal::ClientIdNotValid,
            ResponseCodeVal::BadAuth,
            ResponseCodeVal::NotAuthorised,
            ResponseCodeVal::ServerUnavailable,
            ResponseCodeVal::ServerBusy,
            ResponseCodeVal::Banned,
            ResponseCodeVal::ServerShuttingDown,
            ResponseCodeVal::BadAuthMethod,
            ResponseCodeVal::KeepAliveTimeout,
            ResponseCodeVal::SessionTakenOver,
            ResponseCodeVal::TopicFilterInvalid,
            ResponseCodeVal::TopicNameInvalid,
            ResponseCodeVal::PacketIdInUse,
            ResponseCodeVal::PacketIdNotFound,
            ResponseCodeVal::ReceiveMaxExceeded,
            ResponseCodeVal::TopicAliasInvalid,
            ResponseCodeVal::PacketTooLarge,
            ResponseCodeVal::MessageRateTooHigh,
            ResponseCodeVal::QuotaExceeded,
            ResponseCodeVal::AdministrativeAction,
            ResponseCodeVal::PayloadFormatInvalid,
            ResponseCodeVal::RetainNotSupported,
            ResponseCodeVal::QosNotSupported,
            ResponseCodeVal::UseAnotherServer,
            ResponseCodeVal::ServerMoved,
            ResponseCodeVal::SharedSubNotSupported,
            ResponseCodeVal::ConnectionRateExceeded,
            ResponseCodeVal::MaxConnectTime,
            ResponseCodeVal::SubIdNotSupported,
            ResponseCodeVal::WildcardSubNotSupported
        };

        auto val = Base::value();
        auto iter = std::lower_bound(std::begin(Values), std::end(Values), val);
        if ((iter == std::end(Values)) || (*iter != val)) {
            return false;
        }

        return Base::valid();
    }
};

enum class RetainHandlingVal : std::uint8_t
{
    Send,
    SendNew,
    DontSend,
    NumOfValues
};

using RetainHandling =
    comms::field::EnumValue<
        FieldBase,
        RetainHandlingVal,
        comms::option::ValidNumValueRange<0, (int)RetainHandlingVal::NumOfValues - 1>,
        comms::option::FixedBitLength<2>
    >;

struct SubOptionsFlags : public
    comms::field::BitmaskValue<
        FieldBase,
        comms::option::FixedBitLength<2>,
        comms::option::BitmaskReservedBits<0xfc, 0>
    >
{
    COMMS_BITMASK_BITS(nl, rap);
};

class SubOptions : public
    comms::field::Bitfield<
        FieldBase,
        std::tuple<
            common::field::QoS<comms::option::FixedBitLength<2> >,
            SubOptionsFlags,
            RetainHandling,
            common::field::ReservedBits<comms::option::FixedBitLength<2> >
        >
    >
{
public:
    COMMS_FIELD_MEMBERS_ACCESS(qos, flags, retainHandling, reserved);
};

class SubElem : public
    comms::field::Bundle<
        FieldBase,
        std::tuple<
            common::field::Topic,
            SubOptions
        >
    >
{
public:
    COMMS_FIELD_MEMBERS_ACCESS(topic, options);
};

struct SubscribePayload : public
    comms::field::ArrayList<
        FieldBase,
        SubElem
    >
{
    bool valid() const
    {
        using Base = typename std::decay<decltype(comms::field::toFieldBase(*this))>::type;
        return (!Base::value().empty()) && Base::valid();
    }
};

struct ResponseCodeList : public
    comms::field::ArrayList<
        FieldBase,
        ResponseCode
    >
{
    bool valid() const
    {
        using Base = typename std::decay<decltype(comms::field::toFieldBase(*this))>::type;
        return (!Base::value().empty()) && Base::valid();
    }
};

} // namespace field

} // namespace v5

}  // namespace protocol

}  // namespace mqtt
