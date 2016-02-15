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
    QVariantList bitNames;
    bitNames.append("RETAIN");

    auto props = cc::Property::createPropertiesMap("Flags", std::move(bitNames));
    cc::Property::setSerialisedHidden(props);
    return props;
}

QVariantMap createQosMemberData()
{
    static const QString Name("QoS");
    auto props = cc::Property::createPropertiesMap(Name);
    cc::Property::setSerialisedHidden(props);
    field::updateQosPropertiesMap(props);
    return props;
}

QVariantMap createDupMemberData()
{
    QVariantList bitNames;
    bitNames.append("DUP");

    auto props = cc::Property::createPropertiesMap(getEmptyString(), std::move(bitNames));
    cc::Property::setSerialisedHidden(props);
    return props;
}

QVariantMap getReservedMemberData()
{
    auto props = cc::Property::createPropertiesMap(getEmptyString());
    cc::Property::setFieldHidden(props);
    return props;
}

QVariantMap createFlagsProperties()
{
    QVariantList memberData;
    memberData.append(createRetainMemberData());
    memberData.append(createQosMemberData());
    memberData.append(createDupMemberData());
    memberData.append(getReservedMemberData());

    auto props = cc::Property::createPropertiesMap("Flags", std::move(memberData));
    cc::Property::setSerialisedHidden(props);
    return props;
}

QVariantMap createTopicProperties()
{
    return cc::Property::createPropertiesMap("Topic");
}

QVariantMap createPayloadProperties()
{
    return cc::Property::createPropertiesMap("Payload");
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

