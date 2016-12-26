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

#include "Subscribe.h"

#include "cc_plugin/protocol/field/PacketId.h"
#include "cc_plugin/protocol/field/QoS.h"

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

QVariantMap createTopicFilterMemberData()
{
    return cc::property::field::String().name("Topic").serialisedHidden().asMap();
}

QVariantMap createRequestQosMemberData()
{
    cc::property::field::EnumValue props;
    props.name("Req. QoS").serialisedHidden();
    field::updateQosPropertiesMap(props);
    return props.asMap();
}

QVariantMap createPayloadBundleData()
{
    return
        cc::property::field::Bundle()
            .add(createTopicFilterMemberData())
            .add(createRequestQosMemberData())
            .asMap();

}

QVariantMap createPayloadProperties()
{
    return
        cc::property::field::ArrayList()
            .name("Payload")
            .add(createPayloadBundleData())
            .asMap();
}

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(field::packetIdProperties());
    props.append(createPayloadProperties());

    assert(props.size() == Subscribe::FieldIdx_numOfValues);
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

}  // namespace protocol

}  // namespace cc_plugin

}  // namespace mqtt

