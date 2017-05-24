//
// Copyright 2015 - 2017 (C). Alex Robenko. All rights reserved.
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

#include "mqtt/protocol/v5/field.h"
#include "mqtt/protocol/common/message/Publish.h"

namespace mqtt
{

namespace protocol
{

namespace v5
{

namespace message
{

using PublishFields = std::tuple<
    common::field::PublishFlags,
    common::field::Topic,
    common::field::OptionalPacketId,
    v5::field::Properties,
    common::field::Payload
>;

template <typename TMsgBase>
class Publish : public
        common::message::Publish<
            TMsgBase,
            PublishFields,
            Publish<TMsgBase>
        >
{
public:
    COMMS_MSG_FIELDS_ACCESS(
        publishFlags,
        topic,
        packetId,
        properties,
        payload);

    Publish() = default;
    Publish(const Publish&) = default;
    Publish(Publish&& other) = default;
    ~Publish() = default;

    Publish& operator=(const Publish&) = default;
    Publish& operator=(Publish&&) = default;
};

}  // namespace message

} // namespace v5

}  // namespace protocol

}  // namespace mqtt
