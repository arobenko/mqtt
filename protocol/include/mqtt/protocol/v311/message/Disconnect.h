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
#include "comms/MessageBase.h"
#include "mqtt/protocol/common/MsgId.h"

namespace mqtt
{

namespace protocol
{

namespace v311
{

namespace message
{

template <typename TMsgBase>
class Disconnect : public
        comms::MessageBase<
            TMsgBase,
            comms::option::StaticNumIdImpl<common::MsgId_DISCONNECT>,
            comms::option::ZeroFieldsImpl,
            comms::option::MsgType<Disconnect<TMsgBase> >
        >
{
public:

    enum FieldIdx
    {
        FieldIdx_numOfValues
    };

    Disconnect() = default;
    Disconnect(const Disconnect&) = default;
    Disconnect(Disconnect&& other) = default;
    ~Disconnect() = default;

    Disconnect& operator=(const Disconnect&) = default;
    Disconnect& operator=(Disconnect&&) = default;
};

} // namespace message

} // namespace v311

} // namespace protocol

} // namespace mqtt
