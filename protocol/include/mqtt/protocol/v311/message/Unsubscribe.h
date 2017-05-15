//
// Copyright 2015 - 2017 (C). Alex Robenko. All rights reserved.
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

#include "mqtt/protocol/v311/Message.h"
#include "mqtt/protocol/v311/field.h"

namespace mqtt
{

namespace protocol
{

namespace v311
{

namespace message
{

using UnsubscribeFields = std::tuple<
    field::PacketId,
    field::UnsubscribePayload
>;

template <typename TMsgBase, template<class> class TActual>
using UnsubscribeBase =
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_UNSUBSCRIBE>,
        comms::option::FieldsImpl<UnsubscribeFields>,
        comms::option::MsgType<TActual<TMsgBase> >
    >;

template <typename TMsgBase = Message>
class Unsubscribe : public UnsubscribeBase<TMsgBase, Unsubscribe>
{
    using Base = UnsubscribeBase<TMsgBase, mqtt::protocol::v311::message::Unsubscribe>;
public:

    typedef typename Base::FlagsField FlagsField;

    typedef comms::field::IntValue<
        typename Base::Field,
        std::uint8_t,
        comms::option::DefaultNumValue<2>,
        comms::option::ValidNumValueRange<2, 2>,
        comms::option::FailOnInvalid<comms::ErrorStatus::ProtocolError>
    > UnsubscribeFlagsField;

    COMMS_MSG_FIELDS_ACCESS(packetId, payload);

    Unsubscribe()
    {
        FlagsField newFlags(UnsubscribeFlagsField().value());
        Base::setFlags(newFlags);
    }

    Unsubscribe(const Unsubscribe&) = default;
    Unsubscribe(Unsubscribe&& other) = default;
    virtual ~Unsubscribe() = default;

    Unsubscribe& operator=(const Unsubscribe&) = default;
    Unsubscribe& operator=(Unsubscribe&&) = default;

    bool doValid() const
    {
        auto& flagsField = Base::getFlags();
        UnsubscribeFlagsField actFlags(flagsField.value());

        return actFlags.valid() && Base::doValid();
    }
};

}  // namespace message

} // namespace v311

}  // namespace protocol

}  // namespace mqtt
