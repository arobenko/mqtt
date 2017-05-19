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

#include "comms_champion/comms_champion.h"
#include "mqtt/protocol/common/MsgId.h"
#include "mqtt/protocol/v5/field.h"

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

QVariantMap createProps_qos(const QString& name, bool serialisedHidden = true)
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
    using Field = mqtt::protocol::common::field::WillTopic<>::ValueType;
    return
        cc::property::field::ForField<Field>()
            .name(name_willTopic())
            .asMap();
}

QVariantMap createProps_willTopicOpt()
{
    using Field = mqtt::protocol::common::field::WillTopic<>;
    return
        cc::property::field::ForField<Field>()
            .name(name_willTopic())
            .field(createProps_willTopic())
            .uncheckable()
            .asMap();
}

QVariantMap createProps_willMessage()
{
    using Field = mqtt::protocol::common::field::WillMessage<>::ValueType;
    return
        cc::property::field::ForField<Field>()
            .name(name_willMessage())
            .asMap();
}

QVariantMap createProps_willMessageOpt()
{
    using Field = mqtt::protocol::common::field::WillMessage<>;
    return
        cc::property::field::ForField<Field>()
            .name(name_willMessage())
            .field(createProps_willMessage())
            .uncheckable()
            .asMap();
}

QVariantMap createProps_userName()
{
    using Field = mqtt::protocol::common::field::UserName<>::ValueType;
    return
        cc::property::field::ForField<Field>()
            .name(name_userName())
            .asMap();
}

QVariantMap createProps_userNameOpt()
{
    using Field = mqtt::protocol::common::field::UserName<>;
    return
        cc::property::field::ForField<Field>()
            .name(name_userName())
            .field(createProps_userName())
            .uncheckable()
            .asMap();
}

QVariantMap createProps_password()
{
    using Field = mqtt::protocol::common::field::Password<>::ValueType;
    return
        cc::property::field::ForField<Field>()
            .name(name_password())
            .asMap();
}

QVariantMap createProps_passwordOpt()
{
    using Field = mqtt::protocol::common::field::Password<>;
    return
        cc::property::field::ForField<Field>()
            .name(name_password())
            .field(createProps_password())
            .uncheckable()
            .asMap();
}

QVariantMap createProps_protocolName()
{
    using Field = mqtt::protocol::common::field::ProtocolName<>;
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
    using Field = mqtt::protocol::common::field::ClientId<>;
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
    };

    auto iter = Map.find(val);
    if (iter == Map.end()) {
        assert("Invalid value");
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
            .name("value")
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
    using Field = mqtt::protocol::v5::field::PayloadFormatIndicatorProp;
    auto props =
        cc::property::field::ForField<Field>()
            .add(createProps_propId(PropertyIdVal::PublishExpiryInterval))
            .add(cc::property::field::IntValue().name("value").asMap());
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

// TODO



} // namespace field

} // namespace common

} // namespace protocol

} // namespace cc_plugin

} // namespace mqtt
