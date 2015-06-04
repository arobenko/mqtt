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
#include "mqtt/Message.h"
#include "mqtt/field/PacketId.h"

namespace mqtt
{

namespace message
{

template <typename TFieldBase>
using PubcompFields = std::tuple<
    mqtt::field::PacketId<TFieldBase>
>;

template <typename TMsgBase = Message>
class Pubcomp : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PUBCOMP>,
        comms::option::FieldsImpl<PubcompFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Pubcomp<TMsgBase> >
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PUBCOMP>,
        comms::option::FieldsImpl<PubcompFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Pubcomp<TMsgBase> >
    > Base;
public:
    enum FieldIdx
    {
        FieldIdx_PacketId,
        FieldIdx_NumOfValues
    };

    static_assert(std::tuple_size<typename Base::AllFields>::value == FieldIdx_NumOfValues,
        "Number of fields is incorrect");

    Pubcomp() = default;
    Pubcomp(const Pubcomp&) = default;
    Pubcomp(Pubcomp&& other)
    {
    }
    virtual ~Pubcomp() = default;

    Pubcomp& operator=(const Pubcomp&) = default;
    Pubcomp& operator=(Pubcomp&&) = default;
};

}  // namespace message


}  // namespace mqtt
