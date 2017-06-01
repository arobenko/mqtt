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

#include "mqtt/protocol/common/message/Pubrel.h"
#include "mqtt/protocol/v5/field.h"

namespace mqtt
{

namespace protocol
{

namespace v5
{

namespace message
{

using PubrelFields = std::tuple<
    common::field::PacketId,
    v5::field::ResponseCode,
    v5::field::Properties
>;

template <typename TMsgBase>
class Pubrel : public
        common::message::Pubrel<
            TMsgBase,
            PubrelFields,
            Pubrel<TMsgBase>
        >
{
public:
    COMMS_MSG_FIELDS_ACCESS(packetId, responseCode, properties);

    Pubrel() = default;
    Pubrel(const Pubrel&) = default;
    Pubrel(Pubrel&& other) = default;
    ~Pubrel() = default;

    Pubrel& operator=(const Pubrel&) = default;
    Pubrel& operator=(Pubrel&&) = default;
};

} // namespace message

} // namespace v5

} // namespace protocol

} // namespace mqtt
