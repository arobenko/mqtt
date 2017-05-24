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

#include "comms/MessageBase.h"
#include "mqtt/protocol/v311/field.h"
#include "mqtt/protocol/common/message/Connect.h"

namespace mqtt
{

namespace protocol
{

namespace v311
{

namespace message
{

using ConnectFields = std::tuple<
    common::field::ProtocolName,
    v311::field::ProtocolVersion,
    common::field::ConnectFlags,
    common::field::KeepAlive,
    common::field::ClientId,
    common::field::WillTopic,
    common::field::WillMessage,
    common::field::UserName,
    common::field::Password
>;

template <typename TMsgBase>
class Connect : public
        common::message::Connect<
            TMsgBase,
            ConnectFields,
            Connect<TMsgBase>
        >
{
public:
    COMMS_MSG_FIELDS_ACCESS(
        name,
        propVersion,
        flags,
        keepAlive,
        clientId,
        willTopic,
        willMessage,
        userName,
        password);

    Connect() = default;
    Connect(const Connect&) = default;
    Connect(Connect&& other) = default;
    ~Connect() = default;

    Connect& operator=(const Connect&) = default;
    Connect& operator=(Connect&&) = default;
};

}  // namespace message

} // namespace v311

}  // namespace protocol

}  // namespace mqtt
