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

#include "Protocol.h"

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

Protocol::~Protocol() = default;

const std::string& Protocol::nameImpl() const
{
    static const std::string Str("MQTT");
    return Str;
}

void Protocol::updateMessageInfoImpl(cc::MessageInfo& msgInfo)
{
    auto msgPtr = msgInfo.getAppMessage();
    if (!msgPtr) {
        return;
    }

    auto* castedMsgPtr = dynamic_cast<Message*>(msgPtr.get());
    if (castedMsgPtr == nullptr) {
        return;
    }

    castedMsgPtr->updateFlags();

    Base::updateMessageInfoImpl(msgInfo);
}

}  // namespace cc_plugin

}  // namespace mqtt


