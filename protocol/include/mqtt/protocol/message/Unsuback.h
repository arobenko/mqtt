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


#pragma once

#include <tuple>
#include <algorithm>

#include "mqtt/protocol/Message.h"
#include "mqtt/protocol/field.h"

namespace mqtt
{

namespace protocol
{

namespace message
{

using UnsubackFields = std::tuple<
    field::PacketId
>;

template <typename TMsgBase, template<class> class TActual>
using UnsubackBase =
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_UNSUBACK>,
        comms::option::FieldsImpl<UnsubackFields>,
        comms::option::MsgType<TActual<TMsgBase> >
    >;

template <typename TMsgBase = Message>
class Unsuback : public UnsubackBase<TMsgBase, Unsuback>
{
    typedef UnsubackBase<TMsgBase, mqtt::protocol::message::Unsuback> Base;
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

}  // namespace protocol

}  // namespace mqtt
