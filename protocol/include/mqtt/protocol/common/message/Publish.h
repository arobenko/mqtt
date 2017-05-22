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

template <typename TMsgBase, typename TAllFields, typename TActual>
class Publish : public
        comms::MessageBase<
            TMsgBase,
            comms::option::StaticNumIdImpl<MsgId_PUBLISH>,
            comms::option::FieldsImpl<TAllFields>,
            comms::option::MsgType<TActual>,
            comms::option::HasDoRefresh
        >
{
public:
    template <typename TIter>
    comms::ErrorStatus doRead(TIter& iter, std::size_t size)
    {
        using Base = typename std::decay<decltype(comms::toMessageBase(*this))>::type;
        auto& actual = static_cast<TActual&>(*this);

        auto& flagsField = Base::getFlags();
        auto& publishFlagsField = actual.field_publishFlags();
        publishFlagsField = comms::field_cast<typename std::decay<decltype(publishFlagsField)>::type>(flagsField);
        updatePacketId();
        return Base::template readFieldsFrom<TActual::FieldIdx_topic>(iter, size);
    }

    template <typename TIter>
    comms::ErrorStatus doWrite(TIter& iter, std::size_t size) const
    {
        using Base = typename std::decay<decltype(comms::toMessageBase(*this))>::type;
        return Base::template writeFieldsFrom<TActual::FieldIdx_topic>(iter, size);
    }

    std::size_t doLength() const
    {
        using Base = typename std::decay<decltype(comms::toMessageBase(*this))>::type;
        return Base::doLength() - common::field::PublishFlags::minLength();
    }

    bool doRefresh()
    {
        using Base = typename std::decay<decltype(comms::toMessageBase(*this))>::type;
        bool result = false;
        result = updatePacketId() || result;
        result = updateDup() || result;

        auto& actual = static_cast<TActual&>(*this);
        auto& publishFlagsField = actual.field_publishFlags();
        Base::setFlags(comms::field_cast<typename Base::FlagsField>(publishFlagsField));

        return result;
    }

protected:
    Publish() = default;
    Publish(const Publish&) = default;
    Publish(Publish&& other) = default;
    ~Publish() = default;

    Publish& operator=(const Publish&) = default;
    Publish& operator=(Publish&&) = default;


private:
    bool updatePacketId()
    {
        auto& actual = static_cast<TActual&>(*this);
        auto& publishFlagsField = actual.field_publishFlags();
        auto& qosMemberField = publishFlagsField.field_qos();

        typedef typename std::decay<decltype(qosMemberField)>::type QosFieldType;
        comms::field::OptionalMode packetIdMode = comms::field::OptionalMode::Exists;
        if (qosMemberField.value() == QosFieldType::ValueType::AtMostOnceDelivery) {
            packetIdMode = comms::field::OptionalMode::Missing;
        }

        auto& packetIdField = actual.field_packetId();
        bool updated = (packetIdField.getMode() != packetIdMode);
        packetIdField.setMode(packetIdMode);
        return updated;
    }

    bool updateDup()
    {
        auto& actual = static_cast<TActual&>(*this);
        auto& publishFlagsField = actual.field_publishFlags();
        auto& qosMemberField = publishFlagsField.field_qos();

        typedef typename std::decay<decltype(qosMemberField)>::type QosFieldType;
        if (qosMemberField.value() != QosFieldType::ValueType::AtMostOnceDelivery) {
            return false;
        }

        if (publishFlagsField.field_dup().value() != 0) {
            publishFlagsField.field_dup().value() = 0;
            return true;
        }

        return false;
    }
};

} // namespace message

} // namespace common

} // namespace protocol

} // namespace mqtt
