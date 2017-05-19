//
// Copyright 2017 (C). Alex Robenko. All rights reserved.
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

#pragma once

#include "mqtt/protocol/common/field.h"

namespace mqtt
{

namespace protocol
{

namespace v5
{

namespace field
{

using FieldBase = mqtt::protocol::common::field::FieldBase;

using ProtocolVersionVal = mqtt::protocol::common::field::ProtocolVersionVal;

template <typename... TOpts>
using ProtocolName = mqtt::protocol::common::field::ProtocolName<TOpts...>;

using ProtocolVersion =
    mqtt::protocol::common::field::ProtocolVersion<ProtocolVersionVal::v5>;

using ConnectFlags = mqtt::protocol::common::field::ConnectFlags;

using KeepAlive = mqtt::protocol::common::field::KeepAlive;

enum class PropertyIdVal : std::uint8_t
{
    PayloadFormatIndicator = 1,
    PublishExpiryInterval = 2
};

template <PropertyIdVal TVal>
using PropertyId =
    comms::field::EnumValue<
        FieldBase,
        PropertyIdVal,
        comms::option::DefaultNumValue<(int)TVal>,
        comms::option::ValidNumValueRange<(int)TVal, (int)TVal>,
        comms::option::FailOnInvalid<>
    >;

template <PropertyIdVal TId, typename TValueField>
struct Property : public
        comms::field::Bundle<
            FieldBase,
            std::tuple<
                PropertyId<TId>,
                TValueField
            >
        >
{
    COMMS_FIELD_MEMBERS_ACCESS(id, value);
};


enum class PayloadFormatIndicatorVal : std::uint8_t
{
    Unspecified,
    Utf8CharData,
    NumOfValues
};

using PayloadFormatIndicator =
    comms::field::EnumValue<
        FieldBase,
        PayloadFormatIndicatorVal,
        comms::option::ValidNumValueRange<0, (int)PayloadFormatIndicatorVal::NumOfValues - 1>
    >;

using PayloadFormatIndicatorProp =
    Property<PropertyIdVal::PayloadFormatIndicator, PayloadFormatIndicator>;

using PublishExpiryInterval =
    comms::field::IntValue<
        FieldBase,
        std::uint32_t,
        comms::option::UnitsSeconds
    >;

using PublishExpiryIntervalProp =
    Property<PropertyIdVal::PublishExpiryInterval, PublishExpiryInterval>;

struct PropertyVar : public
    comms::field::Variant<
        FieldBase,
        std::tuple<
            PayloadFormatIndicatorProp,
            PublishExpiryIntervalProp
        >
    >
{
    COMMS_VARIANT_MEMBERS_ACCESS(
        payloadFormatIndicator,
        publishExpiryInterval
    );
};

template <typename... TOpt>
using Properties =
    comms::field::ArrayList<
        FieldBase,
        PropertyVar,
        comms::option::SequenceSerLengthFieldPrefix<
            mqtt::protocol::common::field::VarByteInt
        >,
        TOpt...
    >;

template <typename... TOpts>
using ClientId = mqtt::protocol::common::field::ClientId<TOpts...>;

template <typename... TOpts>
using WillTopic = mqtt::protocol::common::field::WillTopic<TOpts...>;

template <typename... TOpts>
using WillMessage = mqtt::protocol::common::field::WillMessage<TOpts...>;

template <typename... TOpts>
using UserName = mqtt::protocol::common::field::UserName<TOpts...>;

template <typename... TOpts>
using Password = mqtt::protocol::common::field::Password<TOpts...>;

} // namespace field

} // namespace v5

}  // namespace protocol

}  // namespace mqtt
