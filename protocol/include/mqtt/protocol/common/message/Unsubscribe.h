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

#include "comms/MessageBase.h"
#include "mqtt/protocol/common/MsgId.h"
#include "mqtt/protocol/common/field.h"

namespace mqtt
{

namespace protocol
{

namespace common
{

namespace message
{

using UnsubscribeFields = std::tuple<
    common::field::PacketId,
    common::field::UnsubscribePayload
>;

template <typename TMsgBase, template<class> class TActual>
using UnsubscribeBase =
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_UNSUBSCRIBE>,
        comms::option::FieldsImpl<UnsubscribeFields>,
        comms::option::MsgType<TActual<TMsgBase> >
    >;

template <typename TMsgBase>
class Unsubscribe : public UnsubscribeBase<TMsgBase, Unsubscribe>
{
    using UnsubscribeFlagsField = comms::field::IntValue<
        common::field::FieldBase,
        std::uint8_t,
        comms::option::DefaultNumValue<2>,
        comms::option::ValidNumValueRange<2, 2>,
        comms::option::FailOnInvalid<comms::ErrorStatus::ProtocolError>
    > ;

public:

    COMMS_MSG_FIELDS_ACCESS(packetId, payload);

    Unsubscribe()
    {
        using Base = typename std::decay<decltype(comms::toMessageBase(*this))>::type;
        using FlagsField = typename Base::FlagsField;

        FlagsField newFlags(UnsubscribeFlagsField().value());
        Base::setFlags(newFlags);
    }

    Unsubscribe(const Unsubscribe&) = default;
    Unsubscribe(Unsubscribe&& other) = default;
    ~Unsubscribe() = default;

    Unsubscribe& operator=(const Unsubscribe&) = default;
    Unsubscribe& operator=(Unsubscribe&&) = default;

    bool doValid() const
    {
        using Base = typename std::decay<decltype(comms::toMessageBase(*this))>::type;

        auto& flagsField = Base::getFlags();
        UnsubscribeFlagsField actFlags(flagsField.value());

        return actFlags.valid() && Base::doValid();
    }
};

}  // namespace message

} // namespace common

}  // namespace protocol

}  // namespace mqtt
