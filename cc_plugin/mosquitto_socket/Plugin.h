//
// Copyright 2014 - 2016 (C). Alex Robenko. All rights reserved.
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

#include <QtCore/QObject>
#include <QtCore/QtPlugin>
#include "comms_champion/comms_champion.h"

#include "Socket.h"
#include "ConnectAction.h"

namespace mqtt
{

namespace cc_plugin
{

namespace mosquitto_socket
{

class Plugin : public comms_champion::Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "MQTT.Mosquitto.Socket" FILE "mosquitto_socket.json")
    Q_INTERFACES(comms_champion::Plugin)

public:
    Plugin();
    ~Plugin();

protected:
    virtual void getCurrentConfigImpl(QVariantMap& config) override;
    virtual void reconfigureImpl(const QVariantMap& config) override;

private slots:
    void connectStatusChangeRequest(bool connected);
    void connectionStatusChanged(bool connected);

private:
    void createSocketIfNeeded();

    std::shared_ptr<Socket> m_socket;
    ConnectAction* m_connectAction = nullptr;
};

}  // namespace mosquitto_socket

}  // namespace cc_plugin

}  // namespace mqtt
