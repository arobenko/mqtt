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
    QVariantList bitNames;
    bitNames.append(QVariant());
    bitNames.append("Clean Session");
    bitNames.append("Will Flag");

    auto props = cc::Property::createPropertiesMap("Connect Flags", std::move(bitNames));
    cc::Property::setSerialisedHidden(props);
    return props;
}

QVariantMap getFlagsQosMemberData()
{
    auto props = cc::Property::createPropertiesMap("Will QoS");
    cc::Property::setSerialisedHidden(props);
    field::updateQosPropertiesMap(props);
    return props;
}

QVariantMap getFlagsHighMemberData()
{
    QVariantList bitNames;
    bitNames.append("Will Retain");
    bitNames.append("Password Flag");
    bitNames.append("User Name Flag");

    auto props = cc::Property::createPropertiesMap(getEmptyString(), std::move(bitNames));
    cc::Property::setSerialisedHidden(props);
    return props;
}

QVariantMap getWillTopicData()
{
    return cc::Property::createPropertiesMap(getWillTopicFieldName());
}

QVariantMap getWillMessageData()
{
    return cc::Property::createPropertiesMap(getWillMessageFieldName());
}

QVariantMap getUserNameData()
{
    return cc::Property::createPropertiesMap(getUserNameFieldName());
}

QVariantMap getPasswordData()
{
    return cc::Property::createPropertiesMap(getPasswordFieldName());
}

QVariantMap createNameProperties()
{
    return cc::Property::createPropertiesMap("Name");
}

QVariantMap createLevelProperties()
{
    return cc::Property::createPropertiesMap("Level");
}

QVariantMap createFlagsProperties()
{
    QVariantList memberData;
    memberData.append(getFlagsLowMemberData());
    memberData.append(getFlagsQosMemberData());
    memberData.append(getFlagsHighMemberData());
    return cc::Property::createPropertiesMap("Connect Flags", std::move(memberData));
}

QVariantMap createKeepAliveProperties()
{
    return cc::Property::createPropertiesMap("Keep Alive");
}

QVariantMap createClientIdProperties()
{
    return cc::Property::createPropertiesMap("Client ID");
}

QVariantMap createWillTopicProperties()
{
    auto props = cc::Property::createPropertiesMap(getWillTopicFieldName(), getWillTopicData());
    cc::Property::setUncheckable(props);
    return props;
}

QVariantMap createWillMessageProperties()
{
    auto props = cc::Property::createPropertiesMap(getWillMessageFieldName(), getWillMessageData());
    cc::Property::setUncheckable(props);
    return props;
}

QVariantMap createUserNameProperties()
{
    auto props = cc::Property::createPropertiesMap(getUserNameFieldName(), getUserNameData());
    cc::Property::setUncheckable(props);
    return props;
}

QVariantMap updatePasswordProperties()
{
    auto props = cc::Property::createPropertiesMap(getPasswordFieldName(), getPasswordData());
    cc::Property::setUncheckable(props);
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

