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

#include "cc_plugin/protocol/common/message/Pubrel.h"
#include "cc_plugin/protocol/v311/Message.h"
#include "mqtt/protocol/v311/message/Pubrel.h"

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
{

namespace v311
{

namespace message
{

struct Pubrel : public
    cc_plugin::protocol::common::message::Pubrel<
        mqtt::protocol::common::field::ProtocolVersionVal::v311,
        mqtt::protocol::v311::message::Pubrel<cc_plugin::protocol::v311::Message>,
        Pubrel
    >
{
};

} // namespace message

} // namespace v311

} // namespace protocol

} // namespace cc_plugin

} // namespace mqtt


