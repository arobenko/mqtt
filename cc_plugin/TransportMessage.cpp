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
    QVariantMap map;
    map.insert(cc::Property::name(), QVariant::fromValue(QString("ID")));

    static const QString Map[] = {
        QString(),
        "CONNECT",
        "CONNACK",
        "PUBLISH",
        "PUBACK",
        "PUBREC",
        "PUBREL",
        "PUBCOMP",
        "SUBSCRIBE",
        "SUBACK"
    };

    static const unsigned MapSize = std::extent<decltype(Map)>::value;

    static_assert(MapSize == mqtt::MsgId_NumOfValues,
        "Map is incorrect.");

    for (auto idx = 1U; idx < MapSize; ++idx) {
        map.insert(cc::Property::indexedName(idx), QVariant::fromValue(Map[idx]));
    }
    return map;
}

QVariantMap createFlagsProperties()
{
    QVariantMap map;
    map.insert(cc::Property::name(), QVariant::fromValue(QString("Flags")));
    return map;
}

QVariantMap createIdAndFlagsProperties()
{
    QVariantMap map;
    map.insert(cc::Property::indexedData(0), createFlagsProperties());
    map.insert(cc::Property::indexedData(1), createMsgIdMemberData());
    return map;
}

QVariantMap createSizeProperties()
{
    QVariantMap map;
    map.insert(cc::Property::name(), QVariant::fromValue(QString("Size")));
    return map;
}

QVariantMap createDataProperties()
{
    QVariantMap map;
    map.insert(cc::Property::name(), QVariant::fromValue(QString("Data")));
    return map;
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

