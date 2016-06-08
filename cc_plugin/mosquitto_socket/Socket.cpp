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

#include "Socket.h"

#include <cassert>
#include <type_traits>

#include "comms/CompileControl.h"

CC_DISABLE_WARNINGS()
#include <QtNetwork/QHostAddress>
CC_ENABLE_WARNINGS()

#include "comms/util/ScopeGuard.h"

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

namespace mosquitto_socket
{

namespace
{

const QChar SubTopicsSep(';');

class MosquittoInitialiser
{
public:
    static const MosquittoInitialiser& instance()
    {
        static const MosquittoInitialiser Obj;
        return Obj;
    }

    ~MosquittoInitialiser()
    {
        ::mosquitto_lib_cleanup();
    }

private:

    MosquittoInitialiser()
    {
        ::mosquitto_lib_init();
    }

    MosquittoInitialiser(const MosquittoInitialiser&) = delete;
    MosquittoInitialiser& operator=(const MosquittoInitialiser&) = delete;
};

const QString TopicPropName("mqtt.topic");
const QString QosPropName("mqtt.qos");

//const QString FromPropName("mqtt.from");
//const QString ToPropName("mqtt.to");

}  // namespace


Socket::Socket()
{
    static_cast<void>(MosquittoInitialiser::instance());

    connect(
        this, SIGNAL(sigOnConnect(int)),
        this, SLOT(onConnectInternal(int)),
        Qt::QueuedConnection);

    connect(
        this, SIGNAL(sigOnDisconnect(int)),
        this, SLOT(onDisconnectInternal(int)),
        Qt::QueuedConnection);

    connect(
        this, SIGNAL(sigOnMessage(const struct mosquitto_message*)),
        this, SLOT(onMessageInternal(const struct mosquitto_message*)),
        Qt::BlockingQueuedConnection);
}

Socket::~Socket()
{
    blockSignals(true);
}

void Socket::setSubTopics(const QString& value)
{
    m_subTopics = value.split(SubTopicsSep, QString::SkipEmptyParts);
}

QString Socket::getSubTopics() const
{
    QString str;
    for (auto& s : m_subTopics) {
        if (s.isEmpty()) {
            continue;
        }

        if (!str.isEmpty()) {
            str.append(SubTopicsSep);
        }
        str.append(s);
    }
    return str;
}

bool Socket::setConnected(bool connected)
{
    if (connected) {
        return connectToServer();
    }

    return disconnectFromServer();
}

bool Socket::connectToServer()
{
    assert(m_mosq);
    if (m_tryingToConnect || m_connected) {
        assert(!"Already connected or trying to connect.");
        static const QString AlreadyConnectedError(
            tr("Socket is already connected or trying to connect."));
        reportError(AlreadyConnectedError);
        return false;
    }

    if (m_host.isEmpty()) {
        m_host = QHostAddress(QHostAddress::LocalHost).toString();
    }

    int result = ::mosquitto_connect_async(
        m_mosq.get(),
        m_host.toUtf8().
        constData(),
        m_port,
        m_keepAlivePeriod);

    if (result != MOSQ_ERR_SUCCESS) {
        static const QString Error(
            tr("Mosquitto library error, failed to initiate connection."));
        reportError(Error);
        return false;
    }

    result = ::mosquitto_loop_start(m_mosq.get());
    if (result != MOSQ_ERR_SUCCESS) {
        static const QString Error(
            tr("Mosquitto library error, failed to start processing loop."));
        reportError(Error);
        return false;
    }

    m_tryingToConnect = true;
    m_forcedDisconnection = false;

    //m_socket.connectToHost(m_host, m_port);
    return true;
}

bool Socket::disconnectFromServer()
{
    m_tryingToConnect = false;
    m_forcedDisconnection = true;
    assert(m_mosq);
    bool result = true;
    int es = ::mosquitto_disconnect(m_mosq.get());
    if (es != MOSQ_ERR_SUCCESS) {
        static const QString Error(
            tr("Mosquitto library error, failed to disconnect from broker."));
        reportError(Error);
        result = false;
    }

    es = ::mosquitto_loop_stop(m_mosq.get(), true);
    if (es != MOSQ_ERR_SUCCESS) {
        static const QString Error(
            tr("Mosquitto library error, failed to stop processing loop."));
        reportError(Error);
        result = false;
    }

    return result;
}

bool Socket::startImpl()
{
    std::string idStr = m_id.toStdString();
    const char* id = nullptr;
    if (!idStr.empty()) {
        id = idStr.c_str();
    }

    m_mosq.reset(::mosquitto_new(id, m_cleanSession, this));
    if (!m_mosq) {
        static const QString Error(
            tr("Mosquitto library error, failed to initialise."));
        reportError(Error);
        return false;
    }

    auto exitGuard =
        comms::util::makeScopeGuard(
            [this]()
            {
                m_mosq.reset();
            });

    ::mosquitto_connect_callback_set(m_mosq.get(), &Socket::onConnect);
    ::mosquitto_disconnect_callback_set(m_mosq.get(), &Socket::onDisconnect);
    ::mosquitto_message_callback_set(m_mosq.get(), &Socket::onMessage);

    exitGuard.release();
    return true;
}

void Socket::stopImpl()
{
    disconnectFromServer();
    m_mosq.reset();
}

void Socket::sendDataImpl(cc::DataInfoPtr dataPtr)
{
    assert(dataPtr);
    if (dataPtr->m_data.empty()) {
        return;
    }

    QString topic = getPubTopic();
    auto topicVar = dataPtr->m_extraProperties.value(TopicPropName);
    bool topicProvided = (topicVar.isValid() && topicVar.canConvert<QString>());
    if (topicProvided) {
        auto topicTmp = topicVar.toString();
        if (!topicTmp.isEmpty()) {
            topic = std::move(topicTmp);
        }
    }

    if (topic.isEmpty()) {
        static const QString Error(
            tr("Cannot send message without publish topic."));
        return;
    }

    int qos = getPubQos();
    auto qosVar = dataPtr->m_extraProperties.value(QosPropName);
    bool qosProvided = qosVar.isValid() && qosVar.canConvert<int>();
    if (qosProvided) {
        qos = qosVar.toInt();
    }

    assert(m_mosq);
    int pubResult =
        ::mosquitto_publish(
            m_mosq.get(),
            nullptr,
            topic.toUtf8().constData(),
            dataPtr->m_data.size(),
            &dataPtr->m_data[0],
            qos,
            false);

    if (!pubResult) {
        static const QString Error(
            tr("Failed to publish message."));
        return;
    }

    if (!topicProvided) {
        dataPtr->m_extraProperties.insert(TopicPropName, topic);
    }

    if (!qosProvided) {
        dataPtr->m_extraProperties.insert(QosPropName, qos);
    }
}

void Socket::onConnectInternal(int rc)
{
    m_tryingToConnect = false;
    if (rc != 0) {
        static const QString ErrorPrefix("Failed to connect to MQTT broker: ");
        static const QString SuffixMap[] = {
            "success",
            "unacceptable protocol version",
            "identifier rejected",
            "broker unavailable"
        };
        static const std::size_t SuffixMapSize =
            std::extent<decltype(SuffixMap)>::value;

        QString error(ErrorPrefix);
        if ((rc < 0) || (static_cast<int>(SuffixMapSize) <= rc)) {
            static const QString UnknownStr("unknown error");
            error.append(UnknownStr);
        }
        else {
            error.append(SuffixMap[rc]);
        }
        reportError(error);
        emit sigConnectionStatus(false);
        return;
    }

    m_connected = true;
    emit sigConnectionStatus(true);

    for (auto& sub : m_subTopics) {
        if (sub.isEmpty()) {
            continue;
        }

        int result =
            ::mosquitto_subscribe(m_mosq.get(), nullptr, sub.toUtf8().constData(), m_subQos);

        if (result != MOSQ_ERR_SUCCESS) {
            static const QString ErrorPrefix("Failed to subscribe to topic: ");
            reportError(ErrorPrefix + sub);
        }
    }
}

void Socket::onDisconnectInternal(int rc)
{
    m_tryingToConnect = false;
    m_connected = false;

    emit sigConnectionStatus(false);

    if ((m_forcedDisconnection) || (rc == 0)) {
        return;
    }

    static const QString ErrorPrefix("Disconnected from MQTT broker: ");
    QString error(ErrorPrefix);
    error.append(::mosquitto_connack_string(rc));
    reportError(error);
}

void Socket::onMessageInternal(const struct mosquitto_message* msg)
{
    if (msg == nullptr) {
        return;
    }

    auto dataPtr = cc::makeDataInfo();
    dataPtr->m_timestamp = cc::DataInfo::TimestampClock::now();

    auto* begIter =
        reinterpret_cast<cc::DataInfo::DataSeq::const_pointer>(msg->payload);
    auto* endIter = begIter + msg->payloadlen;
    dataPtr->m_data.assign(begIter, endIter);

    dataPtr->m_extraProperties.insert(TopicPropName, msg->topic);
    dataPtr->m_extraProperties.insert(QosPropName, msg->qos);
}

void Socket::onConnect(struct mosquitto* mosq, void* obj, int rc)
{
    auto* thisPtr = checkCallcack(mosq, obj);
    if (thisPtr != nullptr) {
        thisPtr->onConnectInternal(rc);
    }
}

void Socket::onDisconnect(struct mosquitto* mosq, void* obj, int rc)
{
    auto* thisPtr = checkCallcack(mosq, obj);
    if (thisPtr != nullptr) {
        thisPtr->onDisconnectInternal(rc);
    }
}

void Socket::onMessage(
    struct mosquitto* mosq,
    void* obj,
    const struct mosquitto_message* msg)
{
    auto* thisPtr = checkCallcack(mosq, obj);
    if (thisPtr != nullptr) {
        thisPtr->onMessageInternal(msg);
    }
}

Socket* Socket::checkCallcack(struct mosquitto* mosq, void* obj)
{
    if (obj == nullptr) {
        assert(!"NULL object is provided");
        return nullptr;
    }

    auto* thisPtr = reinterpret_cast<Socket*>(obj);
    if (thisPtr->m_mosq.get() != mosq) {
        assert(!"Invalid object is provided");
        return nullptr;
    }

    return thisPtr;
}

}  // namespace mosquitto_socket

} // namespace cc_plugin

} // namespace mqtt
