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

#include "Subscribe.h"

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
    auto props = cc::Property::createPropertiesMap("Topic");
    cc::Property::setSerialisedHidden(props);
    return props;
}

QVariantMap createRequestQosMemberData()
{
    auto props = cc::Property::createPropertiesMap("Req. QoS");
    cc::Property::setSerialisedHidden(props);
    field::updateQosPropertiesMap(props);
    return props;
}

QVariantMap createPayloadBundleData()
{
    QVariantList memberData;
    memberData.append(createTopicFilterMemberData());
    memberData.append(createRequestQosMemberData());

    QVariantMap props;
    cc::Property::setData(props, std::move(memberData));
    return props;
}

QVariantMap createPayloadProperties()
{
    return cc::Property::createPropertiesMap("Payload", createPayloadBundleData());
}

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(cc_plugin::field::packetIdProperties());
    props.append(createPayloadProperties());

    assert(props.size() == Subscribe::FieldIdx_NumOfValues);
    return props;
}

}  // namespace

const char* Subscribe::nameImpl() const
{
    static const char* Str = "SUBSCRIBE";
    return Str;
}

const QVariantList& Subscribe::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace message

}  // namespace cc_plugin

}  // namespace mqtt

