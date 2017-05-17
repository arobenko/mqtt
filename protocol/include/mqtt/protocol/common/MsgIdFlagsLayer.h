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

#include "comms/protocols.h"
#include "field.h"


namespace mqtt
{

namespace protocol
{

namespace common
{

template <typename TMessage,
          typename TAllMessages,
          typename TNextLayer,
          field::ProtocolVersionVal TVer,
          typename TMsgAllocOptions = std::tuple<> >
class MsgIdFlagsLayer :
    public comms::protocol::ProtocolLayerBase<field::MsgIdFlagsBundle<TVer>, TNextLayer>
{
    using Base = comms::protocol::ProtocolLayerBase<field::MsgIdFlagsBundle<TVer>, TNextLayer>;

    using Factory = comms::MsgFactory<TMessage, TAllMessages, TMsgAllocOptions>;

public:

    using AllMessages = typename Factory::AllMessages;

    using MsgPtr = typename Factory::MsgPtr;

    using Message = typename MsgPtr::element_type;

    using MsgIdParamType = typename Message::MsgIdParamType;

    using MsgIdType = typename Message::MsgIdType;

    using Field = typename Base::Field;

    using NextLayer = typename Base::NextLayer;

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

        auto id = field.field_id().value();

        msgPtr = createMsg(id);

        if (!msgPtr) {
            if (factory_.msgCount(id) == 0U) {
                return comms::ErrorStatus::InvalidMsgId;
            }

            return comms::ErrorStatus::MsgAllocFailure;
        }

        using MsgFlagsField = typename Message::FlagsField;

        MsgFlagsField msgFlagsField(field.field_flags().value());
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
        field.field_flags().value() = msg.getFlags().value();
        field.field_id().value() = msg.getId();
    }

    Factory factory_;
};

} // namespace common

}  // namespace protocol

}  // namespace mqtt


