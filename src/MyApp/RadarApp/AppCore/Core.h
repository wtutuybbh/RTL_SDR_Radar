#ifndef CORE_H
#define CORE_H

#include <QObject>

#include <QDebug>
#include <chrono>
#include <QTimer>
#include <QFile>

#include "gui/MainWindow.h"

class IPoolObject;
class IDataController;
class IReciverDevice;
class IDemodulator;
class GraphicsWidget;
class ISubject;

class Core : public QObject
{
    Q_OBJECT

    int32_t TIMEOUT = 1000;
    int32_t TIMEOUT_UPDATE = 250;

    uint32_t SIZE_WIDGET = 600;

    uint32_t _ticks = 0;
    QTimer _timerUpdateWidgets;

    MainWindow* _mainWindow = nullptr;
    GraphicsWidget* _graphicsWidget = nullptr;

    QSharedPointer<IPoolObject> _poolObjects = nullptr;
    QSharedPointer<IDataController> _dataController = nullptr;
    QSharedPointer<IReciverDevice> _device = nullptr;
    QSharedPointer<IDemodulator> _demodulator = nullptr;
    QSharedPointer<ISubject> _subject = nullptr;

public:
    explicit Core(QObject *parent = nullptr);
    ~Core();

    void init();
signals:

public slots:
    void slotUpdateWidgets();

};

#endif // CORE_H
