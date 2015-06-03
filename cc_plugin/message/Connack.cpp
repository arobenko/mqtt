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

#include <type_traits>
#include <functional>

#include "Connack.h"

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

namespace message
{

namespace
{

void updateFlagsInfo(QObject& fieldWidget)
{
    static const QString Names[] = {
        "Session Present"
    };

    static const unsigned NamesCount = std::extent<decltype(Names)>::value;

    for (auto idx = 0U; idx < NamesCount; ++idx) {
        cc::Property::setIndexedNameVal(fieldWidget, idx, Names[idx]);
    }
}

void updateResponseInfo(QObject& fieldWidget)
{
    static const QString Names[] = {
        "Accepted",
        "Wrong Protocol Version",
        "Identifier Rejected",
        "Server Unavailable",
        "Bad Username or Password",
        "Not Authorized"
    };

    static const unsigned NamesCount = std::extent<decltype(Names)>::value;

    static_assert(NamesCount == (unsigned)mqtt::message::ConnackResponseCode::NumOfValues,
        "Invalid map.");

    for (auto idx = 0U; idx < NamesCount; ++idx) {
        cc::Property::setIndexedNameVal(fieldWidget, idx, Names[idx]);
    }
}

}  // namespace

const char* Connack::nameImpl() const
{
    static const char* Str = "CONNACK";
    return Str;
}

void Connack::updateFieldPropertiesImpl(QWidget& fieldWidget, uint idx) const
{
    static const QString Names[] = {
        "Connect Acknowledge Flags",
        "Response"
    };

    static const unsigned NamesCount = std::extent<decltype(Names)>::value;

    static_assert(NamesCount == FieldIdx_NumOfValues,
        "The names map is incorrect");

    if (NamesCount <= idx) {
        return;
    }

    cc::Property::setNameVal(fieldWidget, Names[idx]);

    if (idx == FieldIdx_Flags) {
        static const QString Names[] = {
            "Session Present"
        };
    }

    typedef std::function<void (QObject&)> ExtraUpdateFunc;
    static const ExtraUpdateFunc ExtraFuncs[] = {
        &updateFlagsInfo,
        &updateResponseInfo
    };

    static const unsigned FuncsCount = std::extent<decltype(ExtraFuncs)>::value;

    static_assert(NamesCount == FuncsCount,
        "The update map is incorrect");

    ExtraFuncs[idx](fieldWidget);
}

}  // namespace message

}  // namespace cc_plugin

}  // namespace mqtt

