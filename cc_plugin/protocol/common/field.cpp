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

#include "cc_plugin/protocol/common/field.h"

#include <cassert>

#include "comms_champion/comms_champion.h"
#include "mqtt/protocol/common/MsgId.h"
#include "mqtt/protocol/v5/field.h"
#include "mqtt/protocol/v311/field.h"

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
{

namespace common
{

namespace field
{

namespace
{

using ProtocolVersionVal = mqtt::protocol::common::field::ProtocolVersionVal ;
using PropertyIdVal = mqtt::protocol::v5::field::PropertyIdVal;

QVariantMap createProps_msgId(ProtocolVersionVal version)
{
    auto props =
        cc::property::field::EnumValue()
            .name("ID")
            .serialisedHidden()
            .add("CONNECT", mqtt::protocol::common::MsgId_CONNECT)
            .add("CONNACK", mqtt::protocol::common::MsgId_CONNACK)
            .add("PUBLISH", mqtt::protocol::common::MsgId_PUBLISH)
            .add("PUBACK", mqtt::protocol::common::MsgId_PUBACK)
            .add("PUBREC", mqtt::protocol::common::MsgId_PUBREC)
            .add("PUBREL", mqtt::protocol::common::MsgId_PUBREL)
            .add("PUBCOMP", mqtt::protocol::common::MsgId_PUBCOMP)
            .add("SUBSCRIBE", mqtt::protocol::common::MsgId_SUBSCRIBE)
            .add("SUBACK", mqtt::protocol::common::MsgId_SUBACK)
            .add("UNSUBSCRIBE", mqtt::protocol::common::MsgId_UNSUBSCRIBE)
            .add("UNSUBACK", mqtt::protocol::common::MsgId_UNSUBACK)
            .add("PINGREQ", mqtt::protocol::common::MsgId_PINGREQ)
            .add("PINGRESP", mqtt::protocol::common::MsgId_PINGRESP)
            .add("DISCONNECT", mqtt::protocol::common::MsgId_DISCONNECT);

    if (ProtocolVersionVal::v5 <= version) {
        props.add("AUTH", mqtt::protocol::common::MsgId_AUTH);
    }

    return props.asMap();
}

QVariantMap createProps_flags()
{
    return
        cc::property::field::ForField<mqtt::protocol::common::field::Flags>()
            .name("Flags")
            .serialisedHidden()
            .asMap();
}

QVariantMap createProps_idFlagsBundle(ProtocolVersionVal version)
{
    return
        cc::property::field::Bitfield()
            .add(createProps_flags())
            .add(createProps_msgId(version))
            .asMap();
}

QVariantMap createProps_remSize()
{
    return
        cc::property::field::ForField<mqtt::protocol::common::field::RemSize>()
            .name("Size")
            .asMap();
}

QVariantMap createProps_data()
{
    return cc::property::field::ArrayList().name("Data").asMap();
}

const QString& name_willTopic()
{
    static const QString& Str("Will Topic");
    return Str;
}

const QString& name_willMessage()
{
    static const QString& Str("Will Message");
    return Str;
}

const QString& name_userName()
{
    static const QString& Str("User Name");
    return Str;
}

const QString& name_password()
{
    static const QString& Str("Password");
    return Str;
}

QVariantMap createProps_connectFlagsLow()
{
    using Field = mqtt::protocol::common::field::ConnectFlagsLow;
    auto props =
        cc::property::field::ForField<Field>()
            .name("Connect Flags")
            .add(Field::BitIdx_cleanSession, "Clean Session")
            .add("Will Flag")
            .serialisedHidden();

    assert(props.bits().size() == Field::BitIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_connectFlagsHigh()
{
    using Field = mqtt::protocol::common::field::ConnectFlagsHigh;
    auto props =
        cc::property::field::ForField<Field>()
            .add("Will Retain")
            .add("Password Flag")
            .add("User Name Flag")
            .serialisedHidden();
    assert(props.bits().size() == Field::BitIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_qos(const QString& name = "qos", bool serialisedHidden = true)
{
    using Field = mqtt::protocol::common::field::QoS<>;
    auto props =
        cc::property::field::ForField<Field>()
            .name(name)
            .add("At most once delivery")
            .add("At least once delivery")
            .add("Exactly once delivery");

    if (serialisedHidden) {
        props.serialisedHidden();
    }

    assert(props.values().size() == (int)mqtt::protocol::common::field::QosVal::NumOfValues);
    return props.asMap();
}

QVariantMap createProps_connectFlags()
{
    using Field = mqtt::protocol::common::field::ConnectFlags;
    auto props =
        cc::property::field::ForField<Field>()
            .name("Connect Flags")
            .add(createProps_connectFlagsLow())
            .add(createProps_qos("Will QoS"))
            .add(createProps_connectFlagsHigh());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_willTopic()
{
    using Field = mqtt::protocol::common::field::WillTopic::ValueType;
    return
        cc::property::field::ForField<Field>()
            .name(name_willTopic())
            .asMap();
}

QVariantMap createProps_willTopicOpt()
{
    using Field = mqtt::protocol::common::field::WillTopic;
    return
        cc::property::field::ForField<Field>()
            .name(name_willTopic())
            .field(createProps_willTopic())
            .uncheckable()
            .asMap();
}

QVariantMap createProps_willMessage()
{
    using Field = mqtt::protocol::common::field::WillMessage::ValueType;
    return
        cc::property::field::ForField<Field>()
            .name(name_willMessage())
            .asMap();
}

QVariantMap createProps_willMessageOpt()
{
    using Field = mqtt::protocol::common::field::WillMessage;
    return
        cc::property::field::ForField<Field>()
            .name(name_willMessage())
            .field(createProps_willMessage())
            .uncheckable()
            .asMap();
}

QVariantMap createProps_userName()
{
    using Field = mqtt::protocol::common::field::UserName::ValueType;
    return
        cc::property::field::ForField<Field>()
            .name(name_userName())
            .asMap();
}

QVariantMap createProps_userNameOpt()
{
    using Field = mqtt::protocol::common::field::UserName;
    return
        cc::property::field::ForField<Field>()
            .name(name_userName())
            .field(createProps_userName())
            .uncheckable()
            .asMap();
}

QVariantMap createProps_password()
{
    using Field = mqtt::protocol::common::field::Password::ValueType;
    return
        cc::property::field::ForField<Field>()
            .name(name_password())
            .asMap();
}

QVariantMap createProps_passwordOpt()
{
    using Field = mqtt::protocol::common::field::Password;
    return
        cc::property::field::ForField<Field>()
            .name(name_password())
            .field(createProps_password())
            .uncheckable()
            .asMap();
}

QVariantMap createProps_protocolName()
{
    using Field = mqtt::protocol::common::field::ProtocolName;
    return
        cc::property::field::ForField<Field>()
            .name("Protocol Name")
            .asMap();
}

QVariantMap createProps_protocolVersion()
{
    return
        cc::property::field::EnumValue()
            .name("Protocol Version")
            .add("v3.1.1", (int)mqtt::protocol::common::field::ProtocolVersionVal::v311)
            .add("v5", (int)mqtt::protocol::common::field::ProtocolVersionVal::v5)
            .readOnly()
            .asMap();
}

QVariantMap createProps_keepAlive()
{
    using Field = mqtt::protocol::common::field::KeepAlive;
    return
        cc::property::field::ForField<Field>()
            .name("Keep Alive")
            .asMap();
}

QVariantMap createProps_clientId()
{
    using Field = mqtt::protocol::common::field::ClientId;
    return
        cc::property::field::ForField<Field>()
            .name("Client ID")
            .asMap();
}

QVariantMap createProps_propId(PropertyIdVal val)
{
    static const std::map<PropertyIdVal, QString> Map = {
        std::make_pair(PropertyIdVal::PayloadFormatIndicator, "Payload Format Indicator"),
        std::make_pair(PropertyIdVal::PublishExpiryInterval, "Publish Expiry Interval"),
        std::make_pair(PropertyIdVal::ContentType, "Content Type"),
        std::make_pair(PropertyIdVal::ResponseTopic, "Response Topic"),
        std::make_pair(PropertyIdVal::CorrelationData, "Correlation Data"),
        std::make_pair(PropertyIdVal::SubscriptionIdentifier, "Subscription Identifier"),
        std::make_pair(PropertyIdVal::SessionExpiryInterval, "Session Expiry Interval"),
        std::make_pair(PropertyIdVal::AssignedClientIdentifier, "Assigned Client Identifier"),
        std::make_pair(PropertyIdVal::ServerKeepAlive, "Server Keep Alive"),
        std::make_pair(PropertyIdVal::AuthMethod, "Auth Method"),
        std::make_pair(PropertyIdVal::AuthData, "Auth Data"),
        std::make_pair(PropertyIdVal::RequestProblemInformation, "Request Problem Information"),
        std::make_pair(PropertyIdVal::WillDelayInterval, "Will Delay Interval"),
        std::make_pair(PropertyIdVal::RequestResponseInformation, "Request Response Information"),
        std::make_pair(PropertyIdVal::ResponseInformation, "Response Information"),
        std::make_pair(PropertyIdVal::ServerReference, "Server Reference"),
        std::make_pair(PropertyIdVal::ReasonString, "Reason String"),
        std::make_pair(PropertyIdVal::ReceiveMaximum, "Receive Maximum"),
        std::make_pair(PropertyIdVal::TopicAliasMaximum, "Topic Alias Maximum"),
        std::make_pair(PropertyIdVal::TopicAlias, "Topic Alias"),
        std::make_pair(PropertyIdVal::MaximumQoS, "Maximum QoS"),
        std::make_pair(PropertyIdVal::RetainAvailable, "Retain Available"),
        std::make_pair(PropertyIdVal::UserProperty, "User Property"),
        std::make_pair(PropertyIdVal::MaximumPacketSize, "Maximum Packet Size"),
        std::make_pair(PropertyIdVal::WildcardSubscriptionAvailable, "Wildcard Subscription Available"),
        std::make_pair(PropertyIdVal::SubscriptionIdentifierAvailable, "Subscription Identifier Available"),
        std::make_pair(PropertyIdVal::SharedSubscriptionAvailable, "Shared Subscription Available")
    };

    auto iter = Map.find(val);
    if (iter == Map.end()) {
        assert(!"Invalid value");
        return QVariantMap();
    }

    return
        cc::property::field::EnumValue()
            .add(iter->second, (int)val)
            .readOnly()
            .asMap();
}

QVariantMap createProps_payloadFromatIndicator()
{
    using Field = mqtt::protocol::v5::field::PayloadFormatIndicator;
    auto props =
        cc::property::field::ForField<Field>()
            .add("Unspecified")
            .add("UTF-8 Char Data");
    assert(props.values().size() == (int)Field::ValueType::NumOfValues);
    return props.asMap();
}

QVariantMap createProps_payloadFromatIndicatorProp()
{
    using Field = mqtt::protocol::v5::field::PayloadFormatIndicatorProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::PayloadFormatIndicator))
            .add(createProps_payloadFromatIndicator());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_publishExpiryIntervalProp()
{
    using Field = mqtt::protocol::v5::field::PublishExpiryIntervalProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::PublishExpiryInterval))
            .add(cc::property::field::IntValue().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_contentTypeProp()
{
    using Field = mqtt::protocol::v5::field::ContentTypeProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::ContentType))
            .add(cc::property::field::String().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_responseTopicProp()
{
    using Field = mqtt::protocol::v5::field::ContentTypeProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::ResponseTopic))
            .add(cc::property::field::String().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_correlationDataProp()
{
    using Field = mqtt::protocol::v5::field::ContentTypeProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::CorrelationData))
            .add(cc::property::field::ArrayList().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_subscriptionIdentifierProp()
{
    using Field = mqtt::protocol::v5::field::SubscriptionIdentifierProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::SubscriptionIdentifier))
            .add(cc::property::field::IntValue().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_sessionExpiryIntervalProp()
{
    using Field = mqtt::protocol::v5::field::SessionExpiryIntervalProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::SessionExpiryInterval))
            .add(cc::property::field::IntValue().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_assignedClientIdentifierProp()
{
    using Field = mqtt::protocol::v5::field::AssignedClientIdentifierProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::AssignedClientIdentifier))
            .add(cc::property::field::String().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_serverKeepAliveProp()
{
    using Field = mqtt::protocol::v5::field::SubscriptionIdentifierProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::ServerKeepAlive))
            .add(cc::property::field::IntValue().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_authMethodProp()
{
    using Field = mqtt::protocol::v5::field::AuthMethodProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::AuthMethod))
            .add(cc::property::field::String().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_authDataProp()
{
    using Field = mqtt::protocol::v5::field::AuthMethodProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::AuthData))
            .add(cc::property::field::ArrayList().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_boolPropValue(const QString& name = QString())
{
    using Field = mqtt::protocol::v5::field::BoolPropValue;
    auto props =
        cc::property::field::ForField<Field>()
            .name(name)
            .add("False")
            .add("True");
    assert(props.values().size() == (int)Field::ValueType::NumOfValues);
    return props.asMap();
}

QVariantMap createProps_requestProblemInformationProp()
{
    using Field = mqtt::protocol::v5::field::RequestProblemInformationProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::RequestProblemInformation))
            .add(createProps_boolPropValue());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_willDelayIntervalProp()
{
    using Field = mqtt::protocol::v5::field::WillDelayIntervalProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::WillDelayInterval))
            .add(cc::property::field::IntValue().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_requestResponseInformationProp()
{
    using Field = mqtt::protocol::v5::field::RequestResponseInformationProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::RequestResponseInformation))
            .add(createProps_boolPropValue());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_responseInformationProp()
{
    using Field = mqtt::protocol::v5::field::ResponseInformationProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::ResponseInformation))
            .add(cc::property::field::String().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_serverReferenceProp()
{
    using Field = mqtt::protocol::v5::field::ServerReferenceProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::ServerReference))
            .add(cc::property::field::String().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_reasonStringProp()
{
    using Field = mqtt::protocol::v5::field::ReasonStringProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::ReasonString))
            .add(cc::property::field::String().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_receiveMaximumProp()
{
    using Field = mqtt::protocol::v5::field::ReceiveMaximumProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::ReceiveMaximum))
            .add(cc::property::field::IntValue().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_topicAliasMaximumProp()
{
    using Field = mqtt::protocol::v5::field::TopicAliasMaximumProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::TopicAliasMaximum))
            .add(cc::property::field::IntValue().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_topicAliasProp()
{
    using Field = mqtt::protocol::v5::field::TopicAliasProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::TopicAlias))
            .add(cc::property::field::IntValue().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_maximumQosProp()
{
    using Field = mqtt::protocol::v5::field::MaximumQosProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::MaximumQoS))
            .add(createProps_qos(QString(), false));
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_retainAvailableProp()
{
    using Field = mqtt::protocol::v5::field::RetainAvailableProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::RetainAvailable))
            .add(createProps_boolPropValue());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_userPropertyProp()
{
    using PairField = mqtt::protocol::v5::field::UserProperty;
    auto pairProps =
            cc::property::field::ForField<PairField>()
                .add(cc::property::field::String().name("key").asMap())
                .add(cc::property::field::String().name("value").asMap());

    assert(pairProps.members().size() == PairField::FieldIdx_numOfValues);

    using Field = mqtt::protocol::v5::field::UserPropertyProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::UserProperty))
            .add(pairProps.asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_maximumPacketSizeProp()
{
    using Field = mqtt::protocol::v5::field::MaximumPacketSizeProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::MaximumPacketSize))
            .add(cc::property::field::IntValue().asMap());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_wildcardSubscriptionAvailableProp()
{
    using Field = mqtt::protocol::v5::field::WildcardSubscriptionAvailableProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::WildcardSubscriptionAvailable))
            .add(createProps_boolPropValue());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_subscriptionIdentifierAvailableProp()
{
    using Field = mqtt::protocol::v5::field::SubscriptionIdentifierAvailableProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::SubscriptionIdentifierAvailable))
            .add(createProps_boolPropValue());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_sharedSubscriptionAvailableProp()
{
    using Field = mqtt::protocol::v5::field::SharedSubscriptionAvailableProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::SharedSubscriptionAvailable))
            .add(createProps_boolPropValue());
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_propertyVar()
{
    using Field = mqtt::protocol::v5::field::PropertyVar;
    auto props =
        cc::property::field::ForField<Field>()
            .serialisedHidden()
            .setIndexHidden()
            .add(createProps_payloadFromatIndicatorProp())
            .add(createProps_publishExpiryIntervalProp())
            .add(createProps_contentTypeProp())
            .add(createProps_responseTopicProp())
            .add(createProps_correlationDataProp())
            .add(createProps_subscriptionIdentifierProp())
            .add(createProps_sessionExpiryIntervalProp())
            .add(createProps_assignedClientIdentifierProp())
            .add(createProps_serverKeepAliveProp())
            .add(createProps_authMethodProp())
            .add(createProps_authDataProp())
            .add(createProps_requestProblemInformationProp())
            .add(createProps_willDelayIntervalProp())
            .add(createProps_requestResponseInformationProp())
            .add(createProps_responseInformationProp())
            .add(createProps_serverReferenceProp())
            .add(createProps_reasonStringProp())
            .add(createProps_receiveMaximumProp())
            .add(createProps_topicAliasMaximumProp())
            .add(createProps_topicAliasProp())
            .add(createProps_maximumQosProp())
            .add(createProps_retainAvailableProp())
            .add(createProps_userPropertyProp())
            .add(createProps_maximumPacketSizeProp())
            .add(createProps_wildcardSubscriptionAvailableProp())
            .add(createProps_subscriptionIdentifierAvailableProp())
            .add(createProps_sharedSubscriptionAvailableProp())
        ;
    assert(props.members().size() == Field::FieldIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_properties()
{
    return
        cc::property::field::ArrayList()
            .name("Properties")
            .add(createProps_propertyVar())
            .showPrefix()
            .prefixName("Length")
            .serialisedHidden()
            .asMap();
}

QVariantMap createProps_connectAcknowledgeFlags()
{
    using Field = mqtt::protocol::common::field::ConnackFlags;
    auto props =
        cc::property::field::ForField<Field>()
            .name("Connect Acknowledge Flags")
            .add("Session Present");
    assert(props.bits().size() == Field::BitIdx_numOfValues);
    return props.asMap();
}

QVariantMap createProps_connackResponseCodeV311()
{
    using Field = mqtt::protocol::v311::field::ConnackResponseCode;
    auto props =
        cc::property::field::ForField<Field>()
            .name("Response Code")
            .add("Accepted")
            .add("Wrong Protocol Version")
            .add("Identifier Rejected")
            .add("Server Unavailable")
            .add("Bad Username or Password")
            .add("Not Authorized");
    assert(props.values().size() == (int)Field::ValueType::NumOfValues);
    return props.asMap();
}

QVariantMap createProps_responseCodeV5(bool isSuback = false)
{
    using Field = mqtt::protocol::v5::field::ResponseCode;
    auto props =
        cc::property::field::ForField<Field>()
            .name("Response Code");
    if (isSuback) {
        props.add("GrantedQoS0", (int)Field::ValueType::GrantedQoS0);
    }
    else {
        props.add("Success", (int)Field::ValueType::Success);
    }
    return props
            .add("Granted QoS1", (int)Field::ValueType::GrantedQoS1)
            .add("Granted QoS2", (int)Field::ValueType::GrantedQoS2)
            .add("No Matching Subscribers", (int)Field::ValueType::NoMatchingSubscribers)
            .add("Unspecified Error", (int)Field::ValueType::UnspecifiedError)
            .add("Malformed Packet", (int)Field::ValueType::MalformedPacket)
            .add("Protocol Error", (int)Field::ValueType::ProtocolError)
            .add("Implementation Specific Error", (int)Field::ValueType::ImplementationSpecificError)
            .add("Unsupported Protocol Version", (int)Field::ValueType::UnsupportedProtocolVersion)
            .add("Client Id Not Valid", (int)Field::ValueType::ClientIdNotValid)
            .add("Bad Authentication", (int)Field::ValueType::BadAuth)
            .add("Not Authorised", (int)Field::ValueType::NotAuthorised)
            .add("Server Unavailable", (int)Field::ValueType::ServerUnavailable)
            .add("Server Busy", (int)Field::ValueType::ServerBusy)
            .add("Banned", (int)Field::ValueType::Banned)
            .add("Bad Auth Method", (int)Field::ValueType::BadAuthMethod)
            .add("Topic Name Invalid", (int)Field::ValueType::TopicNameInvalid)
            .add("Packet ID In Use", (int)Field::ValueType::PacketIdInUse)
            .add("Packet ID Not Found", (int)Field::ValueType::PacketIdNotFound)
            .add("Packet Too Large", (int)Field::ValueType::PacketTooLarge)
            .add("Quota Exceeded", (int)Field::ValueType::QuotaExceeded)
            .add("Payload Format Invalid", (int)Field::ValueType::PayloadFormatInvalid)
            .add("Retain Not Supported", (int)Field::ValueType::RetainNotSupported)
            .add("Use Another Server", (int)Field::ValueType::UseAnotherServer)
            .add("Server Moved", (int)Field::ValueType::ServerMoved)
            .add("Connection Rate Exceeded", (int)Field::ValueType::ConnectionRateExceeded)
            .asMap();
}

QVariantMap createProps_retain()
{
    return
        cc::property::field::BitmaskValue()
            .name("Flags")
            .add("RETAIN")
            .serialisedHidden()
            .asMap();
}

QVariantMap createProps_dup()
{
    return
        cc::property::field::BitmaskValue()
            .add("DUP")
            .serialisedHidden()
            .asMap();
}

QVariantMap createProps_reserved()
{
    return cc::property::field::IntValue().hidden().asMap();
}

QVariantMap createProps_publishFlags()
{
    return
        cc::property::field::Bitfield()
            .name("Flags")
            .add(createProps_retain())
            .add(createProps_qos())
            .add(createProps_dup())
            .add(createProps_reserved())
            .serialisedHidden()
            .asMap();
}

QVariantMap createProps_topic()
{
    return cc::property::field::String().name("Topic").asMap();
}

QVariantMap createProps_payload()
{
    return cc::property::field::ArrayList().name("Payload").asMap();
}

const QString& name_packetId()
{
    static const QString Str("Packet ID");
    return Str;
}

QVariantMap createProps_packetId()
{
    return cc::property::field::IntValue().name(name_packetId()).asMap();
}

QVariantMap createProps_packetIdOpt()
{
    return cc::property::field::Optional()
        .name(name_packetId())
        .field(createProps_packetId())
        .uncheckable()
        .asMap();
}

// TODO
} // namespace

QVariantList createProps_transportFields(ProtocolVersionVal version)
{
    QVariantList props;
    props.append(createProps_idFlagsBundle(version));
    props.append(createProps_remSize());
    props.append(createProps_data());
    return props;
}

const QVariantList& emptyList()
{
    static const QVariantList List;
    return List;
}

QVariantList createProps_connect(ProtocolVersionVal version)
{
    QVariantList props;
    props.append(createProps_protocolName());
    props.append(createProps_protocolVersion());
    props.append(createProps_connectFlags());
    props.append(createProps_keepAlive());
    if (ProtocolVersionVal::v5 <= version) {
        props.append(createProps_properties());
    }

    props.append(createProps_clientId());
    props.append(createProps_willTopicOpt());
    props.append(createProps_willMessageOpt());
    props.append(createProps_userNameOpt());
    props.append(createProps_passwordOpt());
    return props;
}

QVariantList createProps_connack(ProtocolVersionVal version)
{
    QVariantList props;
    props.append(createProps_connectAcknowledgeFlags());
    if (version < ProtocolVersionVal::v5) {
        props.append(createProps_connackResponseCodeV311());
        return props;
    }

    props.append(createProps_responseCodeV5());
    props.append(createProps_properties());
    return props;
}

QVariantList createProps_publish(ProtocolVersionVal version)
{
    QVariantList props;
    props.append(createProps_publishFlags());
    props.append(createProps_topic());
    props.append(createProps_packetIdOpt());
    if (ProtocolVersionVal::v5 <= version) {
        props.append(createProps_properties());
    }

    props.append(createProps_payload());
    return props;
}

QVariantList createProps_puback(ProtocolVersionVal version)
{
    QVariantList props;
    props.append(createProps_packetId());
    if (version < ProtocolVersionVal::v5) {
        return props;
    }

    props.append(createProps_responseCodeV5());
    props.append(createProps_properties());
    return props;
}

QVariantList createProps_pubrec(ProtocolVersionVal version)
{
    // The same fields as with PUBACK
    return createProps_puback(version);
}

QVariantList createProps_pubrel(ProtocolVersionVal version)
{
    // The same fields as with PUBACK
    return createProps_puback(version);
}

QVariantList createProps_pubcomp(ProtocolVersionVal version)
{
    // The same fields as with PUBACK
    return createProps_puback(version);
}
// TODO



} // namespace field

} // namespace common

} // namespace protocol

} // namespace cc_plugin

} // namespace mqtt
