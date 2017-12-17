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
class MsgIdFlagsLayer : public
    comms::protocol::ProtocolLayerBase<
        field::MsgIdFlagsBundle<TVer>,
        TNextLayer,
        MsgIdFlagsLayer<TMessage, TAllMessages, TNextLayer, TVer, TMsgAllocOptions>
    >
{
    using Base =
        comms::protocol::ProtocolLayerBase<
            field::MsgIdFlagsBundle<TVer>,
            TNextLayer,
            MsgIdFlagsLayer<TMessage, TAllMessages, TNextLayer, TVer, TMsgAllocOptions>
        >;

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

    template <typename TMsgPtr, typename TIter, typename TNextLayerReader>
    comms::ErrorStatus doRead(
        Field& field,
        TMsgPtr& msgPtr,
        TIter& iter,
        std::size_t size,
        std::size_t* missingSize,
        TNextLayerReader&& nextLayerReader)
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

        es = nextLayerReader.read(msgPtr, iter, size - field.length(), missingSize);
        if (es != comms::ErrorStatus::Success) {
            msgPtr.reset();
        }

        return es;
    }

    template <typename TMsg, typename TIter, typename TNextLayerWriter>
    comms::ErrorStatus doWrite(
        Field& field,
        const TMsg& msg,
        TIter& iter,
        std::size_t size,
        TNextLayerWriter&& nextLayerWriter) const
    {
        updateField(msg, field);
        auto es = field.write(iter, size);
        if (es != comms::ErrorStatus::Success) {
            return es;
        }

        GASSERT(field.length() <= size);
        return nextLayerWriter.write(msg, iter, size - field.length());
    }

    MsgPtr createMsg(MsgIdParamType id, unsigned idx = 0)
    {
        return factory_.createMsg(id, idx);
    }

private:

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


