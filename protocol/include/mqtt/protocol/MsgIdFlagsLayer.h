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

#include "comms/comms.h"
#include "mqtt/protocol/MsgId.h"


namespace mqtt
{

namespace protocol
{

typedef comms::field::EnumValue<
    comms::Field<comms::option::BigEndian>,
    MsgId,
    comms::option::FixedBitLength<4>,
    comms::option::DefaultNumValue<MsgId_CONNECT>,
    comms::option::ValidNumValueRange<MsgId_CONNECT, MsgId_NumOfValues - 1>,
    comms::option::FailOnInvalid<comms::ErrorStatus::ProtocolError>
> MsgIdField;

typedef comms::field::IntValue<
    comms::Field<comms::option::BigEndian>,
    std::uint8_t,
    comms::option::FixedBitLength<4>
> FlagsField;

typedef comms::field::Bitfield<
    comms::Field<comms::option::BigEndian>,
    std::tuple<
        FlagsField,
        MsgIdField
    >
> MsgIdFlagsBundle;

template <typename TMessage,
          typename TAllMessages,
          typename TNextLayer,
          typename TMsgAllocOptions = std::tuple<> >
class MsgIdFlagsLayer :
    public comms::protocol::ProtocolLayerBase<MsgIdFlagsBundle, TNextLayer>
{
    typedef comms::protocol::ProtocolLayerBase<MsgIdFlagsBundle, TNextLayer> Base;

    typedef comms::MsgFactory<TMessage, TAllMessages, TMsgAllocOptions> Factory;

public:

    typedef typename Factory::AllMessages AllMessages;

    typedef typename Factory::MsgPtr MsgPtr;

    typedef typename MsgPtr::element_type Message;

    typedef typename Message::MsgIdParamType MsgIdParamType;

    typedef typename Message::MsgIdType MsgIdType;

    typedef typename Base::Field Field;

    typedef typename Base::NextLayer NextLayer;

    enum FieldIdx {
        FieldIdx_Flags,
        FieldIdx_MsgId
    };

    template <typename... TArgs>
    explicit MsgIdFlagsLayer(TArgs&&... args)
       : Base(std::forward<TArgs>(args)...)
    {
    }

    MsgIdFlagsLayer(const MsgIdFlagsLayer&) = default;

    MsgIdFlagsLayer(MsgIdFlagsLayer&&) = default;

    MsgIdFlagsLayer& operator=(const MsgIdFlagsLayer&) = default;

    MsgIdFlagsLayer& operator=(MsgIdFlagsLayer&&) = default;

    /// @brief Destructor
    ~MsgIdFlagsLayer() = default;

    template <typename TIter>
    comms::ErrorStatus read(
        MsgPtr& msgPtr,
        TIter& iter,
        std::size_t size,
        std::size_t* missingSize = nullptr)
    {
        Field field;
        return readInternal(field, msgPtr, iter, size, missingSize, Base::createNextLayerReader());
    }

    template <std::size_t TIdx, typename TAllFields, typename TIter>
    comms::ErrorStatus readFieldsCached(
        TAllFields& allFields,
        MsgPtr& msgPtr,
        TIter& iter,
        std::size_t size,
        std::size_t* missingSize = nullptr)
    {
        auto& field = Base::template getField<TIdx>(allFields);
        return
            readInternal(
                field,
                msgPtr,
                iter,
                size,
                missingSize,
                Base::template createNextLayerCachedFieldsReader<TIdx>(allFields));
    }

    template <typename TMsg, typename TIter>
    comms::ErrorStatus write(
        const TMsg& msg,
        TIter& iter,
        std::size_t size) const
    {
        Field field;
        updateField(msg, field);
        return writeInternal(field, msg, iter, size, Base::createNextLayerWriter());
    }

    template <std::size_t TIdx, typename TAllFields, typename TMsg, typename TIter>
    comms::ErrorStatus writeFieldsCached(
        TAllFields& allFields,
        const TMsg& msg,
        TIter& iter,
        std::size_t size) const
    {
        auto& field = Base::template getField<TIdx>(allFields);
        updateField(msg, field);
        return
            writeInternal(
                field,
                msg,
                iter,
                size,
                Base::template createNextLayerCachedFieldsWriter<TIdx>(allFields));
    }

    MsgPtr createMsg(MsgIdParamType id, unsigned idx = 0)
    {
        return factory_.createMsg(id, idx);
    }

private:

    template <typename TIter, typename TReader>
    comms::ErrorStatus readInternal(
        Field& field,
        MsgPtr& msgPtr,
        TIter& iter,
        std::size_t size,
        std::size_t* missingSize,
        TReader&& reader)
    {
        GASSERT(!msgPtr);
        auto es = field.read(iter, size);
        if (es == comms::ErrorStatus::NotEnoughData) {
            Base::updateMissingSize(field, size, missingSize);
        }

        if (es != comms::ErrorStatus::Success) {
            return es;
        }

        auto& members = field.value();
        auto& idField = std::get<FieldIdx_MsgId>(members);
        auto id = idField.value();

        msgPtr = createMsg(id);

        if (!msgPtr) {
            if (factory_.msgCount(id) == 0U) {
                return comms::ErrorStatus::InvalidMsgId;
            }

            return comms::ErrorStatus::MsgAllocFailure;
        }

        auto& flagsField = std::get<FieldIdx_Flags>(members);
        typedef typename Message::FlagsField MsgFlagsField;

        MsgFlagsField msgFlagsField(flagsField.value());
        msgPtr->setFlags(msgFlagsField);

        es = reader.read(msgPtr, iter, size - field.length(), missingSize);
        if (es != comms::ErrorStatus::Success) {
            msgPtr.reset();
        }

        return es;
    }

    template <typename TMsg, typename TIter, typename TWriter>
    comms::ErrorStatus writeInternal(
        Field& field,
        const TMsg& msg,
        TIter& iter,
        std::size_t size,
        TWriter&& nextLayerWriter) const
    {
        auto es = field.write(iter, size);
        if (es != comms::ErrorStatus::Success) {
            return es;
        }

        GASSERT(field.length() <= size);
        return nextLayerWriter.write(msg, iter, size - field.length());
    }

    template <typename TMsg>
    static void updateField(const TMsg& msg, Field& field)
    {
        auto& members = field.value();

        auto& flagsField = std::get<FieldIdx_Flags>(members);
        flagsField.value() = msg.getFlags().value();

        auto& msgIdField = std::get<FieldIdx_MsgId>(members);
        msgIdField.value() = msg.getId();
    }

    Factory factory_;
};


}  // namespace protocol

}  // namespace mqtt


