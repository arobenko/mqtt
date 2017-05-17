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

#include "mqtt/protocol/common/Stack.h"
#include "mqtt/protocol/common/field.h"

namespace mqtt
{

namespace protocol
{

namespace v5
{

template <
    typename TMsgBase,
    typename TInputMessages,
    typename TMsgAllocOptions = std::tuple<> >
using Stack =
    common::Stack<
        TMsgBase,
        TInputMessages,
        common::field::ProtocolVersionVal::v5,
        TMsgAllocOptions>;

} // namespace v5

}  // namespace protocol

}  // namespace mqtt


