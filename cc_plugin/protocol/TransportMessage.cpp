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

namespace protocol
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
    return
        cc::property::field::ForField<mqtt::protocol::MsgIdField>()
            .name("ID")
            .add("CONNECT", mqtt::MsgId_CONNECT)
            .add("CONNACK", mqtt::MsgId_CONNACK)
            .add("PUBLISH", mqtt::MsgId_PUBLISH)
            .add("PUBACK", mqtt::MsgId_PUBACK)
            .add("PUBREC", mqtt::MsgId_PUBREC)
            .add("PUBREL", mqtt::MsgId_PUBREL)
            .add("PUBCOMP", mqtt::MsgId_PUBCOMP)
            .add("SUBSCRIBE", mqtt::MsgId_SUBSCRIBE)
            .add("SUBACK", mqtt::MsgId_SUBACK)
            .add("UNSUBSCRIBE", mqtt::MsgId_UNSUBSCRIBE)
            .add("UNSUBACK", mqtt::MsgId_UNSUBACK)
            .add("PINGREQ", mqtt::MsgId_PINGREQ)
            .add("PINGRESP", mqtt::MsgId_PINGRESP)
            .add("DISCONNECT", mqtt::MsgId_DISCONNECT)
            .serialisedHidden()
            .asMap();
}

QVariantMap createFlagsProperties()
{
    return
        cc::property::field::ForField<mqtt::protocol::FlagsField>()
            .name("Flags")
            .serialisedHidden()
            .asMap();
}

QVariantMap createIdAndFlagsProperties()
{
    return
        cc::property::field::ForField<mqtt::protocol::MsgIdFlagsBundle>()
            .add(createFlagsProperties())
            .add(createMsgIdMemberData())
            .asMap();
}

QVariantMap createSizeProperties()
{
    return
        cc::property::field::ForField<mqtt::RemSizeField>()
            .name("Size")
            .asMap();
}

QVariantMap createDataProperties()
{
    return cc::property::field::ArrayList().name("Data").asMap();
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

}  // namespace protocol

}  // namespace cc_plugin

}  // namespace mqtt

