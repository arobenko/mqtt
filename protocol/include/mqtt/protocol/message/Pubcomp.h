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

using PubcompFields = std::tuple<
    field::PacketId
>;

template <typename TMsgBase = Message>
class Pubcomp : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PUBCOMP>,
        comms::option::FieldsImpl<PubcompFields>,
        comms::option::DispatchImpl<Pubcomp<TMsgBase> >
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PUBCOMP>,
        comms::option::FieldsImpl<PubcompFields>,
        comms::option::DispatchImpl<Pubcomp<TMsgBase> >
    > Base;
public:
    COMMS_MSG_FIELDS_ACCESS(Base, packetId);

    Pubcomp() = default;
    Pubcomp(const Pubcomp&) = default;
    Pubcomp(Pubcomp&& other) = default;
    virtual ~Pubcomp() = default;

    Pubcomp& operator=(const Pubcomp&) = default;
    Pubcomp& operator=(Pubcomp&&) = default;
};

}  // namespace message

}  // namespace protocol

}  // namespace mqtt
