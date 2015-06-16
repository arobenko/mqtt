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
#include "mqtt/Message.h"
#include "mqtt/message/Connect.h"
#include "mqtt/message/Connack.h"
#include "mqtt/message/Publish.h"
#include "mqtt/message/Puback.h"
#include "mqtt/message/Pubrec.h"
#include "mqtt/message/Pubrel.h"
#include "mqtt/message/Pubcomp.h"
#include "mqtt/message/Subscribe.h"
#include "mqtt/message/Suback.h"
#include "mqtt/message/Unsubscribe.h"
#include "mqtt/message/Unsuback.h"
#include "mqtt/message/Pingreq.h"

namespace mqtt
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
    message::Pingreq<TMsgBase>
>;

static_assert(
    std::tuple_size<AllMessages<> >::value == (mqtt::MsgId_NumOfValues - 1),
        "Some messages are missing from the bundle.");


}  // namespace mqtt




