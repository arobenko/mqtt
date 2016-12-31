//
// Copyright 2015 (C). Alex Robenko. All rights reserved.
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
#include "mqtt/protocol/message/Connect.h"
#include "mqtt/protocol/message/Connack.h"
#include "mqtt/protocol/message/Publish.h"
#include "mqtt/protocol/message/Puback.h"
#include "mqtt/protocol/message/Pubrec.h"
#include "mqtt/protocol/message/Pubrel.h"
#include "mqtt/protocol/message/Pubcomp.h"
#include "mqtt/protocol/message/Subscribe.h"
#include "mqtt/protocol/message/Suback.h"
#include "mqtt/protocol/message/Unsubscribe.h"
#include "mqtt/protocol/message/Unsuback.h"
#include "mqtt/protocol/message/Pingreq.h"
#include "mqtt/protocol/message/Pingresp.h"
#include "mqtt/protocol/message/Disconnect.h"

namespace mqtt
{

namespace protocol
{

template <typename TMsgBase = Message>
using AllMessages = std::tuple<
    message::Connect<TMsgBase>,
    message::Connack<TMsgBase>,
    message::Publish<TMsgBase>,
    message::Puback<TMsgBase>,
    message::Pubrec<TMsgBase>,
    message::Pubrel<TMsgBase>,
    message::Pubcomp<TMsgBase>,
    message::Subscribe<TMsgBase>,
    message::Suback<TMsgBase>,
    message::Unsubscribe<TMsgBase>,
    message::Unsuback<TMsgBase>,
    message::Pingreq<TMsgBase>,
    message::Pingresp<TMsgBase>,
    message::Disconnect<TMsgBase>
>;

static_assert(
    std::tuple_size<AllMessages<> >::value == (MsgId_NumOfValues - 1),
        "Some messages are missing from the bundle.");

}  // namespace protocol

}  // namespace mqtt




