//
// Copyright 2015 (C). Alex Robenko. All rights reserved.
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

#include <type_traits>
#include <functional>
#include <cassert>

#include "cc_plugin/field/QoS.h"
#include "Connect.h"

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

namespace message
{

namespace
{

const QString& getEmptyString()
{
    static const QString Str;
    return Str;
}

const QString& getWillTopicFieldName()
{
    static const QString& Str("Will Topic");
    return Str;
}

const QString& getWillMessageFieldName()
{
    static const QString& Str("Will Message");
    return Str;
}

const QString& getUserNameFieldName()
{
    static const QString& Str("User Name");
    return Str;
}

const QString& getPasswordFieldName()
{
    static const QString& Str("Password");
    return Str;
}

QVariantMap getFlagsLowMemberData()
{
    QVariantMap map;
    static const QString Name("Connect Flags");
    map.insert(cc::Property::name(), QVariant::fromValue(Name));

    static const QString Map[] = {
        "Clean Session",
        "Will Flag"
    };

    static const unsigned MapSize = std::extent<decltype(Map)>::value;

    for (auto idx = 0U; idx < MapSize; ++idx) {
        map.insert(cc::Property::indexedName(idx + 1), QVariant::fromValue(Map[idx]));
    }
    return map;
}

QVariantMap getFlagsQosMemberData()
{
    QVariantMap map;
    static const QString Name("Will QoS");
    map.insert(cc::Property::name(), QVariant::fromValue(Name));
    field::updateQosPropertiesMap(map);
    return map;
}

QVariantMap getFlagsHighMemberData()
{
    QVariantMap map;
    map.insert(cc::Property::name(), QVariant::fromValue(getEmptyString()));

    static const QString Map[] = {
        "Will Retain",
        "Password Flag",
        "User Name Flag"
    };

    static const unsigned MapSize = std::extent<decltype(Map)>::value;

    for (auto idx = 0U; idx < MapSize; ++idx) {
        map.insert(cc::Property::indexedName(idx), QVariant::fromValue(Map[idx]));
    }
    return map;
}

QVariantMap getWillTopicData()
{
    QVariantMap map;
    map.insert(cc::Property::name(), QVariant::fromValue(getWillTopicFieldName()));
    return map;
}

QVariantMap getWillMessageData()
{
    QVariantMap map;
    map.insert(cc::Property::name(), QVariant::fromValue(getWillMessageFieldName()));
    return map;
}

QVariantMap getUserNameData()
{
    QVariantMap map;
    map.insert(cc::Property::name(), QVariant::fromValue(getUserNameFieldName()));
    return map;
}

QVariantMap getPasswordData()
{
    QVariantMap map;
    map.insert(cc::Property::name(), QVariant::fromValue(getPasswordFieldName()));
    return map;
}

QVariantMap createNameProperties()
{
    static const QString Str("Name");
    QVariantMap props;
    props.insert(cc::Property::name(), QVariant::fromValue(Str));
    return props;
}

QVariantMap createLevelProperties()
{
    static const QString Str("Level");
    QVariantMap props;
    props.insert(cc::Property::name(), QVariant::fromValue(Str));
    return props;
}

QVariantMap createFlagsProperties()
{
    static const QString Str("Connect Flags");
    QVariantMap props;
    props.insert(cc::Property::name(), QVariant::fromValue(Str));
    props.insert(
        cc::Property::indexedData(mqtt::message::ConnectFlagsMemberIdx_FlagsLow),
        getFlagsLowMemberData());
    props.insert(
        cc::Property::indexedData(mqtt::message::ConnectFlagsMemberIdx_WillQos),
        getFlagsQosMemberData());
    props.insert(
        cc::Property::indexedData(mqtt::message::ConnectFlagsMemberIdx_FlagsHigh),
        getFlagsHighMemberData());
    return props;
}

QVariantMap createKeepAliveProperties()
{
    static const QString Str("Keep Alive");
    QVariantMap props;
    props.insert(cc::Property::name(), QVariant::fromValue(Str));
    return props;
}

QVariantMap createClientIdProperties()
{
    static const QString Str("Client ID");
    QVariantMap props;
    props.insert(cc::Property::name(), QVariant::fromValue(Str));
    return props;
}

QVariantMap createWillTopicProperties()
{
    QVariantMap props;
    props.insert(cc::Property::name(), getWillTopicFieldName());
    props.insert(cc::Property::data(), getWillTopicData());
    return props;
}

QVariantMap createWillMessageProperties()
{
    QVariantMap props;
    props.insert(cc::Property::name(), getWillMessageFieldName());
    props.insert(cc::Property::data(), getWillMessageData());
    return props;
}

QVariantMap createUserNameProperties()
{
    QVariantMap props;
    props.insert(cc::Property::name(), getUserNameFieldName());
    props.insert(cc::Property::data(), getUserNameData());
    return props;
}

QVariantMap updatePasswordProperties()
{
    QVariantMap props;
    props.insert(cc::Property::name(), getPasswordFieldName());
    props.insert(cc::Property::data(), getPasswordData());
    return props;
}

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(createNameProperties());
    props.append(createLevelProperties());
    props.append(createFlagsProperties());
    props.append(createKeepAliveProperties());
    props.append(createClientIdProperties());
    props.append(createWillTopicProperties());
    props.append(createWillMessageProperties());
    props.append(createUserNameProperties());
    props.append(updatePasswordProperties());

    assert(props.size() == Connect::FieldIdx_NumOfValues);
    return props;
}

}  // namespace

const char* Connect::nameImpl() const
{
    static const char* Str = "CONNECT";
    return Str;
}

const QVariantList& Connect::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace message

}  // namespace cc_plugin

}  // namespace mqtt

