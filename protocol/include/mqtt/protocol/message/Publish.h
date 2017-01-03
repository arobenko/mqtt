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

using PublishFields = std::tuple<
    field::PublishFlags,
    field::PublishTopic,
    field::OptionalPacketId,
    field::Payload
>;

template <typename TMsgBase, template<class> class TActual>
using PublishBase =
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_PUBLISH>,
        comms::option::FieldsImpl<PublishFields>,
        comms::option::MsgType<TActual<TMsgBase> >,
        comms::option::HasDoRefresh
    >;

template <typename TMsgBase = Message>
class Publish : public PublishBase<TMsgBase, Publish>
{
    typedef PublishBase<TMsgBase, Publish> Base;
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

    template <typename TIter>
    comms::ErrorStatus doRead(TIter& iter, std::size_t size)
    {
        auto& flagsField = Base::getFlags();
        auto& allFields = Base::fields();
        auto& publishFlagsField = std::get<FieldIdx_publishFlags>(allFields);
        publishFlagsField = comms::field_cast<typename std::decay<decltype(publishFlagsField)>::type>(flagsField);
        updatePacketId();
        return Base::template readFieldsFrom<FieldIdx_topic>(iter, size);
    }

    template <typename TIter>
    comms::ErrorStatus doWrite(TIter& iter, std::size_t size) const
    {
        return Base::template writeFieldsFrom<FieldIdx_topic>(iter, size);
    }

    std::size_t doLength() const
    {
        return Base::doLength() - field::PublishFlags::minLength();
    }

    bool doRefresh()
    {
        bool result = false;
        result = updatePacketId() || result;
        result = updateDup() || result;

        auto& allFields = Base::fields();
        auto& publishFlagsField = std::get<FieldIdx_publishFlags>(allFields);
        Base::setFlags(comms::field_cast<typename Base::FlagsField>(publishFlagsField));

        return result;
    }

private:
    bool updatePacketId()
    {
        auto& allFields = Base::fields();
        auto& publishFlagsField = std::get<FieldIdx_publishFlags>(allFields);

        typedef typename std::decay<decltype(publishFlagsField)>::type FlagsFieldType;
        auto& publishFlagsMembers = publishFlagsField.value();
        auto& qosMemberField = std::get<FlagsFieldType::FieldIdx_qos>(publishFlagsMembers);

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
        auto publishFlagsMembers = publishFlagsField.fieldsAsStruct();

        typedef typename std::decay<decltype(publishFlagsMembers.qos)>::type QosFieldType;
        if (publishFlagsMembers.qos.value() != QosFieldType::ValueType::AtMostOnceDelivery) {
            return false;
        }

        if (publishFlagsMembers.dup.value() != 0) {
            publishFlagsMembers.dup.value() = 0;
            return true;
        }

        return false;
    }
};

}  // namespace message

}  // namespace protocol

}  // namespace mqtt
