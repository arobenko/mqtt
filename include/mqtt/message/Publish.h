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
#include "mqtt/field/QoS.h"

namespace mqtt
{

namespace message
{

enum PublishActualFlagIdx
{
    PublishActualFlagIdx_Retain,
    PublishActualFlagIdx_QoS,
    PublishActualFlagIdx_Dup,
    PublishActualFlagIdx_Reserved,
    ActualFlagIdx_NumOfMembers
};

struct PublishActualFlagsValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        auto& members = field.members();
        auto& qosField = std::get<PublishActualFlagIdx_QoS>(members);
        if (qosField.getValue() != mqtt::field::QosType::AtMostOnceDelivery) {
            return true;
        }

        auto& dupField = std::get<PublishActualFlagIdx_Dup>(members);
        if (dupField.getBitValue(0)) {
            return false;
        }
        return true;
    }
};


template <typename TFieldBase>
using PublshFlags =
    comms::field::Bitfield<
        TFieldBase,
        std::tuple<
            comms::field::BitmaskValue<
                TFieldBase,
                comms::option::FixedBitLength<1>
            >,
            mqtt::field::QoS<
                TFieldBase,
                comms::option::FixedBitLength<2>
            >,
            comms::field::BitmaskValue<
                TFieldBase,
                comms::option::FixedBitLength<1>
            >,
            comms::field::IntValue<
                TFieldBase,
                std::uint8_t,
                comms::option::FixedBitLength<4>
            >
        >,
        comms::option::ContentsValidator<PublishActualFlagsValidator>
    >;

struct PublishTopicValidator
{
    template <typename TField>
    bool operator()(TField&& field) const
    {
        return !field.getValue().empty();
        // TODO: add no wildcards
    }
};

template <typename TFieldBase>
using PublishTopicField =
    comms::field::String<
        TFieldBase,
        comms::option::ContentsValidator<PublishTopicValidator>,
        comms::option::SequenceSizeFieldPrefix<
            comms::field::IntValue<
                TFieldBase,
                std::uint16_t
            >
        >
    >;


template <typename TFieldBase>
using PublishFields = std::tuple<
    PublishTopicField<TFieldBase>
>;

template <typename TMsgBase = Message>
class Publish : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PUBLISH>,
        comms::option::FieldsImpl<PublishFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Publish<TMsgBase> >
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PUBLISH>,
        comms::option::FieldsImpl<PublishFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Publish<TMsgBase> >
    > Base;
public:

    typedef PublshFlags<typename TMsgBase::Field> ActualFlags;

    enum FieldIdx
    {
        FieldIdx_Topic,
        FieldIdx_NumOfValues
    };

    static_assert(std::tuple_size<typename Base::AllFields>::value == FieldIdx_NumOfValues,
        "Number of fields is incorrect");

    Publish() = default;
    Publish(const Publish&) = default;
    Publish(Publish&& other)
    {
    }
    virtual ~Publish() = default;

    Publish& operator=(const Publish&) = default;
    Publish& operator=(Publish&&) = default;
};

}  // namespace message


}  // namespace mqtt
