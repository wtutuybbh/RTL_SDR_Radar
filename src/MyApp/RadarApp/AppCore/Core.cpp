#include "Core.h"
#include <QApplication>

#include "../MyLib/RTL_SDR_RadarLib/PoolObject/PoolObject.h"
#include "../MyLib/RTL_SDR_RadarLib/Logger/Logger.h"
#include "../MyLib/RTL_SDR_RadarLib/Carrier/Carrier.h"
#include "../MyLib/RTL_SDR_RadarLib/Carrier/ServiceLocator.h"
#include "../MyLib/RTL_SDR_RadarLib/DataController/DataController.h"
#include "../MyLib/RTL_SDR_RadarLib/RTL_SDR_Reciver/RTL_SDR_Reciver.h"
#include "../MyLib/RTL_SDR_RadarLib/Demodulator/Demodulator.h"
#include "../MyLib/RTL_SDR_RadarLib/GraphicsWidget/GraphicsWidget.h"
#include "../MyLib/RTL_SDR_RadarLib/ModelTable/ModelTable.h"
#include "../include/coord/Conversions.h"

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

    delete _mainWindow;
}

void Core::init()
{  
    ServiceLocator::provide(QSharedPointer<ICarrierClass>( new NullCarrier()) );

    _logger = QSharedPointer<ILogger>(new Logger(sizeLog));
    _device = QSharedPointer<IReciverDevice>(new RTL_SDR_Reciver());
    _device->setLogger(_logger);
    _device->openDevice();


    _poolObjects = QSharedPointer<IPoolObject>(new PoolObject(OBJECT_TYPE::air));

    addImitObject();

    _demodulator = QSharedPointer<IDemodulator>(new Demodulator(_poolObjects));
    _demodulator->setLogger(_logger);

    _dataController = QSharedPointer<IDataController>(new DataController(_device,
                                                                         _demodulator));
    _dataController->run();


    _subject = QSharedPointer<ISubject>(new Subject());

    _mainWindow  = new MainWindow();
    _mainWindow->setLogger(_logger);

    _graphicsWidget = new GraphicsWidget(SIZE_WIDGET, _mainWindow);
    _graphicsWidget->subscribe(_subject);
    _mainWindow->addGraphicsWidget(_graphicsWidget);

    _modelTable = new ModelTable(_mainWindow);
    _modelTable->subscribe(_subject);

    _tableSrc = new TableForm(_mainWindow);
    _tableSrc->setTableModel(_modelTable);
    _mainWindow->addTableWidget(_tableSrc);

    _mainWindow->adjustSize();
    _mainWindow->show();


    QObject::connect(&_timerUpdateWidgets, &QTimer::timeout,
                     this, &Core::slotUpdateWidgets);
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
            if(iter->isImitated())
            {
                static uint16_t azim = 0;
                iter->setGeoCoord(Conversions::getPlace(ServiceLocator::getCarrier()->getGeoCoord(),++azim,20000 + (qrand()%100)*10));
                if(iter->getId() == 2 || iter->getId() == 5)
                    iter->setDateTimeStop(QDateTime::currentDateTime());
                azim %= 360;
            }
            iter->setAzimuth(Conversions::LBToPeleng(ServiceLocator::getCarrier()->getGeoCoord(),
                                                     iter->getGeoCoord()));

            iter->setDistance_M(Conversions::getDistance(ServiceLocator::getCarrier()->getGeoCoord(),
                                                         iter->getGeoCoord()));
        }
    }
}

void Core::addImitObject()
{
    _poolObjects->lockPool();

    for (int i = 1; i < 10; i++)
    {
        auto object = _poolObjects->createNewObject(i,
                                                    QDateTime::currentDateTime(),
                                                    Conversions::getPlace(ServiceLocator::getCarrier()->getGeoCoord(),
                                                                          (qrand()*10) % 360,
                                                                          20000 + qrand()*1000),
                                                    true);
        object->setObjectName("ImitObject"+i);
    }
    _poolObjects->unlockPool();

}


