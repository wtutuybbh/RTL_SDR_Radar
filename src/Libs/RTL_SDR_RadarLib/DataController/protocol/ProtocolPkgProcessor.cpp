#include <QDataStream>
#include <QDebug>
#include <QNetworkInterface>
#include <QDataStream>
#include <string>
#include <cstdint>
#include <algorithm>
#include <stdlib.h>


#include "ProtocolPkgProcessor.h"
#include "osm/coordUtils/Position.h"
#include "Carrier/Carrier.h"
#include "Carrier/ServiceLocator.h"

ProtocolPkgProcessor::ProtocolPkgProcessor(VERSION version) :
    _version(version)
{

    QString mac;

    mac = getMacAddress(QNetworkInterface::InterfaceType::Wifi);

    if(mac.isEmpty())
        mac = getMacAddress(QNetworkInterface::InterfaceType::Ethernet);

    if(mac.isEmpty())
        guid = 0xDEADBEAF;
    else
        guid = convert_mac(mac.toStdString());
}


uint64_t ProtocolPkgProcessor::convert_mac(std::string mac) {

    // Remove colons
    mac.erase(std::remove(mac.begin(), mac.end(), ':'), mac.end());

    // Convert to uint64_t
    return strtoul(mac.c_str(), NULL, 16);
}

/**
    структура для хранения и передачи блока данных на сервер
    .---------.--------.-------------.-------------.------------.-----------.--------------.
    | 1 byte  | 8 byte |   4 byte    |   4 byte    |  4  byte   |  4 byte   | 0 ... N byte |
    |---------|--------|-------------|-------------|------------|-----------|--------------|
    | version | guid   | receiverLat | receiverLon | countFrame | sizeFrame | data         |
    '---------'--------'-------------'-------------'------------'-----------'--------------'
    структура блока data
    .--------.--------.----------.--------.--------.----------.-----------.--------.---------.
    | 4 byte | 9 byte |  4 byte  | 4 byte | 4 byte |  4 byte  |  4 byte   | 8 byte | 4 byte  |
    |--------|--------|----------|--------|--------|----------|-----------|--------|---------|
    | icao   | flight | altitude | speed  | course | latitude | longitude | seen   | message |
    '--------'--------'----------'--------'--------'----------'-----------'--------'---------'
*/

QVector<QByteArray> ProtocolPkgProcessor::serialize(QList<QSharedPointer<IObject> > listObject)
{

    QByteArray array;
    QVector<QByteArray> dataFrame;

    if(listObject.isEmpty())
        return dataFrame;

    uint32_t frameSize = 0;
    uint32_t countFrame = 0;

    for(auto &a: listObject)
    {
        if(a.isNull())
            continue;

        if(frameSize == 0)
            frameSize = a->serializedFrameSize();

        if(array.size() < (UDP_MTU - frameSize - 25))
        {
            array.append(a->serialize());
            countFrame++;
            continue;
        }

        array = array.prepend(createHeader(countFrame,frameSize));
        dataFrame.append(array);

        array.clear();

        array.append(a->serialize());
        countFrame = 1;
    }

    if(!array.isEmpty())
    {
        array = array.prepend(createHeader(countFrame,frameSize));
        dataFrame.append(array);
    }

    return dataFrame;
}

QByteArray ProtocolPkgProcessor::createHeader(uint32_t countFrame, uint32_t frameSize)
{
    QByteArray header;
    QDataStream out (&header,QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);

    out << quint8(_version);
    out << quint64(guid);
    out << quint32(ServiceLocator::getCarrier()->getGeoCoord().latitude() / LAT_VALUE_LSB);
    out << quint32(ServiceLocator::getCarrier()->getGeoCoord().longitude() / LON_VALUE_LSB);
    out << quint32(countFrame);
    out << quint32(frameSize);

    return header;
}

QString ProtocolPkgProcessor::getMacAddress(QNetworkInterface::InterfaceType interType)
{
    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        // Return only the first non-loopback MAC Address
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack) &&
                netInterface.type() == interType)
            return  netInterface.hardwareAddress();
    }
    return QString();
}
