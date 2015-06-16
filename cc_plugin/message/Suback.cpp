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
    static const QString Name("Return Code");
    QVariantMap map;
    map.insert(cc::Property::name(), Name);
    for (auto idx = 0U; idx <= static_cast<decltype(idx)>(mqtt::message::SubackReturnCode::SuccessQos2); ++idx) {
        static const QString Prefix("Success QoS ");
        auto str = Prefix + QString("%1").arg(idx, 1, 10, QChar('0'));
        map.insert(cc::Property::indexedName(idx), str);
    }

    static const QString FailureStr("Failure");
    map.insert(
        cc::Property::indexedName(static_cast<uint>(mqtt::message::SubackReturnCode::Failure)),
            FailureStr);
    map.insert(cc::Property::serialisedHidden(), true);
    return map;
}

QVariantMap createPayloadProperties()
{
    static const QString Name("Payload");
    QVariantMap props;
    props.insert(cc::Property::name(), Name);
    props.insert(cc::Property::data(), createReturnCodeProperties());
    return props;
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

