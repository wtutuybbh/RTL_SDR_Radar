#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QTimer>

#include "ui/Mainwindow.h"

class IDataController;
class IPoolObject;
class IReciverDevice;
class IDemodulator;
class ILogger;
class INetworkWorker;

class Core : public QObject
{
    Q_OBJECT
    int sizeLog = 1000;

    const uint32_t TIMEOUT = 1000;
    QTimer _timer;
    MainWindow _mainWindow;

    QSharedPointer<IPoolObject> _poolObjects;
    QSharedPointer<IDataController> _dataController;
    QSharedPointer<IReciverDevice> _device;
    QSharedPointer<IDemodulator> _demodulator;
    QSharedPointer<ILogger> _logger;

public:
    explicit Core(QObject *parent = nullptr);
    ~Core();

    void init();
    void init(const QString& ip , uint16_t port);
signals:

public slots:
    void slotTimeout();

};

#endif // CORE_H
