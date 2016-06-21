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
#include "cc_plugin/protocol/Message.h"

#include "cc_plugin/protocol/message/Connect.h"
#include "cc_plugin/protocol/message/Connack.h"
#include "cc_plugin/protocol/message/Publish.h"
#include "cc_plugin/protocol/message/Puback.h"
#include "cc_plugin/protocol/message/Pubrec.h"
#include "cc_plugin/protocol/message/Pubrel.h"
#include "cc_plugin/protocol/message/Pubcomp.h"
#include "cc_plugin/protocol/message/Subscribe.h"
#include "cc_plugin/protocol/message/Suback.h"
#include "cc_plugin/protocol/message/Unsubscribe.h"
#include "cc_plugin/protocol/message/Unsuback.h"
#include "cc_plugin/protocol/message/Pingreq.h"
#include "cc_plugin/protocol/message/Pingresp.h"
#include "cc_plugin/protocol/message/Disconnect.h"

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
{

typedef std::tuple<
    cc_plugin::protocol::message::Connect,
    cc_plugin::protocol::message::Connack,
    cc_plugin::protocol::message::Publish,
    cc_plugin::protocol::message::Puback,
    cc_plugin::protocol::message::Pubrec,
    cc_plugin::protocol::message::Pubrel,
    cc_plugin::protocol::message::Pubcomp,
    cc_plugin::protocol::message::Subscribe,
    cc_plugin::protocol::message::Suback,
    cc_plugin::protocol::message::Unsubscribe,
    cc_plugin::protocol::message::Unsuback,
    cc_plugin::protocol::message::Pingreq,
    cc_plugin::protocol::message::Pingresp,
    cc_plugin::protocol::message::Disconnect
> AllMessages;

static_assert(
    std::tuple_size<AllMessages>::value == (mqtt::MsgId_NumOfValues - 1),
        "Some messages are missing from the bundle.");

}  // namespace protocol

}  // namespace cc_plugin

}  // namespace mqtt




