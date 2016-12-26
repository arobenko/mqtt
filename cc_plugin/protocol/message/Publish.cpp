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

#include "cc_plugin/protocol/field/QoS.h"
#include "cc_plugin/protocol/field/PacketId.h"
#include "Publish.h"

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

QVariantMap createRetainMemberData()
{
    return
        cc::property::field::BitmaskValue()
            .name("Flags")
            .add("RETAIN")
            .serialisedHidden()
            .asMap();
}

QVariantMap createQosMemberData()
{
    cc::property::field::EnumValue props;
    props.name("QoS").serialisedHidden();
    field::updateQosPropertiesMap(props);
    return props.asMap();
}

QVariantMap createDupMemberData()
{
    return
        cc::property::field::BitmaskValue()
            .add("DUP")
            .serialisedHidden()
            .asMap();
}

QVariantMap getReservedMemberData()
{
    return cc::property::field::IntValue().hidden().asMap();
}

QVariantMap createFlagsProperties()
{
    return
        cc::property::field::Bitfield()
            .name("Flags")
            .add(createRetainMemberData())
            .add(createQosMemberData())
            .add(createDupMemberData())
            .add(getReservedMemberData())
            .serialisedHidden()
            .asMap();
}

QVariantMap createTopicProperties()
{
    return cc::property::field::String().name("Topic").asMap();
}

QVariantMap createPayloadProperties()
{
    return cc::property::field::ArrayList().name("Payload").asMap();
}

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(createFlagsProperties());
    props.append(createTopicProperties());
    props.append(field::optionalPacketIdProperties());
    props.append(createPayloadProperties());

    assert(props.size() == Publish::FieldIdx_numOfValues);
    return props;
}

}  // namespace

Publish::Publish()
{
}


const char* Publish::nameImpl() const
{
    static const char* Str = "PUBLISH";
    return Str;
}

const QVariantList& Publish::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace message

}  // namespace protocol

}  // namespace cc_plugin

}  // namespace mqtt

