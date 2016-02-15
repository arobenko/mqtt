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

#include "Suback.h"

#include "cc_plugin/field/PacketId.h"
#include "cc_plugin/field/QoS.h"

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

namespace message
{

namespace
{

QVariantMap createReturnCodeProperties()
{
    QVariantList enumValues;
    for (auto idx = 0; idx <= static_cast<decltype(idx)>(mqtt::message::SubackReturnCode::SuccessQos2); ++idx) {
        static const QString Prefix("Success QoS ");
        auto str = Prefix + QString("%1").arg(idx, 1, 10, QChar('0'));
        cc::Property::appendEnumValue(enumValues, str, idx);
    }
    cc::Property::appendEnumValue(enumValues, "Failure", (int)mqtt::message::SubackReturnCode::Failure);
    return cc::Property::createPropertiesMap("Return Code", std::move(enumValues));
}

QVariantMap createPayloadProperties()
{
    return cc::Property::createPropertiesMap("Payload", createReturnCodeProperties());
}

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(cc_plugin::field::packetIdProperties());
    props.append(createPayloadProperties());

    assert(props.size() == Suback::FieldIdx_NumOfValues);
    return props;
}

}  // namespace

const char* Suback::nameImpl() const
{
    static const char* Str = "SUBACK";
    return Str;
}

const QVariantList& Suback::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace message

}  // namespace cc_plugin

}  // namespace mqtt

