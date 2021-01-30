#ifndef PROTOCOLPKGPROCESSOR_H
#define PROTOCOLPKGPROCESSOR_H
#include <QNetworkInterface>
#include <QObject>
#include <math.h>

#include "interface/IObject.h"

enum class  VERSION : uint8_t { VER_1 = 1};

class ProtocolPkgProcessor
{
    const double LON_VALUE_LSB = 360.0 / pow(2, 31);
    const double LAT_VALUE_LSB = 180.0 / pow(2, 31);
    const u_int16_t UDP_MTU = 508;
    VERSION _version;
    uint64_t guid;
    uint64_t convert_mac(std::string mac);
    QString getMacAddress(QNetworkInterface::InterfaceType);
public:

    ProtocolPkgProcessor(VERSION version);

    QVector<QByteArray> serialize(QList<QSharedPointer<IObject>> listObject);
private:
    QByteArray createHeader(uint32_t countFrame, uint32_t frameSize);
};

#endif // PROTOCOLPKGPROCESSOR_H
