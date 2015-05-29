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
    static const QString Name("Flags (low)");
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
    static const QString Name("Flags (high)");
    map.insert(cc::Property::name(), QVariant::fromValue(Name));

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

void updateNameProperties(QWidget& fieldWidget)
{
    static const QString Str("Name");
    cc::Property::setNameVal(fieldWidget, Str);
}

void updateLevelProperties(QWidget& fieldWidget)
{
    static const QString Str("Level");
    cc::Property::setNameVal(fieldWidget, Str);
}

void updateFlagsProperties(QWidget& fieldWidget)
{
    static const QString Str("Flags");
    cc::Property::setNameVal(fieldWidget, Str);
    cc::Property::setIndexedDataVal(
        fieldWidget, mqtt::message::ConnectFlagsMemberIdx_FlagsLow, getFlagsLowMemberData());
    cc::Property::setIndexedDataVal(
        fieldWidget, mqtt::message::ConnectFlagsMemberIdx_WillQos, getFlagsQosMemberData());
    cc::Property::setIndexedDataVal(
        fieldWidget, mqtt::message::ConnectFlagsMemberIdx_FlagsHigh, getFlagsHighMemberData());
}

void updateKeepAliveProperties(QWidget& fieldWidget)
{
    static const QString Str("Keep Alive");
    cc::Property::setNameVal(fieldWidget, Str);
}

void updateClientIdProperties(QWidget& fieldWidget)
{
    static const QString Str("Client ID");
    cc::Property::setNameVal(fieldWidget, Str);
}

void updateWillTopicProperties(QWidget& fieldWidget)
{
    cc::Property::setNameVal(fieldWidget, getWillTopicFieldName());
    cc::Property::setDataVal(
        fieldWidget, getWillTopicData());
}

void updateWillMessageProperties(QWidget& fieldWidget)
{
    cc::Property::setNameVal(fieldWidget, getWillMessageFieldName());
    cc::Property::setDataVal(
        fieldWidget, getWillMessageData());
}

void updateUserNameProperties(QWidget& fieldWidget)
{
    cc::Property::setNameVal(fieldWidget, getUserNameFieldName());
    cc::Property::setDataVal(
        fieldWidget, getUserNameData());
}

void updatePasswordProperties(QWidget& fieldWidget)
{
    cc::Property::setNameVal(fieldWidget, getPasswordFieldName());
    cc::Property::setDataVal(
        fieldWidget, getPasswordData());
}

}  // namespace

const char* Connect::nameImpl() const
{
    static const char* Str = "CONNECT";
    return Str;
}

void Connect::updateFieldPropertiesImpl(QWidget& fieldWidget, uint idx) const
{
    typedef std::function<void (QWidget&)> FieldUpdateFunc;
    static const FieldUpdateFunc FuncMap[] = {
        &updateNameProperties,
        &updateLevelProperties,
        &updateFlagsProperties,
        &updateKeepAliveProperties,
        &updateClientIdProperties,
        &updateWillTopicProperties,
        &updateWillMessageProperties,
        &updateUserNameProperties,
        &updatePasswordProperties
    };

    static const unsigned FuncsCount = std::extent<decltype(FuncMap)>::value;

    static_assert(FuncsCount == FieldIdx_NumOfValues,
        "The funcs map is incorrect");

    if (FuncsCount <= idx) {
        return;
    }

    assert(FuncMap[idx]);
    FuncMap[idx](fieldWidget);
}

}  // namespace message

}  // namespace cc_plugin

}  // namespace mqtt

