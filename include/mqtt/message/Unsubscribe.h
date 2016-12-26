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
#include <algorithm>

#include "mqtt/Message.h"
#include "mqtt/field/QoS.h"
#include "mqtt/field/PacketId.h"

namespace mqtt
{

namespace message
{


struct UnsubscribeTopicValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        auto& topic = field.value();
        return (!topic.empty());
    }
};

template <typename TFieldBase>
using UnsubscribeTopicField =
    comms::field::String<
        TFieldBase,
        comms::option::ContentsValidator<UnsubscribeTopicValidator>,
        comms::option::SequenceSizeFieldPrefix<
            comms::field::IntValue<
                TFieldBase,
                std::uint16_t
            >
        >
    >;

template <typename TFieldBase>
using UnsubscribePacketIdField =
        mqtt::field::PacketId<TFieldBase>;

struct UnsubscribePayloadValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        return 0U < field.value().size();
    }
};

template <typename TFieldBase>
using UnsubscribePayload =
    comms::field::ArrayList<
        TFieldBase,
        UnsubscribeTopicField<TFieldBase>,
        comms::option::ContentsValidator<UnsubscribePayloadValidator>
    >;


template <typename TFieldBase>
using UnsubscribeFields = std::tuple<
    UnsubscribePacketIdField<TFieldBase>,
    UnsubscribePayload<TFieldBase>
>;

template <typename TMsgBase = Message>
class Unsubscribe : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_UNSUBSCRIBE>,
        comms::option::FieldsImpl<UnsubscribeFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Unsubscribe<TMsgBase> >
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_UNSUBSCRIBE>,
        comms::option::FieldsImpl<UnsubscribeFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Unsubscribe<TMsgBase> >
    > Base;
public:

    typedef typename Base::FlagsField FlagsField;

    typedef comms::field::IntValue<
        typename Base::Field,
        std::uint8_t,
        comms::option::DefaultNumValue<2>,
        comms::option::ValidNumValueRange<2, 2>,
        comms::option::FailOnInvalid
    > UnsubscribeFlagsField;

    COMMS_MSG_FIELDS_ACCESS(Base, packetId, payload);

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

protected:

    virtual bool validImpl() const override
    {
        auto& flagsField = Base::getFlags();
        UnsubscribeFlagsField actFlags(flagsField.value());

        return actFlags.valid() && Base::validImpl();
    }
};

}  // namespace message


}  // namespace mqtt
