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

#include "cc_plugin/protocol/common/message/Connack.h"
#include "cc_plugin/protocol/v5/Message.h"
#include "mqtt/protocol/v5/message/Connack.h"

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
{

namespace v5
{

namespace message
{

struct Connack : public
    cc_plugin::protocol::common::message::Connack<
        mqtt::protocol::common::field::ProtocolVersionVal::v5,
        mqtt::protocol::v5::message::Connack<cc_plugin::protocol::v5::Message>,
        Connack
    >
{
};

} // namespace message

} // namespace v5

} // namespace protocol

} // namespace cc_plugin

} // namespace mqtt


