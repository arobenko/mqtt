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
#include "mqtt/field/PacketId.h"

namespace mqtt
{

namespace message
{

template <typename TFieldBase>
using UnsubackPacketIdField =
        mqtt::field::PacketId<TFieldBase>;

template <typename TFieldBase>
using UnsubackFields = std::tuple<
    UnsubackPacketIdField<TFieldBase>
>;

template <typename TMsgBase = Message>
class Unsuback : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_UNSUBACK>,
        comms::option::FieldsImpl<UnsubackFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Unsuback<TMsgBase> >
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_UNSUBACK>,
        comms::option::FieldsImpl<UnsubackFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Unsuback<TMsgBase> >
    > Base;
public:

    typedef typename Base::FlagsField FlagsField;

    COMMS_MSG_FIELDS_ACCESS(Base, packetId);

    Unsuback() = default;
    Unsuback(const Unsuback&) = default;
    Unsuback(Unsuback&& other) = default;
    virtual ~Unsuback() = default;

    Unsuback& operator=(const Unsuback&) = default;
    Unsuback& operator=(Unsuback&&) = default;
};

}  // namespace message


}  // namespace mqtt
