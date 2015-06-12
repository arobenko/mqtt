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


struct SubscribeTopicValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        auto& topic = field.getValue();
        return (!topic.empty());
    }
};

template <typename TFieldBase>
using SubscribeTopicField =
    comms::field::String<
        TFieldBase,
        comms::option::ContentsValidator<SubscribeTopicValidator>,
        comms::option::SequenceSizeFieldPrefix<
            comms::field::IntValue<
                TFieldBase,
                std::uint16_t
            >
        >
    >;

template <typename TFieldBase>
using SubscribePacketIdField =
        mqtt::field::PacketId<TFieldBase>;

struct SubscribePayloadValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        return 0U < field.getValue().size();
    }
};

template <typename TFieldBase>
using SubscribePayload =
    comms::field::ArrayList<
        TFieldBase,
        comms::field::Bundle<
            std::tuple<
                SubscribeTopicField<TFieldBase>,
                mqtt::field::QoS<TFieldBase>
            >
        >,
        comms::option::ContentsValidator<SubscribePayloadValidator>
    >;


template <typename TFieldBase>
using SubscribeFields = std::tuple<
    SubscribePacketIdField<TFieldBase>,
    SubscribePayload<TFieldBase>
>;

template <typename TMsgBase = Message>
class Subscribe : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_SUBSCRIBE>,
        comms::option::FieldsImpl<SubscribeFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Subscribe<TMsgBase> >
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_SUBSCRIBE>,
        comms::option::FieldsImpl<SubscribeFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Subscribe<TMsgBase> >
    > Base;
public:

    typedef typename Base::FlagsField FlagsField;

    typedef comms::field::IntValue<
        typename Base::Field,
        std::uint8_t,
        comms::option::DefaultNumValue<2>,
        comms::option::ValidNumValueRange<2, 2>,
        comms::option::FailOnInvalid
    > SubscribeFlagsField;

    enum FieldIdx
    {
        FieldIdx_PacketId,
        FieldIdx_Payload,
        FieldIdx_NumOfValues
    };

    static_assert(std::tuple_size<typename Base::AllFields>::value == FieldIdx_NumOfValues,
        "Number of fields is incorrect");

    Subscribe()
    {
        FlagsField newFlags(SubscribeFlagsField().getValue());
        Base::setFlags(newFlags);
    }

    Subscribe(const Subscribe&) = default;
    Subscribe(Subscribe&& other)
    {
    }
    virtual ~Subscribe() = default;

    Subscribe& operator=(const Subscribe&) = default;
    Subscribe& operator=(Subscribe&&) = default;

protected:

    virtual bool validImpl() const override
    {
        auto& flagsField = Base::getFlags();
        SubscribeFlagsField actFlags(flagsField.getValue());

        return actFlags.valid() && Base::validImpl();
    }
};

}  // namespace message


}  // namespace mqtt
