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
#include "cc_plugin/protocol/v5/Message.h"

#include "cc_plugin/protocol/v5/message/Connect.h"
#include "cc_plugin/protocol/v5/message/Connack.h"
//#include "cc_plugin/protocol/v5/message/Publish.h"
//#include "cc_plugin/protocol/v5/message/Puback.h"
//#include "cc_plugin/protocol/v5/message/Pubrec.h"
//#include "cc_plugin/protocol/v5/message/Pubrel.h"
//#include "cc_plugin/protocol/v5/message/Pubcomp.h"
//#include "cc_plugin/protocol/v5/message/Subscribe.h"
//#include "cc_plugin/protocol/v5/message/Suback.h"
//#include "cc_plugin/protocol/v5/message/Unsubscribe.h"
//#include "cc_plugin/protocol/v5/message/Unsuback.h"
#include "cc_plugin/protocol/v5/message/Pingreq.h"
#include "cc_plugin/protocol/v5/message/Pingresp.h"
//#include "cc_plugin/protocol/v5/message/Disconnect.h"

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
{

namespace v5
{

typedef std::tuple<
    cc_plugin::protocol::v5::message::Connect,
    cc_plugin::protocol::v5::message::Connack,
//    cc_plugin::protocol::v5::message::Publish,
//    cc_plugin::protocol::v5::message::Puback,
//    cc_plugin::protocol::v5::message::Pubrec,
//    cc_plugin::protocol::v5::message::Pubrel,
//    cc_plugin::protocol::v5::message::Pubcomp,
//    cc_plugin::protocol::v5::message::Subscribe,
//    cc_plugin::protocol::v5::message::Suback,
//    cc_plugin::protocol::v5::message::Unsubscribe,
//    cc_plugin::protocol::v5::message::Unsuback,
    cc_plugin::protocol::v5::message::Pingreq,
    cc_plugin::protocol::v5::message::Pingresp//,
//    cc_plugin::protocol::v5::message::Disconnect
> AllMessages;

//static_assert(
//    std::tuple_size<AllMessages>::value == (mqtt::protocol::v5::MsgId_NumOfValues - 1),
//        "Some messages are missing from the bundle.");

} // namespace v5

}  // namespace protocol

}  // namespace cc_plugin

}  // namespace mqtt




