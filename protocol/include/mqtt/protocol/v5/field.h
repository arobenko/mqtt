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

template <typename... TOpts>
using ProtocolName = mqtt::protocol::common::field::ProtocolName<TOpts...>;

using ProtocolVersion =
    mqtt::protocol::common::field::ProtocolVersion<ProtocolVersionVal::v5>;

using ConnectFlags = mqtt::protocol::common::field::ConnectFlags;

using KeepAlive = mqtt::protocol::common::field::KeepAlive;

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

template <typename... TOpts>
using ContentType = common::field::String<TOpts...>;

template <typename... TOpts>
using ContentTypeProp =
    Property<PropertyIdVal::ContentType, ContentType<TOpts...> >;

template <typename... TOpts>
using ResponseTopic = common::field::String<TOpts...>;

template <typename... TOpts>
using ResponseTopicProp =
    Property<PropertyIdVal::ResponseTopic, ResponseTopic<TOpts...> >;

template <typename... TOpts>
using CorrelationData = common::field::BinData<TOpts...>;

template <typename... TOpts>
using CorrelationDataProp =
    Property<PropertyIdVal::CorrelationData, CorrelationData<TOpts...> >;

using SubscriptionIdentifier = common::field::VarByteInt;

using SubscriptionIdentifierProp =
    Property<PropertyIdVal::SubscriptionIdentifier, SubscriptionIdentifier >;

using SessionExpiryInterval =
    comms::field::IntValue<
        FieldBase,
        std::uint32_t,
        comms::option::UnitsSeconds
    >;

using SessionExpiryIntervalProp =
    Property<PropertyIdVal::SessionExpiryInterval, SessionExpiryInterval>;

template <typename... TOpts>
using AssignedClientIdentifier = common::field::String<TOpts...>;

template <typename... TOpts>
using AssignedClientIdentifierProp =
    Property<PropertyIdVal::AssignedClientIdentifier, AssignedClientIdentifier<TOpts...> >;

using ServerKeepAlive =
    comms::field::IntValue<
        FieldBase,
        std::uint16_t,
        comms::option::UnitsSeconds
    >;

using ServerKeepAliveProp =
    Property<PropertyIdVal::ServerKeepAlive, ServerKeepAlive>;

template <typename... TOpts>
using AuthMethod = common::field::String<TOpts...>;

template <typename... TOpts>
using AuthMethodProp =
    Property<PropertyIdVal::AuthMethod, AuthMethod<TOpts...> >;

template <typename... TOpts>
using AuthData = common::field::BinData<TOpts...>;

template <typename... TOpts>
using AuthDataProp =
    Property<PropertyIdVal::AuthData, AuthData<TOpts...> >;

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

template <typename... TOpts>
using ResponseInformation = common::field::String<TOpts...>;

template <typename... TOpts>
using ResponseInformationProp =
    Property<PropertyIdVal::ResponseInformation, ResponseInformation<TOpts...> >;

template <typename... TOpts>
using ServerReference = common::field::String<TOpts...>;

template <typename... TOpts>
using ServerReferenceProp =
    Property<PropertyIdVal::ServerReference, ServerReference<TOpts...> >;

template <typename... TOpts>
using ReasonString = common::field::String<TOpts...>;

template <typename... TOpts>
using ReasonStringProp =
    Property<PropertyIdVal::ReasonString, ReasonString<TOpts...> >;

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

template <typename... TOpt>
struct UserProperty : public
        comms::field::Bundle<
            FieldBase,
            std::tuple<
                common::field::String<TOpt...>,
                common::field::String<TOpt...>
            >
        >
{
    COMMS_FIELD_MEMBERS_ACCESS(key, value);
};

template <typename... TOpt>
using UserPropertyProp =
    Property<PropertyIdVal::UserProperty, UserProperty<TOpt...> >;

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
            ContentTypeProp<>, // TODO options
            ResponseTopicProp<>, // TODO options
            CorrelationDataProp<>, // TODO options
            SubscriptionIdentifierProp,
            SessionExpiryIntervalProp,
            AssignedClientIdentifierProp<>, // TODO options
            ServerKeepAliveProp,
            AuthMethodProp<>, // TODO options
            AuthDataProp<>, // TODO options
            RequestProblemInformationProp,
            WillDelayIntervalProp,
            RequestResponseInformationProp,
            ResponseInformationProp<>, // TODO options
            ServerReferenceProp<>, // TODO options
            ReasonStringProp<>, // TODO options
            ReceiveMaximumProp,
            TopicAliasMaximumProp,
            TopicAliasProp,
            MaximumQosProp,
            RetainAvailableProp,
            UserPropertyProp<>, // TODO options
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

template <typename... TOpt>
using Properties =
    comms::field::ArrayList<
        FieldBase,
        PropertyVar,
        comms::option::SequenceSerLengthFieldPrefix<
            mqtt::protocol::common::field::VarByteInt
        >,
        TOpt...
    >;

template <typename... TOpts>
using ClientId = mqtt::protocol::common::field::ClientId<TOpts...>;

template <typename... TOpts>
using WillTopic = mqtt::protocol::common::field::WillTopic<TOpts...>;

template <typename... TOpts>
using WillMessage = mqtt::protocol::common::field::WillMessage<TOpts...>;

template <typename... TOpts>
using UserName = mqtt::protocol::common::field::UserName<TOpts...>;

template <typename... TOpts>
using Password = mqtt::protocol::common::field::Password<TOpts...>;

} // namespace field

} // namespace v5

}  // namespace protocol

}  // namespace mqtt
