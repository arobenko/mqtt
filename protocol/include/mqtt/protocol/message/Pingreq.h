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

template <typename TMsgBase, template<class> class TActual>
using PingreqBase =
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PINGREQ>,
        comms::option::NoFieldsImpl,
        comms::option::MsgType<TActual<TMsgBase> >
    >;

template <typename TMsgBase = Message>
class Pingreq : public PingreqBase<TMsgBase, Pingreq>
{
    typedef PingreqBase<TMsgBase, Pingreq> Base;
public:

    static_assert(std::tuple_size<typename Base::AllFields>::value == 0U,
        "Number of fields is incorrect");

    Pingreq() = default;
    Pingreq(const Pingreq&) = default;
    Pingreq(Pingreq&& other) = default;
    virtual ~Pingreq() = default;

    Pingreq& operator=(const Pingreq&) = default;
    Pingreq& operator=(Pingreq&&) = default;
};

}  // namespace message

}  // namespace protocol

}  // namespace mqtt
