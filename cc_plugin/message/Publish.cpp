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

#include <QtCore/QVariantMap>

#include "cc_plugin/field/QoS.h"
#include "cc_plugin/field/PacketId.h"
#include "Publish.h"

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

QVariantMap createRetainMemberData()
{
    QVariantMap map;
    static const QString Name("Flags");
    map.insert(cc::Property::name(), QVariant::fromValue(Name));
    map.insert(cc::Property::serialisedHidden(), QVariant::fromValue(true));

    static const QString Map[] = {
        "RETAIN"
    };

    static const unsigned MapSize = std::extent<decltype(Map)>::value;

    for (auto idx = 0U; idx < MapSize; ++idx) {
        map.insert(cc::Property::indexedName(idx), QVariant::fromValue(Map[idx]));
    }
    return map;
}

QVariantMap createQosMemberData()
{
    QVariantMap map;
    static const QString Name("QoS");
    map.insert(cc::Property::name(), QVariant::fromValue(Name));
    map.insert(cc::Property::serialisedHidden(), QVariant::fromValue(true));
    field::updateQosPropertiesMap(map);
    return map;
}

QVariantMap createDupMemberData()
{
    QVariantMap map;
    map.insert(cc::Property::name(), QVariant::fromValue(getEmptyString()));
    map.insert(cc::Property::serialisedHidden(), QVariant::fromValue(true));

    static const QString Map[] = {
        "DUP"
    };

    static const unsigned MapSize = std::extent<decltype(Map)>::value;

    for (auto idx = 0U; idx < MapSize; ++idx) {
        map.insert(cc::Property::indexedName(idx), QVariant::fromValue(Map[idx]));
    }
    return map;
}

QVariantMap getReservedMemberData()
{
    QVariantMap map;
    map.insert(cc::Property::name(), QVariant::fromValue(getEmptyString()));
    map.insert(cc::Property::fieldHidden(), QVariant::fromValue(true));
    return map;
}

QVariantMap createFlagsProperties()
{
    static const QString Str("Flags");
    QVariantMap props;
    props.insert(cc::Property::name(), Str);
    props.insert(cc::Property::serialisedHidden(), true);
    props.insert(
        cc::Property::indexedData(mqtt::message::PublishActualFlagIdx_Retain),
        createRetainMemberData());
    props.insert(
        cc::Property::indexedData(mqtt::message::PublishActualFlagIdx_QoS),
        createQosMemberData());
    props.insert(
        cc::Property::indexedData(mqtt::message::PublishActualFlagIdx_Dup),
        createDupMemberData());
    props.insert(
        cc::Property::indexedData(mqtt::message::PublishActualFlagIdx_Reserved),
        getReservedMemberData());
    return props;
}

QVariantMap createTopicProperties()
{
    static const QString Str("Topic");
    QVariantMap props;
    props.insert(cc::Property::name(), Str);
    return props;
}

QVariantMap createPayloadProperties()
{
    static const QString Str("Payload");
    QVariantMap props;
    props.insert(cc::Property::name(), Str);
    return props;
}

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(createFlagsProperties());
    props.append(createTopicProperties());
    props.append(cc_plugin::field::optionalPacketIdProperties());
    props.append(createPayloadProperties());

    assert(props.size() == Publish::FieldIdx_NumOfValues);
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

}  // namespace cc_plugin

}  // namespace mqtt

