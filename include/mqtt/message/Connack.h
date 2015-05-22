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


#pragma once

#include <tuple>
#include "mqtt/Message.h"

namespace mqtt
{

namespace message
{

enum class ConnackResponseCode : std::uint8_t
{
    Accepted,
    WrongProtocolVersion,
    IdentifierRejected,
    ServerUnavailable,
    BadUsernameOrPassword,
    NotAuthorized,
    NumOfValues
};

template <typename TFieldBase>
using ConnackFields = std::tuple<
    comms::field::BitmaskValue<
        TFieldBase,
        comms::option::FixedLength<1>,
        comms::option::BitmaskReservedBits<0xfe, 0x0>
    >,
    comms::field::EnumValue<
        TFieldBase,
        ConnackResponseCode,
        comms::option::ValidNumValueRange<0, (int)(ConnackResponseCode::NumOfValues) - 1>
    >
>;

template <typename TMsgBase = Message>
class Connack : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_CONNACK>,
        comms::option::FieldsImpl<ConnackFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Connack<TMsgBase> >
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_CONNACK>,
        comms::option::FieldsImpl<ConnackFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Connack<TMsgBase> >
    > Base;
public:
    enum FieldIdx
    {
        FieldIdx_Flags,
        FieldIdx_Response,
        FieldIdx_NumOfValues
    };

    static_assert(std::tuple_size<typename Base::AllFields>::value == FieldIdx_NumOfValues,
        "Number of fields is incorrect");

    Connack() = default;
    Connack(const Connack&) = default;
    Connack(Connack&& other)
    {
    }
    virtual ~Connack() = default;

    Connack& operator=(const Connack&) = default;
    Connack& operator=(Connack&&) = default;
};

}  // namespace message


}  // namespace mqtt
