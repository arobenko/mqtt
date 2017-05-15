//
// Copyright 2015 - 2016 (C). Alex Robenko. All rights reserved.
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

#include "cc_plugin/protocol/v311/field.h"
#include "Connect.h"

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
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
    return
        cc::property::field::BitmaskValue()
            .name("Connect Flags")
            .add(1, "Clean Session")
            .add("Will Flag")
            .serialisedHidden()
            .asMap();
}

QVariantMap getFlagsQosMemberData()
{
    cc::property::field::EnumValue props;
    props.name("Will QoS").serialisedHidden();
    field::updateQosPropertiesMap(props);
    return props.asMap();
}

QVariantMap getFlagsHighMemberData()
{
    return
        cc::property::field::BitmaskValue()
            .add("Will Retain")
            .add("Password Flag")
            .add("User Name Flag")
            .serialisedHidden()
            .asMap();
}

QVariantMap getWillTopicData()
{
    return cc::property::field::String().name(getWillTopicFieldName()).asMap();
}

QVariantMap getWillMessageData()
{
    return cc::property::field::String().name(getWillMessageFieldName()).asMap();
}

QVariantMap getUserNameData()
{
    return cc::property::field::String().name(getUserNameFieldName()).asMap();
}

QVariantMap getPasswordData()
{
    return cc::property::field::String().name(getPasswordFieldName()).asMap();
}

QVariantMap createNameProperties()
{
    return cc::property::field::String().name("Name").asMap();
}

QVariantMap createLevelProperties()
{
    return cc::property::field::IntValue().name("Level").asMap();
}

QVariantMap createFlagsProperties()
{
    return
        cc::property::field::Bitfield()
            .name("Connect Flags")
            .add(getFlagsLowMemberData())
            .add(getFlagsQosMemberData())
            .add(getFlagsHighMemberData())
            .asMap();
}

QVariantMap createKeepAliveProperties()
{
    return cc::property::field::IntValue().name("Keep Alive").asMap();
}

QVariantMap createClientIdProperties()
{
    return cc::property::field::String().name("Client ID").asMap();
}

QVariantMap createWillTopicProperties()
{
    return
        cc::property::field::Optional()
            .name(getWillTopicFieldName())
            .field(getWillTopicData())
            .uncheckable()
            .asMap();
}

QVariantMap createWillMessageProperties()
{
    return
        cc::property::field::Optional()
            .name(getWillMessageFieldName())
            .field(getWillMessageData())
            .uncheckable()
            .asMap();
}

QVariantMap createUserNameProperties()
{
    return
        cc::property::field::Optional()
            .name(getUserNameFieldName())
            .field(getUserNameData())
            .uncheckable()
            .asMap();
}

QVariantMap updatePasswordProperties()
{
    return
        cc::property::field::Optional()
            .name(getPasswordFieldName())
            .field(getPasswordData())
            .uncheckable()
            .asMap();
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

    assert(props.size() == Connect::FieldIdx_numOfValues);
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

}  // namespace protocol

}  // namespace cc_plugin

}  // namespace mqtt

