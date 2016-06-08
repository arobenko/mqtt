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


#include "Plugin.h"
#include "SocketConfigWidget.h"

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

namespace mosquitto_socket
{

namespace
{

const QString MainConfigKey("mqtt_mosquitto_socket");
const QString HostSubKey("host");
const QString PortSubKey("port");
const QString ClientIdKey("client_id");
const QString CleanSessionKey("clean");
const QString KeepAliveKey("keep_alive");
const QString SubscribeKey("sub");
const QString SubscribeQosKey("sub_qos");
const QString PublishKey("pub");
const QString PublishQosKey("pub_qos");

}  // namespace

Plugin::Plugin()
{
    pluginProperties()
        .setSocketCreateFunc(
            [this]() -> cc::SocketPtr
            {
                createSocketIfNeeded();
                return m_socket;
            })
        .setConfigWidgetCreateFunc(
            [this]()
            {
                createSocketIfNeeded();
                return new SocketConfigWidget(*m_socket);
            })
        .setGuiActionsCreateFunc(
            [this]() -> ListOfGuiActions
            {
                ListOfGuiActions list;
                m_connectAction = new ConnectAction();
                createSocketIfNeeded();
                m_connectAction->setConnected(m_socket->isConnected());
                connect(
                    m_connectAction, SIGNAL(sigConnectStateChangeReq(bool)),
                    this, SLOT(connectStatusChangeRequest(bool)));
                list.append(m_connectAction);
                return list;
            });
}

Plugin::~Plugin() = default;

void Plugin::getCurrentConfigImpl(QVariantMap& config)
{
    static_cast<void>(config);
    createSocketIfNeeded();

    QVariantMap subConfig;
    subConfig.insert(HostSubKey, m_socket->getHost());
    subConfig.insert(PortSubKey, m_socket->getPort());
    subConfig.insert(ClientIdKey, m_socket->getId());
    subConfig.insert(CleanSessionKey, m_socket->getCleanSession());
    subConfig.insert(KeepAliveKey, m_socket->getKeepAlivePeriod());
    subConfig.insert(SubscribeKey, m_socket->getSubTopics());
    subConfig.insert(SubscribeQosKey, m_socket->getSubQos());
    subConfig.insert(PublishKey, m_socket->getPubTopic());
    subConfig.insert(PublishQosKey, m_socket->getPubQos());
    config.insert(MainConfigKey, QVariant::fromValue(subConfig));
}

void Plugin::reconfigureImpl(const QVariantMap& config)
{
    static_cast<void>(config);
    auto subConfigVar = config.value(MainConfigKey);
    if ((!subConfigVar.isValid()) || (!subConfigVar.canConvert<QVariantMap>())) {
        return;
    }

    createSocketIfNeeded();
    assert(m_socket);

    auto subConfig = subConfigVar.value<QVariantMap>();
    {
        auto hostVar = subConfig.value(HostSubKey);
        if (hostVar.isValid() && hostVar.canConvert<QString>()) {
            auto host = hostVar.value<QString>();
            m_socket->setHost(host);
        }
    }

    {
        typedef Socket::PortType PortType;
        auto portVar = subConfig.value(PortSubKey);
        if (portVar.isValid() && portVar.canConvert<PortType>()) {
            auto port = portVar.value<PortType>();
            m_socket->setPort(port);
        }
    }

    {
        auto idVar = subConfig.value(ClientIdKey);
        if (idVar.isValid() && idVar.canConvert<QString>()) {
            m_socket->setId(idVar.toString());
        }
    }

    {
        auto cleanSessionVar = subConfig.value(CleanSessionKey);
        if (cleanSessionVar.isValid() && cleanSessionVar.canConvert<bool>()) {
            m_socket->setCleanSession(cleanSessionVar.value<bool>());
        }
    }

    {
        auto keepAliveVar = subConfig.value(KeepAliveKey);
        if (keepAliveVar.isValid() && keepAliveVar.canConvert<int>()) {
            m_socket->setKeepAlivePeriod(keepAliveVar.toInt());
        }
    }

    {
        auto subVar = subConfig.value(SubscribeKey);
        if (subVar.isValid() && subVar.canConvert<QString>()) {
            m_socket->setSubTopics(subVar.toString());
        }
    }

    {
        auto subQosVar = subConfig.value(SubscribeQosKey);
        if (subQosVar.isValid() && subQosVar.canConvert<int>()) {
            m_socket->setSubQos(subQosVar.toInt());
        }
    }

    {
        auto pubVar = subConfig.value(PublishKey);
        if (pubVar.isValid() && pubVar.canConvert<QString>()) {
            m_socket->setPubTopic(pubVar.toString());
        }
    }

    {
        auto pubQosVar = subConfig.value(PublishQosKey);
        if (pubQosVar.isValid() && pubQosVar.canConvert<int>()) {
            m_socket->setPubQos(pubQosVar.toInt());
        }
    }
}

void Plugin::connectStatusChangeRequest(bool connected)
{
    assert(m_socket);
    m_socket->setConnected(connected);
}

void Plugin::connectionStatusChanged(bool connected)
{
    assert(m_connectAction != nullptr);
    m_connectAction->setConnected(connected);
}

void Plugin::createSocketIfNeeded()
{
    if (!m_socket) {
        m_socket.reset(new Socket());
        connect(
            m_socket.get(), SIGNAL(sigConnectionStatus(bool)),
            this, SLOT(connectionStatusChanged(bool)));
    }
}


}  // namespace mosquitto_socket

}  // namespace cc_plugin

}  // namespace mqtt

