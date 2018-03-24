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

#include "comms/protocols.h"
#include "mqtt/protocol/common/version.h"
#include "mqtt/protocol/common/MsgIdFlagsLayer.h"
#include "mqtt/protocol/common/field.h"


namespace mqtt
{

namespace protocol
{

namespace common
{

template <
    typename TMsgBase,
    typename TInputMessages,
    field::ProtocolVersionVal TVer,
    typename TMsgAllocOptions = comms::option::EmptyOption,
    typename TDataFieldStorageOptions = comms::option::EmptyOption>
using Stack =
    MsgIdFlagsLayer<
        TMsgBase,
        TInputMessages,
        comms::protocol::MsgSizeLayer<
            field::RemSize,
            comms::protocol::MsgDataLayer<TDataFieldStorageOptions>
        >,
        TVer,
        TMsgAllocOptions
    >;

} // namespace common

}  // namespace protocol

}  // namespace mqtt
