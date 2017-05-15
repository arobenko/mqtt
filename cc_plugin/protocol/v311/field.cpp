//
// Copyright 2016 (C). Alex Robenko. All rights reserved.
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

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
{

namespace v311
{

namespace field
{

namespace
{

QVariantMap createPacketIdProperties()
{
    return cc::property::field::IntValue().name(packetIdFieldName()).asMap();
}

QVariantMap createOptionalPacketIdProperties()
{
    return cc::property::field::Optional()
        .name(packetIdFieldName())
        .field(createPacketIdProperties())
        .uncheckable()
        .asMap();
}

}  // namespace

const QString& packetIdFieldName()
{
    static const QString Str("Packet ID");
    return Str;
}

const QVariantMap& packetIdProperties()
{
    static const auto Props = createPacketIdProperties();
    return Props;
}

const QVariantMap& optionalPacketIdProperties()
{
    static const auto Props = createOptionalPacketIdProperties();
    return Props;
}

void updateQosPropertiesMap(cc::property::field::EnumValue& props)
{
    props
        .add("At most once delivery")
        .add("At least once delivery")
        .add("Exactly once delivery");
}

}  // namespace field

} // namespace v311

}  // namespace protocol

}  // namespace cc_plugin

}  // namespace mqtt



