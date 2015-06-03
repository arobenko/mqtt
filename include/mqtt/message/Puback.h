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
#include "mqtt/field/PacketId.h"

namespace mqtt
{

namespace message
{

template <typename TFieldBase>
using PubackFields = std::tuple<
    mqtt::field::PacketId<TFieldBase>
>;

template <typename TMsgBase = Message>
class Puback : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PUBACK>,
        comms::option::FieldsImpl<PubackFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Puback<TMsgBase> >
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PUBACK>,
        comms::option::FieldsImpl<PubackFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Puback<TMsgBase> >
    > Base;
public:
    enum FieldIdx
    {
        FieldIdx_PacketId,
        FieldIdx_NumOfValues
    };

    static_assert(std::tuple_size<typename Base::AllFields>::value == FieldIdx_NumOfValues,
        "Number of fields is incorrect");

    Puback() = default;
    Puback(const Puback&) = default;
    Puback(Puback&& other)
    {
    }
    virtual ~Puback() = default;

    Puback& operator=(const Puback&) = default;
    Puback& operator=(Puback&&) = default;
};

}  // namespace message


}  // namespace mqtt
