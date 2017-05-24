//
// Copyright 2016 - 2017 (C). Alex Robenko. All rights reserved.
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

namespace v311
{

namespace field
{

using FieldBase = common::field::FieldBase;

using ProtocolVersionVal = mqtt::protocol::common::field::ProtocolVersionVal;

using ProtocolVersion =
    mqtt::protocol::common::field::ProtocolVersion<ProtocolVersionVal::v311>;


enum class SubackReturnCode : std::uint8_t
{
    SuccessQos0 = 0,
    SuccessQos1 = 1,
    SuccessQos2 = 2,
    Failure = 0x80
};

namespace details
{

struct SubackPayloadValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        return 0U < field.value().size();
    }
};

struct SubackReturnCodeValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        auto value = field.value();
        return
            (value == SubackReturnCode::SuccessQos0) ||
            (value == SubackReturnCode::SuccessQos1) ||
            (value == SubackReturnCode::SuccessQos2) ||
            (value == SubackReturnCode::Failure);
    }
};

struct SubscribePayloadValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        return 0U < field.value().size();
    }
};


}  // namespace details

enum class ConnackResponseCodeVal : std::uint8_t
{
    Accepted,
    WrongProtocolVersion,
    IdentifierRejected,
    ServerUnavailable,
    BadUsernameOrPassword,
    NotAuthorized,
    NumOfValues
};

using ConnackResponseCode = comms::field::EnumValue<
        FieldBase,
        ConnackResponseCodeVal,
        comms::option::ValidNumValueRange<0, (int)(ConnackResponseCodeVal::NumOfValues) - 1>
    >;

using SubackPayload =
    comms::field::ArrayList<
        FieldBase,
        comms::field::EnumValue<
            FieldBase,
            SubackReturnCode,
            comms::option::ContentsValidator<details::SubackReturnCodeValidator>
        >,
        comms::option::ContentsValidator<details::SubackPayloadValidator>
    >;

class SubElem : public
    comms::field::Bundle<
        FieldBase,
        std::tuple<
            common::field::Topic,
            common::field::QoS<>
        >
    >
{
public:
    COMMS_FIELD_MEMBERS_ACCESS(topic, qos);
};

using SubscribePayload =
    comms::field::ArrayList<
        FieldBase,
        SubElem,
        comms::option::ContentsValidator<details::SubscribePayloadValidator>
    >;

}  // namespace field

} // namespace v311

}  // namespace protocol

}  // namespace mqtt


