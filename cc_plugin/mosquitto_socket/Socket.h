//
// Copyright 2016 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <list>
#include <memory>

#include "comms/CompileControl.h"

CC_DISABLE_WARNINGS()
#include <QtCore/QStringList>
#include <QtCore/QString>
CC_ENABLE_WARNINGS()

#include <mosquitto.h>

#include "comms_champion/Socket.h"


namespace mqtt
{

namespace cc_plugin
{

namespace mosquitto_socket
{

class Socket : public QObject,
               public comms_champion::Socket
{
    Q_OBJECT
    using Base = comms_champion::Socket;

public:
    typedef unsigned short PortType;

    Socket();
    ~Socket();

    void setId(const QString& value)
    {
        m_id = value;
    }

    const QString& getId() const
    {
        return m_id;
    }

    void setHost(const QString& value)
    {
        m_host = value;
    }

    const QString& getHost() const
    {
        return m_host;
    }

    void setPort(PortType value)
    {
        m_port = value;
    }

    PortType getPort() const
    {
        return m_port;
    }

    void setCleanSession(bool value)
    {
        m_cleanSession = value;
    }

    bool getCleanSession() const
    {
        return m_cleanSession;
    }

    void setKeepAlivePeriod(int value)
    {
        m_keepAlivePeriod = value;
    }

    int getKeepAlivePeriod() const
    {
        return m_keepAlivePeriod;
    }

    void setSubTopics(const QString& value);

    QString getSubTopics() const;

    void setPubTopic(const QString& value)
    {
        m_pubTopic = value;
    }

    const QString& getPubTopic() const
    {
        return m_pubTopic;
    }

    void setPubQos(int value)
    {
        m_pubQos = value;
    }

    int getPubQos() const
    {
        return m_pubQos;
    }

    void setSubQos(int value)
    {
        m_subQos = value;
    }

    int getSubQos() const
    {
        return m_subQos;
    }

    //---

    bool setConnected(bool connected);

    bool connectToServer();

    bool disconnectFromServer();

    bool isConnected() const
    {
        return m_connected;
    }

signals:
    void sigOnConnect(int rc);
    void sigOnDisconnect(int rc);
    void sigOnMessage(const struct mosquitto_message* msg);
protected:
    virtual bool startImpl() override;
    virtual void stopImpl() override;
    virtual bool socketConnectImpl() override;
    virtual void socketDisconnectImpl() override;
    virtual void sendDataImpl(comms_champion::DataInfoPtr dataPtr) override;

private:
    struct MosqDeleter
    {
        void operator()(struct mosquitto * mosq)
        {
            ::mosquitto_destroy(mosq);
        }
    };
    typedef std::unique_ptr<struct mosquitto, MosqDeleter> MosqPtr;

private slots:
    void onConnectInternal(int rc);
    void onDisconnectInternal(int rc);
    void onMessageInternal(const struct mosquitto_message* msg);

private:
    static void onConnect(struct mosquitto* mosq, void* obj, int rc);
    static void onDisconnect(struct mosquitto* mosq, void* obj, int rc);
    static void onMessage(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg);
    static Socket* checkCallback(struct mosquitto* mosq, void* obj);

    void reportConnectFromThread(int rc);
    void reportDisconnectFromThread(int rc);
    void reportMessageFromThread(const struct mosquitto_message* msg);

    static const PortType DefaultPort = 1883;
    MosqPtr m_mosq;
    QString m_id;
    QString m_host;
    PortType m_port = DefaultPort;
    bool m_cleanSession = true;
    int m_keepAlivePeriod = 60;
    QStringList m_subTopics;
    QString m_pubTopic;
    int m_pubQos = 0;
    int m_subQos = 0;

    bool m_connected = false;
    bool m_tryingToConnect = false;
};

}  // namespace mosquitto_socket

} // namespace cc_plugin

} // namespace mqtt
