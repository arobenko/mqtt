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
#include "mqtt/protocol/v311/Message.h"
#include "mqtt/protocol/v311/field.h"

namespace mqtt
{

namespace protocol
{

namespace v311
{

namespace message
{


using PubrecFields = std::tuple<
    field::PacketId
>;

template <typename TMsgBase = Message>
class Pubrec : public
        comms::MessageBase<
            TMsgBase,
            comms::option::StaticNumIdImpl<MsgId_PUBREC>,
            comms::option::FieldsImpl<PubrecFields>,
            comms::option::MsgType<Pubrec<TMsgBase> >
        >
{
public:
    COMMS_MSG_FIELDS_ACCESS(packetId);

    Pubrec() = default;
    Pubrec(const Pubrec&) = default;
    Pubrec(Pubrec&& other) = default;
    virtual ~Pubrec() = default;

    Pubrec& operator=(const Pubrec&) = default;
    Pubrec& operator=(Pubrec&&) = default;
};

}  // namespace message

} // namespace v311

}  // namespace protocol

}  // namespace mqtt
