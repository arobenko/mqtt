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

#include <cstdint>
#include <tuple>
#include <vector>
#include "comms/comms.h"

#include "MsgId.h"

namespace mqtt
{

class MsgHandler;

typedef std::tuple<
    comms::option::BigEndian,
    comms::option::ReadIterator<const std::uint8_t*>,
    comms::option::WriteIterator<std::back_insert_iterator<std::vector<std::uint8_t> > >,
    comms::option::MsgIdType<MsgId>,
    comms::option::Handler<MsgHandler>
> DefaultOptions;

template <typename... TOptions>
class MessageT : public comms::Message<TOptions...>
{
    typedef comms::Message<TOptions...> Base;
public:

    typedef typename Base::Field Field;
    typedef comms::field::IntValue<
        Field,
        std::uint8_t,
        comms::option::ValidNumValueRange<0x0, 0xf>,
        comms::option::FailOnInvalid
    > FlagsField;

    MessageT() = default;
    MessageT(const MessageT&) = default;
    MessageT(MessageT&&) = default;
    virtual ~MessageT() = default;

    MessageT& operator=(const MessageT&) = default;
    MessageT& operator=(MessageT&) = default;

    const FlagsField& getFlags() const
    {
        return m_flags;
    }

    void setFlags(const FlagsField& flags)
    {
        m_flags = flags;
    }

    bool refresh()
    {
        return refreshImpl();
    }

protected:
    virtual bool refreshImpl()
    {
        return false;
    }

private:
    FlagsField m_flags;
};

typedef MessageT<DefaultOptions> Message;

}  // namespace mqtt
