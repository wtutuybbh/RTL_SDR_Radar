#ifndef CORE_H
#define CORE_H

#include <QObject>

#include <QDebug>
#include <chrono>
#include <QTimer>
#include <QFile>

#include "gui/MainWindow.h"

class IDataController;
class IReciverDevice;
class IDSP;

class Core : public QObject
{
    Q_OBJECT
    ///< период обновления ифнормации
    int32_t TIMEOUT_UPDATE = 16;

    uint32_t SIZE_WIDGET = 600;
    int sizeLog = 1000;
    ///< таймер обновления
    QTimer _timerUpdateWidgets;
    ///< основная форма
    MainWindow* _mainWindow = nullptr;
    ///<контроллер данных
    QSharedPointer<IDataController> _dataController = nullptr;
    ///<устройство
    QSharedPointer<IReciverDevice> _device = nullptr;
    ///< библиотека ЦОС
    QSharedPointer<IDSP> _dsp = nullptr;
public:
    explicit Core(QObject *parent = nullptr);
    ~Core();
    /*!
     * \brief init  -иинициализация приложения
     */
    void init();
    QSharedPointer<IReciverDevice> device() const;

    QSharedPointer<IDSP> dsp() const;

    QSharedPointer<IDataController> dataController() const;

};

#endif // CORE_H
