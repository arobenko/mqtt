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

#include "mqtt/protocol/MsgId.h"
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
            .add("CONNECT", mqtt::protocol::MsgId_CONNECT)
            .add("CONNACK", mqtt::protocol::MsgId_CONNACK)
            .add("PUBLISH", mqtt::protocol::MsgId_PUBLISH)
            .add("PUBACK", mqtt::protocol::MsgId_PUBACK)
            .add("PUBREC", mqtt::protocol::MsgId_PUBREC)
            .add("PUBREL", mqtt::protocol::MsgId_PUBREL)
            .add("PUBCOMP", mqtt::protocol::MsgId_PUBCOMP)
            .add("SUBSCRIBE", mqtt::protocol::MsgId_SUBSCRIBE)
            .add("SUBACK", mqtt::protocol::MsgId_SUBACK)
            .add("UNSUBSCRIBE", mqtt::protocol::MsgId_UNSUBSCRIBE)
            .add("UNSUBACK", mqtt::protocol::MsgId_UNSUBACK)
            .add("PINGREQ", mqtt::protocol::MsgId_PINGREQ)
            .add("PINGRESP", mqtt::protocol::MsgId_PINGRESP)
            .add("DISCONNECT", mqtt::protocol::MsgId_DISCONNECT)
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
        cc::property::field::ForField<mqtt::protocol::RemSizeField>()
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

