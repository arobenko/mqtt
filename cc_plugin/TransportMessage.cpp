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

#include "TransportMessage.h"

#include <cassert>

#include <QtCore/QVariantMap>

#include "mqtt/MsgId.h"
#include "comms/Assert.h"

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

namespace
{

enum FieldIdx
{
    FieldIdx_IdAndFlags,
    FieldIdx_Size,
    FieldIdx_Data,
    FieldIdx_NumOfValues
};

QVariantMap createMsgIdMemberData()
{
    QVariantList enumProps;
    cc::Property::appendEnumValue(enumProps, "CONNECT", mqtt::MsgId_CONNECT);
    cc::Property::appendEnumValue(enumProps, "CONNACK", mqtt::MsgId_CONNACK);
    cc::Property::appendEnumValue(enumProps, "PUBLISH", mqtt::MsgId_PUBLISH);
    cc::Property::appendEnumValue(enumProps, "PUBACK", mqtt::MsgId_PUBACK);
    cc::Property::appendEnumValue(enumProps, "PUBREC", mqtt::MsgId_PUBREC);
    cc::Property::appendEnumValue(enumProps, "PUBREL", mqtt::MsgId_PUBREL);
    cc::Property::appendEnumValue(enumProps, "PUBCOMP", mqtt::MsgId_PUBCOMP);
    cc::Property::appendEnumValue(enumProps, "SUBSCRIBE", mqtt::MsgId_SUBSCRIBE);
    cc::Property::appendEnumValue(enumProps, "SUBACK", mqtt::MsgId_SUBACK);
    cc::Property::appendEnumValue(enumProps, "UNSUBSCRIBE", mqtt::MsgId_UNSUBSCRIBE);
    cc::Property::appendEnumValue(enumProps, "UNSUBACK", mqtt::MsgId_UNSUBACK);
    cc::Property::appendEnumValue(enumProps, "PINGREQ", mqtt::MsgId_PINGREQ);
    cc::Property::appendEnumValue(enumProps, "PINGRESP", mqtt::MsgId_PINGRESP);
    cc::Property::appendEnumValue(enumProps, "DISCONNECT", mqtt::MsgId_DISCONNECT);

    GASSERT(enumProps.size() == (mqtt::MsgId_NumOfValues - 1));

    auto props = cc::Property::createPropertiesMap("ID", std::move(enumProps));
    cc::Property::setSerialisedHidden(props);
    return props;
}

QVariantMap createFlagsProperties()
{
    auto props = cc::Property::createPropertiesMap("Flags");
    cc::Property::setSerialisedHidden(props);
    return props;
}

QVariantMap createIdAndFlagsProperties()
{
    QVariantList membersProps;
    membersProps.append(createFlagsProperties());
    membersProps.append(createMsgIdMemberData());

    QVariantMap props;
    cc::Property::setData(props, std::move(membersProps));
    return props;
}

QVariantMap createSizeProperties()
{
    return cc::Property::createPropertiesMap("Size");
}

QVariantMap createDataProperties()
{
    return cc::Property::createPropertiesMap("Data");
}

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(createIdAndFlagsProperties());
    props.append(createSizeProperties());
    props.append(createDataProperties());

    assert(props.size() == FieldIdx_NumOfValues);
    return props;
}

}  // namespace

const QVariantList& TransportMessage::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}


}  // namespace cc_plugin

}  // namespace mqtt

