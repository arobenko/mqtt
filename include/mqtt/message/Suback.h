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
#include <algorithm>

#include "mqtt/Message.h"
#include "mqtt/field.h"

namespace mqtt
{

namespace message
{

struct SubackPayloadValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        return 0U < field.value().size();
    }
};

enum class SubackReturnCode : std::uint8_t
{
    SuccessQos0 = 0,
    SuccessQos1 = 1,
    SuccessQos2 = 2,
    Failure = 0x80
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

template <typename TFieldBase>
using SubackPayload =
    comms::field::ArrayList<
        TFieldBase,
        comms::field::EnumValue<
            TFieldBase,
            SubackReturnCode,
            comms::option::ContentsValidator<SubackReturnCodeValidator>
        >,
        comms::option::ContentsValidator<SubackPayloadValidator>
    >;


template <typename TFieldBase>
using SubackFields = std::tuple<
    field::PacketId,
    SubackPayload<TFieldBase>
>;

template <typename TMsgBase = Message>
class Suback : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_SUBACK>,
        comms::option::FieldsImpl<SubackFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Suback<TMsgBase> >
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_SUBACK>,
        comms::option::FieldsImpl<SubackFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Suback<TMsgBase> >
    > Base;
public:

    typedef typename Base::FlagsField FlagsField;

    COMMS_MSG_FIELDS_ACCESS(Base, packetId, payload);

    Suback() = default;
    Suback(const Suback&) = default;
    Suback(Suback&& other) = default;
    virtual ~Suback() = default;

    Suback& operator=(const Suback&) = default;
    Suback& operator=(Suback&&) = default;
};

}  // namespace message


}  // namespace mqtt
