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

using PubrelFields = std::tuple<
    field::PacketId
>;

template <typename TMsgBase = Message>
class Pubrel : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PUBREL>,
        comms::option::FieldsImpl<PubrelFields>,
        comms::option::MsgType<Pubrel<TMsgBase> >,
        comms::option::DispatchImpl
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PUBREL>,
        comms::option::FieldsImpl<PubrelFields>,
        comms::option::MsgType<Pubrel<TMsgBase> >,
        comms::option::DispatchImpl
    > Base;
public:

    COMMS_MSG_FIELDS_ACCESS(Base, packetId);

    Pubrel()
    {
        typename Base::FlagsField flags;
        flags.value() = 2;
        Base::setFlags(flags);
    }

    Pubrel(const Pubrel&) = default;
    Pubrel(Pubrel&& other) = default;
    virtual ~Pubrel() = default;

    Pubrel& operator=(const Pubrel&) = default;
    Pubrel& operator=(Pubrel&&) = default;
};

}  // namespace message

}  // namespace protocol

}  // namespace mqtt
