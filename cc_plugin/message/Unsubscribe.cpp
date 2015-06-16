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

#include "Unsubscribe.h"

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

QVariantMap createTopicFilterMemberData()
{
    QVariantMap map;
    static const QString Name("Topic");
    map.insert(cc::Property::name(), QVariant::fromValue(Name));
    map.insert(cc::Property::serialisedHidden(), true);
    return map;
}

QVariantMap createPayloadProperties()
{
    static const QString Name("Payload");
    QVariantMap props;
    props.insert(cc::Property::name(), Name);
    props.insert(cc::Property::data(), createTopicFilterMemberData());
    return props;
}

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(cc_plugin::field::packetIdProperties());
    props.append(createPayloadProperties());

    assert(props.size() == Unsubscribe::FieldIdx_NumOfValues);
    return props;
}

}  // namespace

const char* Unsubscribe::nameImpl() const
{
    static const char* Str = "UNSUBSCRIBE";
    return Str;
}

const QVariantList& Unsubscribe::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace message

}  // namespace cc_plugin

}  // namespace mqtt

