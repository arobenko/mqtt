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

#include "Connack.h"

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

namespace message
{

namespace
{

QVariantMap createConnectAcknowledgeFlagsProperties()
{
    QVariantMap props;
    props.insert(cc::Property::name(), "Connect Acknowledge Flags");

    static const QString BitNames[] = {
        "Session Present"
    };

    static const unsigned BitNamesCount = std::extent<decltype(BitNames)>::value;

    for (auto idx = 0U; idx < BitNamesCount; ++idx) {
        props.insert(cc::Property::indexedName(idx), BitNames[idx]);
    }

    return props;
}

QVariantMap createResponseProperties()
{
    QVariantMap props;
    props.insert(cc::Property::name(), "Response");

    static const QString ValueNames[] = {
        "Accepted",
        "Wrong Protocol Version",
        "Identifier Rejected",
        "Server Unavailable",
        "Bad Username or Password",
        "Not Authorized"
    };

    static const unsigned ValueNamesCount = std::extent<decltype(ValueNames)>::value;

    static_assert(ValueNamesCount == (unsigned)mqtt::message::ConnackResponseCode::NumOfValues,
        "Invalid map.");

    for (auto idx = 0U; idx < ValueNamesCount; ++idx) {
        props.insert(cc::Property::indexedName(idx), ValueNames[idx]);
    }
    return props;
}

QVariantList createFieldsProperties()
{
    QVariantList props;
    props.append(createConnectAcknowledgeFlagsProperties());
    props.append(createResponseProperties());

    assert(props.size() == Connack::FieldIdx_NumOfValues);
    return props;
}

}  // namespace

const char* Connack::nameImpl() const
{
    static const char* Str = "CONNACK";
    return Str;
}

const QVariantList& Connack::fieldsPropertiesImpl() const
{
    static const auto Props = createFieldsProperties();
    return Props;
}

}  // namespace message

}  // namespace cc_plugin

}  // namespace mqtt

