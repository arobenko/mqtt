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
#include "mqtt/protocol/Message.h"
#include "mqtt/protocol/field.h"

namespace mqtt
{

namespace protocol
{

namespace message
{

using PubackFields = std::tuple<
    field::PacketId
>;

template <typename TMsgBase, template<class> class TActual>
using PubackBase =
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PUBACK>,
        comms::option::FieldsImpl<PubackFields>,
        comms::option::MsgType<TActual<TMsgBase> >
    >;

template <typename TMsgBase = Message>
class Puback : public PubackBase<TMsgBase, Puback>
{
    typedef PubackBase<TMsgBase, mqtt::protocol::message::Puback> Base;
public:
    COMMS_MSG_FIELDS_ACCESS(Base, packetId);

    Puback() = default;
    Puback(const Puback&) = default;
    Puback(Puback&& other) = default;
    virtual ~Puback() = default;

    Puback& operator=(const Puback&) = default;
    Puback& operator=(Puback&&) = default;
};

}  // namespace message

}  // namespace protocol

}  // namespace mqtt
