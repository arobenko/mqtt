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
#include "cc_plugin/Message.h"

#include "cc_plugin/message/Connect.h"
#include "cc_plugin/message/Connack.h"
#include "cc_plugin/message/Publish.h"
#include "cc_plugin/message/Puback.h"
#include "cc_plugin/message/Pubrec.h"
#include "cc_plugin/message/Pubrel.h"
#include "cc_plugin/message/Pubcomp.h"
#include "cc_plugin/message/Subscribe.h"
#include "cc_plugin/message/Suback.h"

namespace mqtt
{

namespace cc_plugin
{

typedef std::tuple<
    cc_plugin::message::Connect,
    cc_plugin::message::Connack,
    cc_plugin::message::Publish,
    cc_plugin::message::Puback,
    cc_plugin::message::Pubrec,
    cc_plugin::message::Pubrel,
    cc_plugin::message::Pubcomp,
    cc_plugin::message::Subscribe,
    cc_plugin::message::Suback
> AllMessages;

static_assert(
    std::tuple_size<AllMessages>::value == (mqtt::MsgId_NumOfValues - 1),
        "Some messages are missing from the bundle.");

}  // namespace cc_plugin

}  // namespace mqtt




