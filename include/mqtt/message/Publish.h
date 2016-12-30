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
#include "mqtt/field.h"

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
        auto& members = field.value();
        auto& qosField = std::get<PublishActualFlagIdx_QoS>(members);
        typedef typename std::decay<decltype(qosField)>::type QosFieldType;
        if (qosField.value() != QosFieldType::ValueType::AtMostOnceDelivery) {
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
using PublishFlags =
    comms::field::Bitfield<
        TFieldBase,
        std::tuple<
            comms::field::BitmaskValue<
                TFieldBase,
                comms::option::FixedBitLength<1>
            >,
            mqtt::field::QoS<
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
        auto& topic = field.value();
        return
            (!topic.empty()) &&
            (std::none_of(
                topic.begin(), topic.end(),
                [](char ch) -> bool
                {
                    return (ch == '#') || (ch == '+');
                }));
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
using PublishPayload =
    comms::field::ArrayList<
        TFieldBase,
        std::uint8_t
    >;


template <typename TFieldBase>
using PublishFields = std::tuple<
    PublishFlags<TFieldBase>,
    PublishTopicField<TFieldBase>,
    field::OptionalPacketId,
    PublishPayload<TFieldBase>
>;

template <typename TMsgBase = Message>
class Publish : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PUBLISH>,
        comms::option::FieldsImpl<PublishFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Publish<TMsgBase> >,
        comms::option::NoDefaultFieldsReadImpl,
        comms::option::NoDefaultFieldsWriteImpl
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PUBLISH>,
        comms::option::FieldsImpl<PublishFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Publish<TMsgBase> >,
        comms::option::NoDefaultFieldsReadImpl,
        comms::option::NoDefaultFieldsWriteImpl
    > Base;
public:

    COMMS_MSG_FIELDS_ACCESS(Base, publishFlags, topic, packetId, payload);

    Publish()
    {
        auto& allFields = Base::fields();
        auto& packetIdField = std::get<FieldIdx_packetId>(allFields);
        packetIdField.setMissing();
    }

    Publish(const Publish&) = default;
    Publish(Publish&& other) = default;
    virtual ~Publish() = default;

    Publish& operator=(const Publish&) = default;
    Publish& operator=(Publish&&) = default;

protected:

    virtual bool refreshImpl() override
    {
        bool result = updatePacketId() && updateDup();

        auto& allFields = Base::fields();
        auto& publishFlagsField = std::get<FieldIdx_publishFlags>(allFields);
        Base::setFlags(comms::field_cast<typename Base::FlagsField>(publishFlagsField));

        return result;
    }

    virtual comms::ErrorStatus readImpl(
        typename Base::ReadIterator& iter,
        std::size_t size) override
    {
        auto& flagsField = Base::getFlags();
        auto& allFields = Base::fields();
        auto& publishFlagsField = std::get<FieldIdx_publishFlags>(allFields);
        publishFlagsField = comms::field_cast<typename std::decay<decltype(publishFlagsField)>::type>(flagsField);
        updatePacketId();
        return Base::template readFieldsFrom<FieldIdx_topic>(iter, size);
    }

    virtual comms::ErrorStatus writeImpl(
        typename Base::WriteIterator& iter,
        std::size_t size) const override
    {
        return Base::template writeFieldsFrom<FieldIdx_topic>(iter, size);
    }

    virtual std::size_t lengthImpl() const override
    {
        return Base::lengthImpl() - PublishFlags<typename Base::Field>::minLength();
    }

private:
    bool updatePacketId()
    {
        auto& allFields = Base::fields();
        auto& publishFlagsField = std::get<FieldIdx_publishFlags>(allFields);
        auto& publishFlagsMembers = publishFlagsField.value();
        auto& qosMemberField = std::get<PublishActualFlagIdx_QoS>(publishFlagsMembers);

        typedef typename std::decay<decltype(qosMemberField)>::type QosFieldType;
        comms::field::OptionalMode packetIdMode = comms::field::OptionalMode::Exists;
        if (qosMemberField.value() == QosFieldType::ValueType::AtMostOnceDelivery) {
            packetIdMode = comms::field::OptionalMode::Missing;
        }

        auto& packetIdField = std::get<FieldIdx_packetId>(Base::fields());
        bool updated = (packetIdField.getMode() != packetIdMode);
        packetIdField.setMode(packetIdMode);
        return updated;
    }

    bool updateDup()
    {
        auto& allFields = Base::fields();
        auto& publishFlagsField = std::get<FieldIdx_publishFlags>(allFields);
        auto& publishFlagsMembers = publishFlagsField.value();
        auto& qosMemberField = std::get<PublishActualFlagIdx_QoS>(publishFlagsMembers);
        auto& dupFlagsField = std::get<PublishActualFlagIdx_Dup>(publishFlagsMembers);

        typedef typename std::decay<decltype(qosMemberField)>::type QosFieldType;
        if (qosMemberField.value() != QosFieldType::ValueType::AtMostOnceDelivery) {
            return false;
        }

        if (dupFlagsField.value() != 0) {
            dupFlagsField.value() = 0;
            return true;
        }

        return false;
    }
};

}  // namespace message


}  // namespace mqtt
