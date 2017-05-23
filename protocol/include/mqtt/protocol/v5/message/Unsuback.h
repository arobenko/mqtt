//
// Copyright 2017 (C). Alex Robenko. All rights reserved.
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
#include "comms/MessageBase.h"
#include "mqtt/protocol/common/MsgId.h"
#include "mqtt/protocol/v5/field.h"

namespace mqtt
{

namespace protocol
{

namespace v5
{

namespace message
{

using UnsubackFields = std::tuple<
    common::field::PacketId,
    v5::field::Properties,
    v5::field::ResponseCodeList
>;

template <typename TMsgBase>
class Unsuback : public
        comms::MessageBase<
            TMsgBase,
            comms::option::StaticNumIdImpl<common::MsgId_UNSUBACK>,
            comms::option::FieldsImpl<UnsubackFields>,
            comms::option::MsgType<Unsuback<TMsgBase> >
        >
{
public:
    COMMS_MSG_FIELDS_ACCESS(packetId, properties, payload);

    Unsuback() = default;
    Unsuback(const Unsuback&) = default;
    Unsuback(Unsuback&& other) = default;
    virtual ~Unsuback() = default;

    Unsuback& operator=(const Unsuback&) = default;
    Unsuback& operator=(Unsuback&&) = default;
};

} // namespace message

} // namespace v5

} // namespace protocol

} // namespace mqtt
