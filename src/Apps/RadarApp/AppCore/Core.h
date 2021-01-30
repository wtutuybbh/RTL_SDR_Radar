#ifndef CORE_H
#define CORE_H

#include <QObject>

#include <QDebug>
#include <chrono>
#include <QTimer>
#include <QFile>

#include "gui/MainWindow.h"
#include "gui/TableForm.h"

class IPoolObject;
class IDataController;
class IReciverDevice;
class IDemodulator;
class GraphicsWidget;
class ISubject;
class ModelTable;
class ILogger;
class IDSP;

class Core : public QObject
{
    Q_OBJECT
    ///< период обновления ифнормации
    int32_t TIMEOUT_UPDATE = 1000;

    uint32_t SIZE_WIDGET = 600;
    int sizeLog = 1000;
    ///< таймер обновления
    QTimer _timerUpdateWidgets;
    ///< основная форма
    MainWindow* _mainWindow = nullptr;
    ///< виджет радара
    GraphicsWidget* _graphicsWidget = nullptr;
    ///< виджет таблицы
    TableForm* _tableSrc = nullptr;
    ///< модель  таблицы
    ModelTable* _modelTable = nullptr;
    ///< пулл объектов
    QSharedPointer<IPoolObject> _poolObjects = nullptr;
    ///<контроллер данных
    QSharedPointer<IDataController> _dataController = nullptr;
    ///<устройство
    QSharedPointer<IReciverDevice> _device = nullptr;
    ///< демодулятор
    QSharedPointer<IDemodulator> _demodulator = nullptr;
    ///< поставщик из паттерна наблюдатель
    QSharedPointer<ISubject> _subject = nullptr;
    ///< логгер
    QSharedPointer<ILogger> _logger = nullptr;
    ///< библиотека ЦОС
    QSharedPointer<IDSP> _dsp;
    /*!
     * \brief updateGeoPositionInfo
     * обновление геопозиции радиотехнических объектов в пуле
     * объектов
     */
    void updateGeoPositionInfo();

public:
    explicit Core(QObject *parent = nullptr);
    ~Core();
    /*!
     * \brief init  -иинициализация приложения
     */
    void init();
    void init(const QString& ip , uint16_t port, int64_t interval_send_ms);
signals:

public slots:
    /*!
     * \brief slotUpdateWidgets слот обновления
     * данных на виджетах
     */
    void slotUpdateWidgets();

};

#endif // CORE_H
