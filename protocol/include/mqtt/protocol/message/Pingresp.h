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
#include "mqtt/protocol/Message.h"

namespace mqtt
{

namespace protocol
{

namespace message
{

template <typename TMsgBase = Message>
class Pingresp : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PINGRESP>,
        comms::option::NoFieldsImpl,
        comms::option::DispatchImpl<Pingresp<TMsgBase> >
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PINGRESP>,
        comms::option::NoFieldsImpl,
        comms::option::DispatchImpl<Pingresp<TMsgBase> >
    > Base;
public:

    static_assert(std::tuple_size<typename Base::AllFields>::value == 0U,
        "Number of fields is incorrect");

    Pingresp() = default;
    Pingresp(const Pingresp&) = default;
    Pingresp(Pingresp&& other) = default;
    virtual ~Pingresp() = default;

    Pingresp& operator=(const Pingresp&) = default;
    Pingresp& operator=(Pingresp&&) = default;
};

}  // namespace message

}  // namespace protocol

}  // namespace mqtt
