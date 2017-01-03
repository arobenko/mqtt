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
#include <algorithm>

#include "mqtt/protocol/Message.h"
#include "mqtt/protocol/field.h"

namespace mqtt
{

namespace protocol
{

namespace message
{

using SubscribeFields = std::tuple<
    field::PacketId,
    field::SubscribePayload
>;

template <typename TMsgBase, template<class> class TActual>
using SubscribeBase =
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_SUBSCRIBE>,
        comms::option::FieldsImpl<SubscribeFields>,
        comms::option::MsgType<TActual<TMsgBase> >
    >;

template <typename TMsgBase = Message>
class Subscribe : public SubscribeBase<TMsgBase, Subscribe>
{
    typedef SubscribeBase<TMsgBase, Subscribe> Base;
public:

    typedef typename Base::FlagsField FlagsField;

    typedef comms::field::IntValue<
        typename Base::Field,
        std::uint8_t,
        comms::option::DefaultNumValue<2>,
        comms::option::ValidNumValueRange<2, 2>,
        comms::option::FailOnInvalid
    > SubscribeFlagsField;

    COMMS_MSG_FIELDS_ACCESS(Base, packetId, payload);

    Subscribe()
    {
        FlagsField newFlags(SubscribeFlagsField().value());
        Base::setFlags(newFlags);
    }

    Subscribe(const Subscribe&) = default;
    Subscribe(Subscribe&& other) = default;
    virtual ~Subscribe() = default;

    Subscribe& operator=(const Subscribe&) = default;
    Subscribe& operator=(Subscribe&&) = default;

    bool doValid() const
    {
        auto& flagsField = Base::getFlags();
        SubscribeFlagsField actFlags(flagsField.value());

        return actFlags.valid() && Base::doValid();
    }
};

}  // namespace message

}  // namespace protocol

}  // namespace mqtt
