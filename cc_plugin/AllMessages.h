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

namespace mqtt
{

namespace cc_plugin
{

typedef std::tuple<
    cc_plugin::message::Connect,
    cc_plugin::message::Connack,
    cc_plugin::message::Publish,
    cc_plugin::message::Puback
> AllMessages;

}  // namespace cc_plugin

}  // namespace mqtt




