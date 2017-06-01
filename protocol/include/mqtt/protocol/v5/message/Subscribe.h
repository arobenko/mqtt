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
#include "mqtt/protocol/v5/field.h"
#include "mqtt/protocol/common/message/Subscribe.h"

namespace mqtt
{

namespace protocol
{

namespace v5
{

namespace message
{

using SubscribeFields = std::tuple<
    common::field::PacketId,
    v5::field::Properties,
    v5::field::SubscribePayload
>;

template <typename TMsgBase>
class Subscribe : public
        common::message::Subscribe<
            TMsgBase,
            SubscribeFields,
            Subscribe<TMsgBase>
        >
{
public:
    COMMS_MSG_FIELDS_ACCESS(packetId, properties, payload);

    Subscribe() = default;
    Subscribe(const Subscribe&) = default;
    Subscribe(Subscribe&& other) = default;
    ~Subscribe() = default;

    Subscribe& operator=(const Subscribe&) = default;
    Subscribe& operator=(Subscribe&&) = default;
};

}  // namespace message

} // namespace v5

}  // namespace protocol

}  // namespace mqtt
