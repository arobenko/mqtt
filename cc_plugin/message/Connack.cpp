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
    return
        cc::property::field::BitmaskValue()
            .name("Connect Acknowledge Flags")
            .add("Session Present")
            .asMap();
}

QVariantMap createResponseProperties()
{
    return
        cc::property::field::EnumValue()
            .name("Response")
            .add("Accepted")
            .add("Wrong Protocol Version")
            .add("Identifier Rejected")
            .add("Server Unavailable")
            .add("Bad Username or Password")
            .add("Not Authorized")
            .asMap();
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

