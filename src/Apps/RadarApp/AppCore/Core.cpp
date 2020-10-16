#include "Core.h"
#include <QApplication>

#include "PoolObject/PoolObject.h"
#include "Logger/Logger.h"
#include "Carrier/Carrier.h"
#include "Carrier/ServiceLocator.h"
#include "DataController/DataController.h"
#include "RTL_SDR_Reciver/RTL_SDR_Reciver.h"
#include "Demodulator/Demodulator.h"
#include "GraphicsWidget/GraphicsWidget.h"
#include "ModelTable/ModelTable.h"
#include "Carrier/NullCarrier.h"
#include "DSPLib/DSPLib.h"
#include "osm/coordUtils/Conversions.h"
#include "publisher/Subject.h"

Core::Core(QObject *parent) : QObject(parent)
{

    QFile File(QApplication::applicationDirPath()+"/../../../import/style.qss");
    if(File.open(QFile::ReadOnly))
    {
        QString StyleSheet = QLatin1String(File.readAll());
        qApp->setStyleSheet(StyleSheet);
    }
    else
        qDebug()<<"error load QSS file.Need filepath"
               <<QApplication::applicationDirPath()+"/import/style.qss";

    //носитель приемника стационарный объект
    ServiceLocator::provide(QSharedPointer<ICarrierClass>( new NullCarrier()) );
    //модуль логгирования
    _logger = QSharedPointer<ILogger>(new Logger(sizeLog));
    //модуль взаимодействия с приемником
    _device = QSharedPointer<IReciverDevice>(new RTL_SDR_Reciver());
    _device->setLogger(_logger);
    _device->openDevice();

    _dsp = QSharedPointer<IDSP> (new DSPLib());

    //пулл объектов для хранения объектов типа самолет
    _poolObjects = QSharedPointer<IPoolObject>(new PoolObject(OBJECT_TYPE::air));

    //демодулятор входного сигнала
    _demodulator = QSharedPointer<IDemodulator>(new Demodulator(_poolObjects));
    _demodulator->setLogger(_logger);

    //паттерн наблюдатель наблюдатель
    _subject = QSharedPointer<ISubject>(new Subject());
    //основная форма
    _mainWindow  = new MainWindow();
    _mainWindow->setLogger(_logger);
    _mainWindow->setDsp(_dsp);
    //виджет радара и вывода значков
    _graphicsWidget = new GraphicsWidget(SIZE_WIDGET, _mainWindow);
    //подписка на события
    _graphicsWidget->subscribe(_subject);
    _mainWindow->addGraphicsWidget(_graphicsWidget);


    //модель таблицы
    _modelTable = new ModelTable(_mainWindow);
    //подписка на события
    _modelTable->subscribe(_subject);

    //виджет таблицы
    _tableSrc = new TableForm(_mainWindow);
    _tableSrc->setTableModel(_modelTable);
    _mainWindow->addTableWidget(_tableSrc);

    _mainWindow->adjustSize();
    _mainWindow->show();

    QObject::connect(&_timerUpdateWidgets, &QTimer::timeout,
                     this, &Core::slotUpdateWidgets);
}

Core::~Core()
{
    _dataController->stop();
    _dataController.clear();

    _device->closeDevice();
    _device.clear();

    _demodulator.clear();

    _subject->Deatach(_graphicsWidget);
    delete _graphicsWidget;

    _subject->Deatach(_modelTable);
    delete _modelTable;

    _subject.clear();

    _poolObjects.clear();

    _dsp.clear();
    delete _mainWindow;
}

void Core::init()
{  
    //контроллер данных
    _dataController = QSharedPointer<IDataController>(new DataController(_device,
                                                                         _demodulator));
    _dataController->setDSP(_dsp);
    _dataController->run();

    _timerUpdateWidgets.start(TIMEOUT_UPDATE);
}

void Core::init(const QString &ip, uint16_t port, int64_t interval_send_ms)
{

    _dataController = QSharedPointer<IDataController>(new DataController(_device,
                                                                         _demodulator,
                                                                         ip,
                                                                         port,
                                                                         interval_send_ms));
    _dataController->setDSP(_dsp);
    _dataController->run();

    _timerUpdateWidgets.start(TIMEOUT_UPDATE);
}


void Core::slotUpdateWidgets()
{
    if(_poolObjects.isNull())
        return;

    if(_poolObjects->tryLockPool())
    {
        if(!_device.isNull() && (_mainWindow != nullptr))
            _mainWindow->setReciverDeviceState(_device->isOpenDevice());

        updateGeoPositionInfo();
        _subject->Notify(_poolObjects);

        _poolObjects->unlockPool();
    }
}

void Core::updateGeoPositionInfo()
{
    for (auto &iter: _poolObjects->allValues())
    {
        if(!iter.isNull() && iter->isValidGeoCoord())
        {
            iter->setAzimuth(Conversions::LBToPeleng(ServiceLocator::getCarrier()->getGeoCoord(),
                                                     iter->getGeoCoord()));

            iter->setDistance_M(Conversions::getDistance(ServiceLocator::getCarrier()->getGeoCoord(),
                                                         iter->getGeoCoord()));
        }
    }
}



