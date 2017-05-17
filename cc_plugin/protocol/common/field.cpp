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

#include "field.h"

#include "comms_champion/comms_champion.h"
#include "mqtt/protocol/common/MsgId.h"

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

} // namespace field

} // namespace common

} // namespace protocol

} // namespace cc_plugin

} // namespace mqtt
