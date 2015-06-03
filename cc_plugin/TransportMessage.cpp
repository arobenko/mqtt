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
#include "mqtt/MsgId.h"

#include <QtCore/QVariantMap>

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

QVariantMap getMsgIdMemberData()
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
        "PUBREL"
    };

    static const unsigned MapSize = std::extent<decltype(Map)>::value;

    static_assert(MapSize == mqtt::MsgId_NumOfValues,
        "Map is incorrect.");

    for (auto idx = 1U; idx < MapSize; ++idx) {
        map.insert(cc::Property::indexedName(idx), QVariant::fromValue(Map[idx]));
    }
    return map;
}

QVariantMap getFlagsMemberData()
{
    QVariantMap map;
    map.insert(cc::Property::name(), QVariant::fromValue(QString("Flags")));
    return map;
}

}  // namespace

void TransportMessage::updateFieldPropertiesImpl(QWidget& fieldWidget, uint idx) const
{
    static const char* FieldNames[] = {
        "ID",
        "Size",
        "Data"
    };

    static const std::size_t NumOfFields =
        std::tuple_size<cc_plugin::Stack::AllFields>::value;

    static_assert(
        std::extent<decltype(FieldNames)>::value == NumOfFields,
        "FieldNames array must be updated.");

    if (NumOfFields <= idx) {
        return;
    }

    cc::Property::setNameVal(fieldWidget, FieldNames[idx]);

    if (idx == FieldIdx_IdAndFlags) {
        cc::Property::setIndexedDataVal(fieldWidget, 0, getFlagsMemberData());
        cc::Property::setIndexedDataVal(fieldWidget, 1, getMsgIdMemberData());
    }
}

}  // namespace cc_plugin

}  // namespace mqtt

