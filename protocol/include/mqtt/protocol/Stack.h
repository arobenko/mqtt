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

#include "comms/comms.h"

#include "MsgIdFlagsLayer.h"

#include "MsgId.h"
#include "AllMessages.h"


namespace mqtt
{

namespace protocol
{

typedef comms::field::IntValue<
    comms::Field<comms::option::LittleEndian>,
    std::uint32_t,
    comms::option::VarLength<1, 4>
> RemSizeField;

template <
    typename TMsgBase = Message,
    typename TAllMessages = AllMessages<TMsgBase>,
    typename TMsgAllocOptions = std::tuple<> >
using Stack =
    mqtt::protocol::MsgIdFlagsLayer<
        TAllMessages,
        comms::protocol::MsgSizeLayer<
            RemSizeField,
            comms::protocol::MsgDataLayer<TMsgBase>
        >,
        TMsgAllocOptions
    >;

}  // namespace protocol

}  // namespace mqtt
